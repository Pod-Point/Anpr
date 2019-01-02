PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

UNAME := $(shell uname)

ifeq ($(ARCH), arm)
CC  = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++ --sysroot=/home/jose/Documents/Utility/YoctoRPi/sysroot #-std=c++0x -I/lib/ -w  -fmessage-length=0 -Wall
INCLUDE = -I.
CPPFLAGS += -DRASP

LDFLAGS += -Wl,--rpath-link /home/jose/Documents/Utility/YoctoRPi/sysroot/opt/vc/lib/
LDLIBS += -lopenalpr\
	-lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_video -lpthread -lresolv
else
ARCH = x86_64
CC    = gcc
CXX = g++
INCLUDE = -I.
LDLIBS += -lopenalpr\
	-lopencv_imgproc -lopencv_imgcodecs -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_video#-lopencv_ffmpeg# -lliblept170 -lopencv_ffmpeg300 -lopencv_world300
endif

CPPFLAGS += $(INCLUDE)

CXXFLAGS += -std=c++11 -O0 -g3 -Wall 

TARGET_DIR = build/$(ARCH)
TARGET  = PodPointAnpr

CPPSRCS := $(wildcard *.cpp)
CPPOBJS := $(patsubst %.cpp, $(TARGET_DIR)/%.o, $(CPPSRCS))

export ARCH
export CC
export CXX
export CXXFLAGS
export LDFLAGS
export LDLIBS
export TARGET_DIR

.PHONY: all
all:$(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/$(TARGET) : $(COBJS) $(CPPOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TARGET_DIR)/%.o: %.cpp | $(TARGET_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)	
	
.PHONY: test
test:
	cd UnitTest && $(MAKE)	

.PHONY: clean
clean:
	rm -fr $(TARGET_DIR)/*

.PHONY: clean-test
clean-test:
	cd UnitTest && $(MAKE) clean		
