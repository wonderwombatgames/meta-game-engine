/**
  * implement graphic backend functions using SDL
  *
  */

#include <algorithm>
#include <cmath>

#include "backends/handler_sdl.hpp"
#include "systems/graphics.hpp"

namespace
{
using namespace W2E;
using namespace Utils;

inline void BoxBoundXYWH2SDLRect(const BoxBoundXYWH& box, SDL_Rect& rect)
{
  rect.x = static_cast< int >(box.topLeft.x);
  rect.y = static_cast< int >(box.topLeft.y);
  rect.w = static_cast< int >(box.size.width);
  rect.h = static_cast< int >(box.size.height);
}

inline SDL_BlendMode mapBlendMode(eBlendMode mode)
{
  SDL_BlendMode map[] = {
      SDL_BLENDMODE_NONE, SDL_BLENDMODE_BLEND, SDL_BLENDMODE_ADD, SDL_BLENDMODE_MOD};
  return map[mode];
}

void computeClipRects(const W2E::Component::GraphicPod& component,
                      SpaceDim rw,
                      SpaceDim rh,
                      i32 tw,
                      i32 th,
                      BoxBoundXYWH& src,
                      BoxBoundXYWH& dst,
                      Vector2& center)
{
  // compute Image parameters in px
  i32 tx = 0;
  i32 ty = 0;
  f32 sx = 1.0f;
  f32 sy = 1.0f;

  if(nullptr != component.transformData)
  {
    tx = static_cast< i32 >(component.transformData->position.x);
    ty = static_cast< i32 >(component.transformData->position.y);
    sx = component.transformData->scale.x;
    sy = component.transformData->scale.y;
  }

  f32 ax = component.anchor.x;
  f32 ay = component.anchor.y;
  i32 w = static_cast< i32 >(tw * std::abs(sx));
  i32 h = static_cast< i32 >(th * std::abs(sy));
  i32 cx = static_cast< i32 >(ax * w);
  i32 cy = static_cast< i32 >(ay * h);
  i32 x = tx - cx;
  i32 y = ty - cy;

  src.topLeft.x = 0;
  src.topLeft.y = 0;
  src.size.width = tw;
  src.size.height = th;

  dst.topLeft.x = x;
  dst.topLeft.y = y;
  dst.size.width = w;
  dst.size.height = h;

  center.x = cx;
  center.y = cy;
}
}

namespace W2E
{

using namespace BE;

class Display : public DisplayInterface
{
public:
  Display(const BoxBoundXYWH& rect, Flags flags = 0);
  virtual ~Display(){};
  Display() = delete;
  Display(Display& other) = delete;

  // rendering
  virtual void render() override;
  virtual void clear() override;
  virtual void clear(const Colour& c) override;

  // reseting the viewport
  virtual void setColour(const Colour& c) override;
  virtual const Colour& getColour() const override;
  virtual void setResolution(Dimension2& res) override;
  virtual const Dimension2& getResolution() const override;
  virtual void setViewRect(const BoxBoundXYWH& rect) override;
  virtual const BoxBoundXYWH& getViewRect() const override;
  virtual void setTitle(const String& title) override;
  virtual const char* getTitle() const override;
  virtual void setFullscreen(bool fs) override;
  virtual const bool isFullscreen() const override;

protected:
  using HANDLER_ = SDL2::Handler;

