DIR_GTEST_INCLUDE = /home/jose/Documents/Utility/googletest/googletest/include
DIR_GTEST_LIB     = /home/jose/Documents/Utility/googletest/lib

INCLUDE = -I. -I$(DIR_GTEST_INCLUDE)

CPPFLAGS = $(INCLUDE)# -I../ -I../$(DIR_IDT_SDK) -I../$(DIR_COMMON) -I$(DIR_GTEST_INCLUDE) -I$(DIR_OPENSSL_INCLUDE) -I$(DIR_CURL_INCLUDE) # -L$(DIR_GTEST_LIB) -L$(DIR_OPENSSL_LIB)

CXXFLAGS += -std=c++11 -O0 -g3 -Wall -pthread

LDFLAGS = -L$(DIR_GTEST_LIB)# -L$(DIR_CURL_LIB)
LDLIBS += -lopenalpr\
	-lopencv_imgproc -lopencv_imgcodecs -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_video\
	-lgtest -lgtest_main
#-llibcrypto -lcrypt32 -lws2_32 -ldnsapi -llibcurl #-lopencv_ffmpeg# -lliblept170 -lopencv_ffmpeg300 -lopencv_world300
#WIN_LIB = ../WindowsLib
#VPATH := ../: ../$(DIR_COMMON)

CPPSRCS := $(wildcard *.cpp)
CPPOBJS := $(CPPSRCS:.cpp=.o)
#CPPOBJS += Utils.o tinyxml2.o CRCCalculator.o Icd00.o Pod.o PodMessage.o PodMessageQueue.o WinSyslog.o PodManager.o Observer.o

TARGET  = Test

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET) $(CPPOBJS)
	
$(TARGET): $(CPPOBJS)
	$(CXX) -o $@ $^ $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)
