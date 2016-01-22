#!/bin/sh
echo "// unity build " > unity.cpp
for i in $(ls *.cpp | grep -v unity); do echo "#include \"${i}\"" >> unity.cpp; done
