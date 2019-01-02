ifeq ($(ARCH), arm)
DIR_GTEST_LIB     = /home/jose/Documents/Utility/googletest/lib/arm
CPPFLAGS += -DRASP
else
DIR_GTEST_LIB     = /home/jose/Documents/Utility/googletest/lib
endif

DIR_GTEST_INCLUDE = /home/jose/Documents/Utility/googletest/googletest/include

INCLUDE = -I. -I$(DIR_GTEST_INCLUDE)

CPPFLAGS += $(INCLUDE)

CXXFLAGS += -pthread

LDFLAGS += -L$(DIR_GTEST_LIB)
LDLIBS += -lgtest -lgtest_main
TARGET_TEST_DIR = ../$(TARGET_DIR)-UnitTest

CPPSRCS := $(wildcard *.cpp)
CPPOBJS := $(patsubst %.cpp, $(TARGET_TEST_DIR)/%.o, $(CPPSRCS))
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
