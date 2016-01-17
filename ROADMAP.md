# - Game

# - Systems:

## - artificial intelligence

## - state machine

## - behavior

## - graphics

## - transform

## - physics

## - audio

# - Managers:

## - entities

## - controller

## - command dispatcher

# World:

## - stage

## - layer

## - entity

# - Components:

## - atlas ~> graphics

## - image ~> graphics

## - image atlas ~> graphics
* extends image

## - particle emitter ~> graphics
* extends image atlas

## - tiles ~> graphics
* extends atlas

## - 9slicesImage ~> graphics
* extends image

## - label bmp ~> graphics
* extends image atlas

## - label freetype ~> graphics
* extends label bmp

## - static body ~> physics

## - kinematic body ~> physics

## - dynamic body ~> physics
