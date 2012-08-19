CC=gcc
CXX=g++
CFLAGS=-std=c99 -Wall -c -Wc++-compat -O3 -Ilib -Icommon
CXXFLAGS=-Wall -c -O3 -Ilib -Icommon
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
	CC = clang
	CXX = clang++
	SHARED := $(SHARED) $(OBJDIR)/lib/pez/pez.cocoa.o
	LIBS = -framework OpenGL -framework AppKit lib/jsoncpp/libjson_linux-gcc-4.2.1_libmt.a
endif


all: $(OBJDIR) salad tetknot

salad:  $(OBJDIR)/main.o $(SHARED)
	$(CXX) $< $(SHARED) -o salad $(LIBS)

tetknot:  $(OBJDIR)/tetknot.o $(SHARED)
	$(CXX) $< $(SHARED) -o tetknot $(LIBS)

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
	rm -f salad 
	rm -rf $(OBJDIR)
