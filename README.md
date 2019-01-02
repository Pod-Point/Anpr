# Anpr
## Raspberry Pi setup
On a Raspberry Pi intatall:
```
sudo apt-get install build-essential chrpath diffstat gawk libncurses5-dev texinfo pkg-config
```

Install OpenCV for 
```
sudo apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt-get install libxvidcore-dev libx264-dev
sudo apt-get install libgtk2.0-dev
sudo apt-get install libatlas-base-dev gfortran
sudo apt-get install libopencv-dev python-opencv
```

Install OpenAlpr
```
sudo apt-get install openalpr
sudo apt-get update && sudo apt-get install -y openalpr openalpr-daemon openalpr-utils libopenalpr-dev
```

Setup camera driver
```
sudo modprobe bcm2835-v4l2
```
add to /etc/modules

Set interface for camera in "Raspiconfig"


## Cross complie instruction for Raspberry PI

on Ubuntu 18.04 install the toolchain for arm
```
sudo apt-get install build-essential
sudo apt-get install g++-arm-linux-gnueabihf
sudo apt-get install gdb-multiarch
```
Set the file system of Raspberry Pi ("sysroot")
in the make file E.g.
```
CXX = arm-linux-gnueabihf-g++ --sysroot=/home/jose/Documents/Utility/YoctoRPi/sysroot
```

## Install gtest
follow the instruction from : https://github.com/google/googletest
and compile for ARM and for x86_64 and set the folder in "UnitTest/Makefile" e.g.
```
ifeq ($(ARCH), arm)
DIR_GTEST_LIB     = /home/jose/Documents/Utility/googletest/lib/arm
CPPFLAGS += -DRASP
else
DIR_GTEST_LIB     = /home/jose/Documents/Utility/googletest/lib
endif
```
### Build Instruction
#### build for x86_64, app only
```
make 
```
and
```
make clean
```

#### build for ARM, app only
```
make ARCH=ARM
```
and
```
make clean ARCH=ARM
```

#### build for x86_64, test only 
```
make test
```
and
```
make clean-test
```

#### build for ARM, test only 
```
make test ARCH=ARM
```
and
```
make clean-test ARCH=ARM
```
