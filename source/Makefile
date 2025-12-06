#!/usr/bin/make -f
# Source-level build rules for Integra LV2 skeleton

CC  ?= gcc
CXX ?= g++
AR  ?= ar

# Default to integra if not specified
VARIANT ?= integra

# Set Name and Bundle based on Variant
ifeq ($(VARIANT), pulverize)
    NAME = pulverize
    BUNDLE = pulverize.lv2
    CXXFLAGS += -DPULVERIZE_MODE
else
    NAME = integra
    BUNDLE = integra.lv2
endif

SRC  = integra.cpp
OBJ  = $(SRC:.cpp=.o)
LIB  = $(NAME).so

CXXFLAGS += -Wall -Wextra -fvisibility=hidden -fPIC -std=c++11 -O3
LDFLAGS  += -shared

# Installation settings
INSTALL_PATH ?= /usr/local/lib/lv2

all: $(LIB)

$(LIB): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

install: $(LIB)
	mkdir -p $(DESTDIR)$(INSTALL_PATH)/$(BUNDLE)
	cp $(LIB) $(DESTDIR)$(INSTALL_PATH)/$(BUNDLE)/
	# Copy the correct asset folder based on the bundle name
	cp -r $(BUNDLE)/* $(DESTDIR)$(INSTALL_PATH)/$(BUNDLE)/

clean:
	-rm -f *.o *.so

.PHONY: all clean
