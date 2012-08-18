CC=gcc
CXX=g++
CFLAGS=-std=c99 -Wall -c -Wc++-compat -O3 -Ilib -Icommon
CXXFLAGS=-Wall -c -O3 -Ilib
OBJDIR=build

SHARED := \
	$(OBJDIR)/lib/pez/pez.o \
	$(OBJDIR)/lib/pez/bstrlib.o \
	$(OBJDIR)/lib/lodepng/lodepng.o

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	SHARED := $(SHARED) $(OBJDIR)/lib/pez/pez.linux.o
	LIBS = -lX11 -lGL
endif

ifeq ($(UNAME), Darwin)
	SHARED := $(SHARED) $(OBJDIR)/lib/pez/pez.cocoa.o
	LIBS = -framework OpenGL -framework AppKit
endif


all: $(OBJDIR) salad 

salad:  $(OBJDIR)/main.o $(SHARED)
	$(CXX) $(FLAGS) $(SHARED) $< -o salad 

$(OBJDIR):
	@mkdir -p $@/lib/pez
	@mkdir -p $@/lib/lodepng

$(OBJDIR)/%.o: %.cpp
	@echo $<
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
	@echo $<
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.m
	@echo $<
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)
