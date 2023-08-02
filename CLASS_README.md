# VIP HELICS

The class fork of the HELICS repository.

## Setup

Follow the [first time setup guide](CLASS_SETUP.md) for the class.

## Building Helics

```
mkdir build
cd build

cmake -DCMAKE_INSTALL_PREFIX=../install -DHELICS_BUILD_CXX_SHARED_LIB=ON -DHELICS_BUILD_EXAMPLES=ON -DHELICS_BUILD_TESTS=ON ..

make
make install
```
