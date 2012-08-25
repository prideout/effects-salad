CC=gcc
CXX=g++
CFLAGS=-std=c99 -Wall -c -Wc++-compat -O3 -Ilib -I. -g
CXXFLAGS=-Wall -c -O3 -Ilib -I. -g
OBJDIR=build

SHARED := \
	$(OBJDIR)/common/camera.o \
	$(OBJDIR)/common/demoContext.o \
	$(OBJDIR)/common/tetUtil.o \
	$(OBJDIR)/common/drawable.o \
	$(OBJDIR)/common/effect.o \
	$(OBJDIR)/common/init.o \
	$(OBJDIR)/common/instancer.o \
	$(OBJDIR)/common/light.o \
	$(OBJDIR)/common/programs.o \
	$(OBJDIR)/common/quad.o \
	$(OBJDIR)/common/tube.o \
	$(OBJDIR)/common/surface.o \
	$(OBJDIR)/common/texture.o \
	$(OBJDIR)/common/timer.o \
	$(OBJDIR)/common/vao.o \
	$(OBJDIR)/common/viewport.o \
	$(OBJDIR)/lib/lodepng/lodepng.o \
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


all: $(OBJDIR) salad tetknot

salad:  $(OBJDIR)/main.o $(SHARED)
	$(CXX) $< $(SHARED) -o salad $(LIBS)

tetknot:  $(OBJDIR)/tetknot.o $(SHARED)
	$(CXX) $< $(SHARED) -o tetknot $(LIBS)

$(OBJDIR):
	@mkdir -p $@/common
	@mkdir -p $@/lib/pez
	@mkdir -p $@/lib/lodepng
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

depend:
	makedepend -Y -pbuild/ -- $(CFLAGS) -- *.cpp common/*.cpp fx/*.cpp 


# DO NOT DELETE

build/main.o: lib/pez/pez.h lib/pez/gl3.h common/camera.h lib/glm/glm.hpp
build/main.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/main.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/main.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
build/main.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
build/main.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
build/main.o: lib/glm/./core/type_gentype.hpp lib/glm/./core/type_size.hpp
build/main.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
build/main.o: lib/glm/./core/_swizzle.hpp lib/glm/./core/_swizzle_func.hpp
build/main.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
build/main.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
build/main.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
build/main.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
build/main.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
build/main.o: lib/glm/./core/type_mat2x3.hpp lib/glm/./core/type_mat2x3.inl
build/main.o: lib/glm/./core/type_mat2x4.hpp lib/glm/./core/type_mat2x4.inl
build/main.o: lib/glm/./core/type_mat3x2.hpp lib/glm/./core/type_mat3x2.inl
build/main.o: lib/glm/./core/type_mat3x3.hpp lib/glm/./core/type_mat3x3.inl
build/main.o: lib/glm/./core/type_mat3x4.hpp lib/glm/./core/type_mat3x4.inl
build/main.o: lib/glm/./core/type_mat4x2.hpp lib/glm/./core/type_mat4x2.inl
build/main.o: lib/glm/./core/type_mat4x3.hpp lib/glm/./core/type_mat4x3.inl
build/main.o: lib/glm/./core/type_mat4x4.hpp lib/glm/./core/type_mat4x4.inl
build/main.o: lib/glm/./core/func_trigonometric.hpp
build/main.o: lib/glm/./core/func_trigonometric.inl
build/main.o: lib/glm/./core/_vectorize.hpp
build/main.o: lib/glm/./core/func_exponential.hpp
build/main.o: lib/glm/./core/func_exponential.inl
build/main.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
build/main.o: lib/glm/./core/func_common.inl lib/glm/./core/func_packing.hpp
build/main.o: lib/glm/./core/func_packing.inl
build/main.o: lib/glm/./core/func_geometric.hpp
build/main.o: lib/glm/./core/func_geometric.inl
build/main.o: lib/glm/./core/func_matrix.hpp lib/glm/./core/func_matrix.inl
build/main.o: lib/glm/./core/func_vector_relational.hpp
build/main.o: lib/glm/./core/func_vector_relational.inl
build/main.o: lib/glm/./core/func_integer.hpp lib/glm/./core/func_integer.inl
build/main.o: lib/glm/./core/func_noise.hpp lib/glm/./core/func_noise.inl
build/main.o: lib/glm/./core/_swizzle.hpp lib/glm/gtc/matrix_transform.hpp
build/main.o: lib/glm/glm.hpp lib/glm/gtc/matrix_transform.inl
build/main.o: common/demoContext.h lib/pez/pez.h common/camera.h
build/main.o: common/drawable.h common/init.h common/typedefs.h
build/main.o: lib/jsoncpp/json.h lib/jsoncpp/autolink.h lib/jsoncpp/config.h
build/main.o: lib/jsoncpp/value.h lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
build/main.o: lib/jsoncpp/features.h lib/jsoncpp/writer.h lib/tetgen/tetgen.h
build/main.o: common/programs.h common/surface.h fx/all.h fx/quads.h
build/main.o: common/effect.h common/timer.h common/instancer.h common/vao.h
build/main.o: common/texture.h common/typedefs.h common/quad.h
build/tetknot.o: lib/pez/pez.h lib/pez/gl3.h lib/jsoncpp/json.h
