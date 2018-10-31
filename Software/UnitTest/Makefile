ifeq ($(ARCH), arm)
DIR_GTEST_LIB     = /home/jose/Documents/Utility/googletest/lib/arm
CPPFLAGS += -DRASP
else
DIR_GTEST_LIB     = /home/jose/Documents/Utility/googletest/lib
endif

DIR_GTEST_INCLUDE = /home/jose/Documents/Utility/googletest/googletest/include

INCLUDE = -I. -I$(DIR_GTEST_INCLUDE)

CPPFLAGS += $(INCLUDE)# -I../ -I../$(DIR_IDT_SDK) -I../$(DIR_COMMON) -I$(DIR_GTEST_INCLUDE) -I$(DIR_OPENSSL_INCLUDE) -I$(DIR_CURL_INCLUDE) # -L$(DIR_GTEST_LIB) -L$(DIR_OPENSSL_LIB)

CXXFLAGS += -pthread

LDFLAGS += -L$(DIR_GTEST_LIB)# -L$(DIR_CURL_LIB)
LDLIBS += -lgtest -lgtest_main
#-llibcrypto -lcrypt32 -lws2_32 -ldnsapi -llibcurl
#WIN_LIB = ../WindowsLib
#VPATH := ../: ../$(DIR_COMMON)
TARGET_TEST_DIR = ../$(TARGET_DIR)-UnitTest

CPPSRCS := $(wildcard *.cpp)
#CPPOBJS := $(CPPSRCS:.cpp=.o)
CPPOBJS := $(patsubst %.cpp, $(TARGET_TEST_DIR)/%.o, $(CPPSRCS))
#CPPOBJS += Utils.o tinyxml2.o CRCCalculator.o Icd00.o Pod.o PodMessage.o PodMessageQueue.o WinSyslog.o PodManager.o Observer.o
TARGET = Test

.PHONY: all
all:$(TARGET_TEST_DIR)/$(TARGET)

$(TARGET_TEST_DIR)/$(TARGET): $(CPPOBJS)
	$(CXX) -o $@ $^ $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)	

$(TARGET_TEST_DIR)/%.o: %.cpp | $(TARGET_TEST_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(TARGET_TEST_DIR):
	mkdir -p $(TARGET_TEST_DIR)

.PHONY: clean
clean:
	rm -f $(TARGET_TEST_DIR)/*
