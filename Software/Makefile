PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)

#ifeq ("$(OS)","Windows_NT") 
CC    = gcc# -I/lib/ -w  -fmessage-length=0  -lstdc++ -Wall
CXX = g++# -fmessage-length=0

LDLIBS += -lopenalpr\
	-lopencv_imgproc -lopencv_imgcodecs -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_video #-lopencv_ffmpeg# -lliblept170 -lopencv_ffmpeg300 -lopencv_world300

else
CC    = arm-linux-gnueabihf-gcc # -I/lib/ -w  -fmessage-length=0  -lstdc++ -Wall
CXX = arm-linux-gnueabihf-g++ #-std=c++0x -I/lib/ -w  -fmessage-length=0 -Wall
INCLUDE = \
    -I"C:/SysGCC/Raspberry/arm-linux-gnueabihf/include" \
    -I"C:/SysGCC/Raspberry/arm-linux-gnueabihf/include/c++/4.9" \
    -I"C:/SysGCC/Raspberry/arm-linux-gnueabihf/include/c++/4.9/backward" \
    -I"C:/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include" \
    -I"C:/SysGCC/Raspberry/arm-linux-gnueabihf/include/use/include/arm-linux/gnueabihf" \
    -I"C:/SysGCC/Raspberry/arm-linux-gnueabihf/4.9/include" \
    -I"C:/SysGCC/Raspberry/arm-linux-gnueabihf/4.9/include-fixed"
    
DIR_OPENSSL_INCLUDE = "C:/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/lib/arm-linux-gnueabihf"
DIR_CURL_LIB = "C:/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/lib/arm-linux-gnueabihf"
LDLIBS += -lIDTechSDK -lcrypto -lcurl -lpthread
LDFLAGS += -L. -Wl,-R. '-Wl,-R$$ORIGIN' -Wl,--rpath-link=C:/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/lib/arm-linux-gnueabihf -Wl,--rpath-link=C:/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/lib/arm-linux-gnueabihf
LIB_EXTENSION = so
endif

#OBJS = sw.o
CPPFLAGS := -I$(INCLUDE) -I$(DIR_OPENCV_INCLUDE) -I$(DIR_OPENALPR_INCLUDE)

#ifeq ($(BUILD_MODE),debug)
#	CFLAGS += -O0 -g3 -Wall
#else ifeq ($(BUILD_MODE),run)
#	CFLAGS += -O2
#else
#	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
#endif
CFLAGS += -O0 -g3 -Wall
CXXFLAGS += -std=c++11 -O0 -g3 -Wall #-D_GLIBCXX_USE_CXX11_ABI=0
#LDFLAGS += -L$(DIR_LIB)

TARGET_DIR = build
TARGET  = PodPointAnpr

CPPSRCS := $(wildcard *.cpp)
CPPOBJS := $(patsubst %.cpp, $(TARGET_DIR)/%.o, $(CPPSRCS))

.PHONY: all
all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/$(TARGET) : $(COBJS) $(CPPOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)
#	cp $(DIR_LIB)/*.* $(TARGET_DIR)/
	
#$(TARGET_DIR)/%.o: %.cpp | $(TARGET_DIR)
#	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

#%.o:	$(PROJECT_ROOT)%.c
#	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
$(TARGET_DIR)/%.o: %.cpp | $(TARGET_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(TARGET_DIR):
	mkdir $(TARGET_DIR)	

.PHONY: clean
clean:
	rm -fr $(TARGET_DIR)/*
