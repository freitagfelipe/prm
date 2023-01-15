#!/bin/bash

set -e

echo "Installing PRM..."

if ! [ -x "$(command -v cmake)" ]; then
    echo "You need CMake to build PRM"

    exit 1
elif ! [ -x "$(command -v make)" ]; then
    echo "You need make to build PRM"

    exit 1
fi

cd ..
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

if [ ! -d $HOME/.local/bin ]; then
    echo "Creating the following folder to store the executable: $HOME/.local/bin"

    mkdir -p $HOME/.local/bin
fi

echo "Moving the PRM executable to the $HOME/.local/bin"

mv prm $HOME/.local/bin

if ! grep -q "PATH=\"\$HOME/.local/bin:" "$HOME/.profile"; then
    echo "Adding $HOME/.local/bin to the PATH in $HOME/.profile"

    echo 'if [ -d "$HOME/.local/bin" ] ; then PATH="$HOME/.local/bin:$PATH" fi' >> $HOME/.profile
fi

echo "Finished the installation"
