CC=gcc
CXX=g++
CFLAGS=-std=c99 -Wall -c -Wc++-compat -O3 -Ilib -I. -g
CXXFLAGS=-Wall -c -O3 -Ilib -I. -g
OBJDIR=build

SHARED := \
	$(OBJDIR)/common/camera.o \
	$(OBJDIR)/common/cameraPath.o \
	$(OBJDIR)/common/curve.o \
	$(OBJDIR)/common/demoContext.o \
	$(OBJDIR)/common/tetUtil.o \
	$(OBJDIR)/common/drawable.o \
	$(OBJDIR)/common/effect.o \
	$(OBJDIR)/common/init.o \
	$(OBJDIR)/common/instancer.o \
	$(OBJDIR)/common/light.o \
	$(OBJDIR)/common/normalField.o \
	$(OBJDIR)/common/programs.o \
	$(OBJDIR)/common/quad.o \
	$(OBJDIR)/common/tube.o \
	$(OBJDIR)/common/surface.o \
	$(OBJDIR)/common/texture.o \
	$(OBJDIR)/common/timer.o \
	$(OBJDIR)/common/vao.o \
	$(OBJDIR)/common/viewport.o \
	$(OBJDIR)/fx/fullscreen.o \
	$(OBJDIR)/fx/ground.o \
	$(OBJDIR)/fx/background.o \
	$(OBJDIR)/fx/fpsOverlay.o \
	$(OBJDIR)/fx/fireFlies.o \
	$(OBJDIR)/fx/buildings.o \
	$(OBJDIR)/fx/portal.o \
	$(OBJDIR)/fx/quads.o \
	$(OBJDIR)/lib/lodepng/lodepng.o \
	$(OBJDIR)/lib/noise/perlin.o \
	$(OBJDIR)/lib/pez/bstrlib.o \
	$(OBJDIR)/lib/pez/pez.o

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	SHARED := $(SHARED) $(OBJDIR)/lib/pez/pez.linux.o
	LIBS = -lX11 -lGL lib/jsoncpp/libjson_linux-gcc-4.4.6_libmt.a lib/tetgen/libtet.a
endif

ifeq ($(UNAME), Darwin)
	CC = clang
	CXX = clang++
	SHARED := $(SHARED) $(OBJDIR)/lib/pez/pez.cocoa.o
	LIBS = -framework OpenGL -framework AppKit lib/jsoncpp/libjson_linux-gcc-4.2.1_libmt.a lib/tetgen/libtet.a
endif


all: $(OBJDIR) $(OBJDIR)/make.deps salad tetknot 

salad:  $(OBJDIR)/main.o $(SHARED)
	$(CXX) $< $(SHARED) -o salad $(LIBS)

tetknot:  $(OBJDIR)/tetknot.o $(SHARED)
	$(CXX) $< $(SHARED) -o tetknot $(LIBS)

$(OBJDIR): 
	@mkdir -p $@
	@mkdir -p $@/common
	@mkdir -p $@/fx
	@mkdir -p $@/lib/pez
	@mkdir -p $@/lib/lodepng
	@mkdir -p $@/lib/noise
	@mkdir -p $@/lib/tetgen

$(OBJDIR)/%.o: %.cpp
	@echo $<
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.cxx
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
	rm -f tetknot
	rm -rf $(OBJDIR)

$(OBJDIR)/make.deps: $(OBJDIR)
	touch $(OBJDIR)/make.deps
	makedepend -f$(OBJDIR)/make.deps -pbuild/ -- $(CFLAGS) -- *.cpp common/*.cpp fx/*.cpp >& /dev/null

-include $(OBJDIR)/make.deps