  // data
  UniquePtr< HANDLER_ > data_;
  BoxBoundXYWH rect_;
  Colour background_;
};

// Display
void Display::render() { SDL_RenderPresent(data_->view_->renderer_); }

void Display::setColour(const Colour& c)
{
  this->background_ = c;
  u8 r = 0;
  u8 g = 0;
  u8 b = 0;
  u8 a = 0;
  SDL2::colour8RGBA(r, g, b, a, c);

  SDL_SetRenderDrawColor(data_->view_->renderer_, r, g, b, a);
}

const Colour& Display::getColour() const { return this->background_; }

void Display::setTitle(const String& title)
{
  SDL_SetWindowTitle(data_->view_->window_, title.c_str());
}

const char* Display::getTitle() const { return SDL_GetWindowTitle(data_->view_->window_); }

void Display::clear(const Colour& c)
{
  this->setColour(c);
  SDL_RenderClear(data_->view_->renderer_);
}

void Display::clear()
{
  this->setColour(this->background_);
  SDL_RenderClear(data_->view_->renderer_);
}

void Display::setFullscreen(bool fs)
{
  int flag = 0;
  if(fs)
  {
    flag = SDL_WINDOW_FULLSCREEN;
  }

  SDL_SetWindowFullscreen(data_->view_->window_, flag);
  this->setColour(this->background_);
}

const bool Display::isFullscreen() const
{
  return (SDL_GetWindowFlags(data_->view_->window_) == SDL_WINDOW_FULLSCREEN);
}

void Display::setResolution(Dimension2& res)
{
  // only update if different
  if(rect_.size.width != res.width || rect_.size.height != res.height)
  {
    rect_.size = res;
    int width = static_cast< int >(rect_.size.width);
    int height = static_cast< int >(rect_.size.height);

    SDL_Rect rect;
    SDL_RenderGetViewport(data_->view_->renderer_, &rect);
    rect.w = width;
    rect.h = height;

    SDL_SetWindowSize(data_->view_->window_, width, height);
    SDL_RenderSetLogicalSize(data_->view_->renderer_, width, height);
    SDL_RenderSetViewport(data_->view_->renderer_, &rect);
    SDL_RenderSetClipRect(data_->view_->renderer_, &rect);
    this->setColour(this->background_);

    data_->view_->resolution_ = res;
  }
}

const Dimension2& Display::getResolution() const { return rect_.size; }

void Display::setViewRect(const BoxBoundXYWH& rect)
{
  rect_ = rect;

  this->setResolution(rect_.size);
  this->setColour(this->background_);
}

const BoxBoundXYWH& Display::getViewRect() const { return rect_; }

Display::Display(const BoxBoundXYWH& rect, Flags flags)
    : data_{new HANDLER_}
    , rect_{rect}
    , background_{RGBA, {{0.0, 0.0, 0.0, 1.0}}}
{
  this->setViewRect(rect);
  this->setColour(this->background_);
}

namespace Component
{

// Image
template <>
bool Image< SDL2::Handler >::isLoaded()
{
  return (data_->image_ && data_->image_->buffer_);
}

template <>
ErrorCode Image< SDL2::Handler >::loadFromFile(const String& filepath)
{
  data_->image_ = SDL2::Texture::createTexture(filepath, data_->view_->renderer_);

  // query window
  int rw = data_->view_->resolution_.width;
  int rh = data_->view_->resolution_.height;

  // query Image
  int tw = data_->image_->rect_.size.width;
  int th = data_->image_->rect_.size.height;

  // compute the relative size
  SpaceDim w = static_cast< SpaceDim >(tw) / static_cast< SpaceDim >(rw);
  SpaceDim h = static_cast< SpaceDim >(th) / static_cast< SpaceDim >(rh);

  // size - between 0.0 - 1.0 (in relation to Display size)
  textureSize_ = {{w, h}};

  return (this->isLoaded()) ? NO_ERROR : UNKNOWN_ERROR;
}

template <>
Image< SDL2::Handler >::Image(const String& filepath)
    : data_{new HANDLER_}
    , textureSize_{{0.0f, 0.0f}}
{
  this->loadFromFile(filepath);
}

template <>
Image< SDL2::Handler >::Image()
    : data_{new HANDLER_}
    , textureSize_{{0.0f, 0.0f}}
{
}

template <>
Image< SDL2::Handler >::~Image()
{
}

template <>
void Image< SDL2::Handler >::paint(const GraphicPod& component, const TransformPod& transformData)
{
  if(data_->view_->renderer_ && this->isLoaded() && component.isVisible &&
     component.transformData != nullptr)
  {
    BoxBoundXYWH src;
    BoxBoundXYWH dst;
    Vector2 center;

    i32 rw = data_->view_->resolution_.width;
    i32 rh = data_->view_->resolution_.height;
    i32 tw = static_cast< i32 >(textureSize_.width * rw);
    i32 th = static_cast< i32 >(textureSize_.height * rh);

    computeClipRects(component, rw, rh, tw, th, src, dst, center);

    dst.topLeft.x -= static_cast< int >(transformData.position.x);
    dst.topLeft.y -= static_cast< int >(transformData.position.y);

    float rot = 0.0f;
    float sx = 1.0f;
    float sy = 1.0f;
    if(nullptr != component.transformData)
    {
      sx = component.transformData->scale.x;
      sy = component.transformData->scale.y;
      rot = component.transformData->rotation.yaw;
    }

    double angle = fmod((rot * 360.0), 360.0);
    SDL_Point sdlcenter_ = {static_cast< int >(center.x), static_cast< int >(center.y)};

    // calculate  src and dst rectangles
    SDL_Rect srcrect_;
    BoxBoundXYWH2SDLRect(src, srcrect_);

    SDL_Rect dstrect_;
    BoxBoundXYWH2SDLRect(dst, dstrect_);

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if(sx < 0.0)
    {
      flip = static_cast< SDL_RendererFlip >(flip | SDL_FLIP_HORIZONTAL);
    }
    if(sy < 0.0)
    {
      flip = static_cast< SDL_RendererFlip >(flip | SDL_FLIP_VERTICAL);
    }

    // blending mode
    SDL_SetTextureBlendMode(data_->image_->buffer_,
                            static_cast< SDL_BlendMode >(mapBlendMode(component.blendingMode)));

    // color modulation
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;
    SDL2::colour8RGBA(r, g, b, a, component.colourTint);
    SDL_SetTextureColorMod(data_->image_->buffer_, r, g, b);

    // alpha mode
    u8 alpha = static_cast< u8 >(255 * component.alphaMode);
    SDL_SetTextureAlphaMod(data_->image_->buffer_, alpha);

    // paint the Image
    SDL_RenderCopyEx(data_->view_->renderer_,
                     data_->image_->buffer_,
                     &srcrect_,
                     &dstrect_,
                     angle,
                     &sdlcenter_,
                     flip);
  }
}

} // end namespace Component

namespace System
{

DisplayHandler Graphics::createDisplay(const BoxBoundXYWH& rect, Flags flags)
{
  this->display = std::make_shared< Display >(rect, flags);
  return this->display;
}

Graphics::Graphics()
    : SystemsInterface{"Graphics"}
    , display{nullptr}
    , camera_{{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{1.0f}}}
    , components_{}
    , resources_{}
{
  SDL2::initGraphicSystem();
  this->camera_ = {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{1.0f}}};
}

Graphics::Graphics(const char* name)
    : SystemsInterface{name}
    , display{nullptr}
    , camera_{{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{1.0f}}}
    , components_{}
    , resources_{}
{
  SDL2::initGraphicSystem();
  SDL2::initGraphicSystem();
  this->camera_ = {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{1.0f}}};
}

Graphics::~Graphics() { SDL2::quitGraphicSystem(); }

} // end namespace System

} // end namespace W2E
