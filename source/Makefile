#!/usr/bin/make -f
# Source-level build rules for Integra LV2 skeleton

CC  ?= gcc
CXX ?= g++
AR  ?= ar

NAME = integra
SRC  = integra.cpp
OBJ  = $(SRC:.cpp=.o)
LIB  = $(NAME).so

CXXFLAGS += -Wall -Wextra -fvisibility=hidden -fPIC -std=c++11
LDFLAGS  += -shared

# Installation settings
INSTALL_PATH ?= /usr/local/lib/lv2
BUNDLE = integra.lv2

all: $(LIB)

$(LIB): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

install: $(LIB)
	mkdir -p $(DESTDIR)$(INSTALL_PATH)/$(BUNDLE)
	cp $(LIB) $(DESTDIR)$(INSTALL_PATH)/$(BUNDLE)/
	cp -r $(BUNDLE)/* $(DESTDIR)$(INSTALL_PATH)/$(BUNDLE)/

clean:
	-rm -f $(OBJ) $(LIB)

.PHONY: all clean
