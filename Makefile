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
build/tetknot.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h
build/tetknot.o: lib/jsoncpp/value.h lib/jsoncpp/forwards.h
build/tetknot.o: lib/jsoncpp/reader.h lib/jsoncpp/features.h
build/tetknot.o: lib/jsoncpp/writer.h common/init.h common/typedefs.h
build/tetknot.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp
build/tetknot.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
build/tetknot.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
build/tetknot.o: lib/glm/./core/type_half.hpp lib/glm/./core/type_half.inl
build/tetknot.o: lib/glm/./core/_detail.hpp lib/glm/./core/type_float.hpp
build/tetknot.o: lib/glm/./core/type_int.hpp lib/glm/./core/type_gentype.hpp
build/tetknot.o: lib/glm/./core/type_size.hpp lib/glm/./core/type_vec1.hpp
build/tetknot.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
build/tetknot.o: lib/glm/./core/_swizzle_func.hpp
build/tetknot.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
build/tetknot.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
build/tetknot.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
build/tetknot.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
build/tetknot.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
build/tetknot.o: lib/glm/./core/type_mat2x3.hpp
build/tetknot.o: lib/glm/./core/type_mat2x3.inl
build/tetknot.o: lib/glm/./core/type_mat2x4.hpp
build/tetknot.o: lib/glm/./core/type_mat2x4.inl
build/tetknot.o: lib/glm/./core/type_mat3x2.hpp
build/tetknot.o: lib/glm/./core/type_mat3x2.inl
build/tetknot.o: lib/glm/./core/type_mat3x3.hpp
build/tetknot.o: lib/glm/./core/type_mat3x3.inl
build/tetknot.o: lib/glm/./core/type_mat3x4.hpp
build/tetknot.o: lib/glm/./core/type_mat3x4.inl
build/tetknot.o: lib/glm/./core/type_mat4x2.hpp
build/tetknot.o: lib/glm/./core/type_mat4x2.inl
build/tetknot.o: lib/glm/./core/type_mat4x3.hpp
build/tetknot.o: lib/glm/./core/type_mat4x3.inl
build/tetknot.o: lib/glm/./core/type_mat4x4.hpp
build/tetknot.o: lib/glm/./core/type_mat4x4.inl
build/tetknot.o: lib/glm/./core/func_trigonometric.hpp
build/tetknot.o: lib/glm/./core/func_trigonometric.inl
build/tetknot.o: lib/glm/./core/_vectorize.hpp
build/tetknot.o: lib/glm/./core/func_exponential.hpp
build/tetknot.o: lib/glm/./core/func_exponential.inl
build/tetknot.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
build/tetknot.o: lib/glm/./core/func_common.inl
build/tetknot.o: lib/glm/./core/func_packing.hpp
build/tetknot.o: lib/glm/./core/func_packing.inl
build/tetknot.o: lib/glm/./core/func_geometric.hpp
build/tetknot.o: lib/glm/./core/func_geometric.inl
build/tetknot.o: lib/glm/./core/func_matrix.hpp
build/tetknot.o: lib/glm/./core/func_matrix.inl
build/tetknot.o: lib/glm/./core/func_vector_relational.hpp
build/tetknot.o: lib/glm/./core/func_vector_relational.inl
build/tetknot.o: lib/glm/./core/func_integer.hpp
build/tetknot.o: lib/glm/./core/func_integer.inl
build/tetknot.o: lib/glm/./core/func_noise.hpp lib/glm/./core/func_noise.inl
build/tetknot.o: lib/glm/./core/_swizzle.hpp lib/pez/pez.h
build/tetknot.o: lib/tetgen/tetgen.h common/programs.h common/tube.h
build/tetknot.o: common/drawable.h common/tetUtil.h common/typedefs.h
build/tetknot.o: common/texture.h lib/glm/gtc/matrix_transform.hpp
build/tetknot.o: lib/glm/glm.hpp lib/glm/gtc/matrix_transform.inl
build/tetknot.o: lib/glm/gtc/type_ptr.hpp lib/glm/gtc/half_float.hpp
build/tetknot.o: lib/glm/gtc/half_float.inl lib/glm/gtc/quaternion.hpp
build/tetknot.o: lib/glm/gtc/quaternion.inl lib/glm/gtc/type_ptr.inl
build/common/camera.o: common/camera.h lib/glm/glm.hpp
build/common/camera.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/camera.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/camera.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
build/common/camera.o: lib/glm/./core/type_half.inl
build/common/camera.o: lib/glm/./core/_detail.hpp
build/common/camera.o: lib/glm/./core/type_float.hpp
build/common/camera.o: lib/glm/./core/type_int.hpp
build/common/camera.o: lib/glm/./core/type_gentype.hpp
build/common/camera.o: lib/glm/./core/type_size.hpp
build/common/camera.o: lib/glm/./core/type_vec1.hpp
build/common/camera.o: lib/glm/./core/type_vec.hpp
build/common/camera.o: lib/glm/./core/_swizzle.hpp
build/common/camera.o: lib/glm/./core/_swizzle_func.hpp
build/common/camera.o: lib/glm/./core/type_vec1.inl
build/common/camera.o: lib/glm/./core/type_vec2.hpp
build/common/camera.o: lib/glm/./core/type_vec2.inl
build/common/camera.o: lib/glm/./core/type_vec3.hpp
build/common/camera.o: lib/glm/./core/type_vec3.inl
build/common/camera.o: lib/glm/./core/type_vec4.hpp
build/common/camera.o: lib/glm/./core/type_vec4.inl
build/common/camera.o: lib/glm/./core/type_mat2x2.hpp
build/common/camera.o: lib/glm/./core/type_mat.hpp
build/common/camera.o: lib/glm/./core/type_mat2x2.inl
build/common/camera.o: lib/glm/./core/type_mat2x3.hpp
build/common/camera.o: lib/glm/./core/type_mat2x3.inl
build/common/camera.o: lib/glm/./core/type_mat2x4.hpp
build/common/camera.o: lib/glm/./core/type_mat2x4.inl
build/common/camera.o: lib/glm/./core/type_mat3x2.hpp
build/common/camera.o: lib/glm/./core/type_mat3x2.inl
build/common/camera.o: lib/glm/./core/type_mat3x3.hpp
build/common/camera.o: lib/glm/./core/type_mat3x3.inl
build/common/camera.o: lib/glm/./core/type_mat3x4.hpp
build/common/camera.o: lib/glm/./core/type_mat3x4.inl
build/common/camera.o: lib/glm/./core/type_mat4x2.hpp
build/common/camera.o: lib/glm/./core/type_mat4x2.inl
build/common/camera.o: lib/glm/./core/type_mat4x3.hpp
build/common/camera.o: lib/glm/./core/type_mat4x3.inl
build/common/camera.o: lib/glm/./core/type_mat4x4.hpp
build/common/camera.o: lib/glm/./core/type_mat4x4.inl
build/common/camera.o: lib/glm/./core/func_trigonometric.hpp
build/common/camera.o: lib/glm/./core/func_trigonometric.inl
build/common/camera.o: lib/glm/./core/_vectorize.hpp
build/common/camera.o: lib/glm/./core/func_exponential.hpp
build/common/camera.o: lib/glm/./core/func_exponential.inl
build/common/camera.o: lib/glm/./core/func_common.hpp
build/common/camera.o: lib/glm/./core/_fixes.hpp
build/common/camera.o: lib/glm/./core/func_common.inl
build/common/camera.o: lib/glm/./core/func_packing.hpp
build/common/camera.o: lib/glm/./core/func_packing.inl
build/common/camera.o: lib/glm/./core/func_geometric.hpp
build/common/camera.o: lib/glm/./core/func_geometric.inl
build/common/camera.o: lib/glm/./core/func_matrix.hpp
build/common/camera.o: lib/glm/./core/func_matrix.inl
build/common/camera.o: lib/glm/./core/func_vector_relational.hpp
build/common/camera.o: lib/glm/./core/func_vector_relational.inl
build/common/camera.o: lib/glm/./core/func_integer.hpp
build/common/camera.o: lib/glm/./core/func_integer.inl
build/common/camera.o: lib/glm/./core/func_noise.hpp
build/common/camera.o: lib/glm/./core/func_noise.inl
build/common/camera.o: lib/glm/./core/_swizzle.hpp
build/common/camera.o: lib/glm/gtc/matrix_transform.hpp lib/glm/glm.hpp
build/common/camera.o: lib/glm/gtc/matrix_transform.inl lib/pez/pez.h
build/common/camera.o: lib/pez/gl3.h common/init.h common/typedefs.h
build/common/camera.o: lib/jsoncpp/json.h lib/jsoncpp/autolink.h
build/common/camera.o: lib/jsoncpp/config.h lib/jsoncpp/value.h
build/common/camera.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
build/common/camera.o: lib/jsoncpp/features.h lib/jsoncpp/writer.h
build/common/camera.o: lib/tetgen/tetgen.h
build/common/demoContext.o: common/demoContext.h lib/glm/glm.hpp
build/common/demoContext.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/demoContext.o: lib/glm/./core/_detail.hpp
build/common/demoContext.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
build/common/demoContext.o: lib/glm/./core/type_half.hpp
build/common/demoContext.o: lib/glm/./core/type_half.inl
build/common/demoContext.o: lib/glm/./core/_detail.hpp
build/common/demoContext.o: lib/glm/./core/type_float.hpp
build/common/demoContext.o: lib/glm/./core/type_int.hpp
build/common/demoContext.o: lib/glm/./core/type_gentype.hpp
build/common/demoContext.o: lib/glm/./core/type_size.hpp
build/common/demoContext.o: lib/glm/./core/type_vec1.hpp
build/common/demoContext.o: lib/glm/./core/type_vec.hpp
build/common/demoContext.o: lib/glm/./core/_swizzle.hpp
build/common/demoContext.o: lib/glm/./core/_swizzle_func.hpp
build/common/demoContext.o: lib/glm/./core/type_vec1.inl
build/common/demoContext.o: lib/glm/./core/type_vec2.hpp
build/common/demoContext.o: lib/glm/./core/type_vec2.inl
build/common/demoContext.o: lib/glm/./core/type_vec3.hpp
build/common/demoContext.o: lib/glm/./core/type_vec3.inl
build/common/demoContext.o: lib/glm/./core/type_vec4.hpp
build/common/demoContext.o: lib/glm/./core/type_vec4.inl
build/common/demoContext.o: lib/glm/./core/type_mat2x2.hpp
build/common/demoContext.o: lib/glm/./core/type_mat.hpp
build/common/demoContext.o: lib/glm/./core/type_mat2x2.inl
build/common/demoContext.o: lib/glm/./core/type_mat2x3.hpp
build/common/demoContext.o: lib/glm/./core/type_mat2x3.inl
build/common/demoContext.o: lib/glm/./core/type_mat2x4.hpp
build/common/demoContext.o: lib/glm/./core/type_mat2x4.inl
build/common/demoContext.o: lib/glm/./core/type_mat3x2.hpp
build/common/demoContext.o: lib/glm/./core/type_mat3x2.inl
build/common/demoContext.o: lib/glm/./core/type_mat3x3.hpp
build/common/demoContext.o: lib/glm/./core/type_mat3x3.inl
build/common/demoContext.o: lib/glm/./core/type_mat3x4.hpp
build/common/demoContext.o: lib/glm/./core/type_mat3x4.inl
build/common/demoContext.o: lib/glm/./core/type_mat4x2.hpp
build/common/demoContext.o: lib/glm/./core/type_mat4x2.inl
build/common/demoContext.o: lib/glm/./core/type_mat4x3.hpp
build/common/demoContext.o: lib/glm/./core/type_mat4x3.inl
build/common/demoContext.o: lib/glm/./core/type_mat4x4.hpp
build/common/demoContext.o: lib/glm/./core/type_mat4x4.inl
build/common/demoContext.o: lib/glm/./core/func_trigonometric.hpp
build/common/demoContext.o: lib/glm/./core/func_trigonometric.inl
build/common/demoContext.o: lib/glm/./core/_vectorize.hpp
build/common/demoContext.o: lib/glm/./core/func_exponential.hpp
build/common/demoContext.o: lib/glm/./core/func_exponential.inl
build/common/demoContext.o: lib/glm/./core/func_common.hpp
build/common/demoContext.o: lib/glm/./core/_fixes.hpp
build/common/demoContext.o: lib/glm/./core/func_common.inl
build/common/demoContext.o: lib/glm/./core/func_packing.hpp
build/common/demoContext.o: lib/glm/./core/func_packing.inl
build/common/demoContext.o: lib/glm/./core/func_geometric.hpp
build/common/demoContext.o: lib/glm/./core/func_geometric.inl
build/common/demoContext.o: lib/glm/./core/func_matrix.hpp
build/common/demoContext.o: lib/glm/./core/func_matrix.inl
build/common/demoContext.o: lib/glm/./core/func_vector_relational.hpp
build/common/demoContext.o: lib/glm/./core/func_vector_relational.inl
build/common/demoContext.o: lib/glm/./core/func_integer.hpp
build/common/demoContext.o: lib/glm/./core/func_integer.inl
build/common/demoContext.o: lib/glm/./core/func_noise.hpp
build/common/demoContext.o: lib/glm/./core/func_noise.inl
build/common/demoContext.o: lib/glm/./core/_swizzle.hpp lib/pez/pez.h
build/common/demoContext.o: lib/pez/gl3.h common/camera.h
build/common/demoContext.o: lib/glm/gtc/matrix_transform.hpp lib/glm/glm.hpp
build/common/demoContext.o: lib/glm/gtc/matrix_transform.inl
build/common/demoContext.o: common/drawable.h
build/common/drawable.o: common/drawable.h lib/pez/pez.h lib/pez/gl3.h
build/common/drawable.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp
build/common/drawable.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
build/common/drawable.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
build/common/drawable.o: lib/glm/./core/type_half.hpp
build/common/drawable.o: lib/glm/./core/type_half.inl
build/common/drawable.o: lib/glm/./core/_detail.hpp
build/common/drawable.o: lib/glm/./core/type_float.hpp
build/common/drawable.o: lib/glm/./core/type_int.hpp
build/common/drawable.o: lib/glm/./core/type_gentype.hpp
build/common/drawable.o: lib/glm/./core/type_size.hpp
build/common/drawable.o: lib/glm/./core/type_vec1.hpp
build/common/drawable.o: lib/glm/./core/type_vec.hpp
build/common/drawable.o: lib/glm/./core/_swizzle.hpp
build/common/drawable.o: lib/glm/./core/_swizzle_func.hpp
build/common/drawable.o: lib/glm/./core/type_vec1.inl
build/common/drawable.o: lib/glm/./core/type_vec2.hpp
build/common/drawable.o: lib/glm/./core/type_vec2.inl
build/common/drawable.o: lib/glm/./core/type_vec3.hpp
build/common/drawable.o: lib/glm/./core/type_vec3.inl
build/common/drawable.o: lib/glm/./core/type_vec4.hpp
build/common/drawable.o: lib/glm/./core/type_vec4.inl
build/common/drawable.o: lib/glm/./core/type_mat2x2.hpp
build/common/drawable.o: lib/glm/./core/type_mat.hpp
build/common/drawable.o: lib/glm/./core/type_mat2x2.inl
build/common/drawable.o: lib/glm/./core/type_mat2x3.hpp
build/common/drawable.o: lib/glm/./core/type_mat2x3.inl
build/common/drawable.o: lib/glm/./core/type_mat2x4.hpp
build/common/drawable.o: lib/glm/./core/type_mat2x4.inl
build/common/drawable.o: lib/glm/./core/type_mat3x2.hpp
build/common/drawable.o: lib/glm/./core/type_mat3x2.inl
build/common/drawable.o: lib/glm/./core/type_mat3x3.hpp
build/common/drawable.o: lib/glm/./core/type_mat3x3.inl
build/common/drawable.o: lib/glm/./core/type_mat3x4.hpp
build/common/drawable.o: lib/glm/./core/type_mat3x4.inl
build/common/drawable.o: lib/glm/./core/type_mat4x2.hpp
build/common/drawable.o: lib/glm/./core/type_mat4x2.inl
build/common/drawable.o: lib/glm/./core/type_mat4x3.hpp
build/common/drawable.o: lib/glm/./core/type_mat4x3.inl
build/common/drawable.o: lib/glm/./core/type_mat4x4.hpp
build/common/drawable.o: lib/glm/./core/type_mat4x4.inl
build/common/drawable.o: lib/glm/./core/func_trigonometric.hpp
build/common/drawable.o: lib/glm/./core/func_trigonometric.inl
build/common/drawable.o: lib/glm/./core/_vectorize.hpp
build/common/drawable.o: lib/glm/./core/func_exponential.hpp
build/common/drawable.o: lib/glm/./core/func_exponential.inl
build/common/drawable.o: lib/glm/./core/func_common.hpp
build/common/drawable.o: lib/glm/./core/_fixes.hpp
build/common/drawable.o: lib/glm/./core/func_common.inl
build/common/drawable.o: lib/glm/./core/func_packing.hpp
build/common/drawable.o: lib/glm/./core/func_packing.inl
build/common/drawable.o: lib/glm/./core/func_geometric.hpp
build/common/drawable.o: lib/glm/./core/func_geometric.inl
build/common/drawable.o: lib/glm/./core/func_matrix.hpp
build/common/drawable.o: lib/glm/./core/func_matrix.inl
build/common/drawable.o: lib/glm/./core/func_vector_relational.hpp
build/common/drawable.o: lib/glm/./core/func_vector_relational.inl
build/common/drawable.o: lib/glm/./core/func_integer.hpp
build/common/drawable.o: lib/glm/./core/func_integer.inl
build/common/drawable.o: lib/glm/./core/func_noise.hpp
build/common/drawable.o: lib/glm/./core/func_noise.inl
build/common/drawable.o: lib/glm/./core/_swizzle.hpp common/demoContext.h
build/common/drawable.o: common/camera.h lib/glm/gtc/matrix_transform.hpp
build/common/drawable.o: lib/glm/glm.hpp lib/glm/gtc/matrix_transform.inl
build/common/effect.o: common/effect.h common/drawable.h lib/pez/pez.h
build/common/effect.o: lib/pez/gl3.h lib/glm/glm.hpp lib/glm/core/_fixes.hpp
build/common/effect.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
build/common/effect.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
build/common/effect.o: lib/glm/./core/type_half.hpp
build/common/effect.o: lib/glm/./core/type_half.inl
build/common/effect.o: lib/glm/./core/_detail.hpp
build/common/effect.o: lib/glm/./core/type_float.hpp
build/common/effect.o: lib/glm/./core/type_int.hpp
build/common/effect.o: lib/glm/./core/type_gentype.hpp
build/common/effect.o: lib/glm/./core/type_size.hpp
build/common/effect.o: lib/glm/./core/type_vec1.hpp
build/common/effect.o: lib/glm/./core/type_vec.hpp
build/common/effect.o: lib/glm/./core/_swizzle.hpp
build/common/effect.o: lib/glm/./core/_swizzle_func.hpp
build/common/effect.o: lib/glm/./core/type_vec1.inl
build/common/effect.o: lib/glm/./core/type_vec2.hpp
build/common/effect.o: lib/glm/./core/type_vec2.inl
build/common/effect.o: lib/glm/./core/type_vec3.hpp
build/common/effect.o: lib/glm/./core/type_vec3.inl
build/common/effect.o: lib/glm/./core/type_vec4.hpp
build/common/effect.o: lib/glm/./core/type_vec4.inl
build/common/effect.o: lib/glm/./core/type_mat2x2.hpp
build/common/effect.o: lib/glm/./core/type_mat.hpp
build/common/effect.o: lib/glm/./core/type_mat2x2.inl
build/common/effect.o: lib/glm/./core/type_mat2x3.hpp
build/common/effect.o: lib/glm/./core/type_mat2x3.inl
build/common/effect.o: lib/glm/./core/type_mat2x4.hpp
build/common/effect.o: lib/glm/./core/type_mat2x4.inl
build/common/effect.o: lib/glm/./core/type_mat3x2.hpp
build/common/effect.o: lib/glm/./core/type_mat3x2.inl
build/common/effect.o: lib/glm/./core/type_mat3x3.hpp
build/common/effect.o: lib/glm/./core/type_mat3x3.inl
build/common/effect.o: lib/glm/./core/type_mat3x4.hpp
build/common/effect.o: lib/glm/./core/type_mat3x4.inl
build/common/effect.o: lib/glm/./core/type_mat4x2.hpp
build/common/effect.o: lib/glm/./core/type_mat4x2.inl
build/common/effect.o: lib/glm/./core/type_mat4x3.hpp
build/common/effect.o: lib/glm/./core/type_mat4x3.inl
build/common/effect.o: lib/glm/./core/type_mat4x4.hpp
build/common/effect.o: lib/glm/./core/type_mat4x4.inl
build/common/effect.o: lib/glm/./core/func_trigonometric.hpp
build/common/effect.o: lib/glm/./core/func_trigonometric.inl
build/common/effect.o: lib/glm/./core/_vectorize.hpp
build/common/effect.o: lib/glm/./core/func_exponential.hpp
build/common/effect.o: lib/glm/./core/func_exponential.inl
build/common/effect.o: lib/glm/./core/func_common.hpp
build/common/effect.o: lib/glm/./core/_fixes.hpp
build/common/effect.o: lib/glm/./core/func_common.inl
build/common/effect.o: lib/glm/./core/func_packing.hpp
build/common/effect.o: lib/glm/./core/func_packing.inl
build/common/effect.o: lib/glm/./core/func_geometric.hpp
build/common/effect.o: lib/glm/./core/func_geometric.inl
build/common/effect.o: lib/glm/./core/func_matrix.hpp
build/common/effect.o: lib/glm/./core/func_matrix.inl
build/common/effect.o: lib/glm/./core/func_vector_relational.hpp
build/common/effect.o: lib/glm/./core/func_vector_relational.inl
build/common/effect.o: lib/glm/./core/func_integer.hpp
build/common/effect.o: lib/glm/./core/func_integer.inl
build/common/effect.o: lib/glm/./core/func_noise.hpp
build/common/effect.o: lib/glm/./core/func_noise.inl
build/common/effect.o: lib/glm/./core/_swizzle.hpp common/timer.h
build/common/init.o: common/init.h common/typedefs.h lib/glm/glm.hpp
build/common/init.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/init.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/init.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
build/common/init.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
build/common/init.o: lib/glm/./core/type_float.hpp
build/common/init.o: lib/glm/./core/type_int.hpp
build/common/init.o: lib/glm/./core/type_gentype.hpp
build/common/init.o: lib/glm/./core/type_size.hpp
build/common/init.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
build/common/init.o: lib/glm/./core/_swizzle.hpp
build/common/init.o: lib/glm/./core/_swizzle_func.hpp
build/common/init.o: lib/glm/./core/type_vec1.inl
build/common/init.o: lib/glm/./core/type_vec2.hpp
build/common/init.o: lib/glm/./core/type_vec2.inl
build/common/init.o: lib/glm/./core/type_vec3.hpp
build/common/init.o: lib/glm/./core/type_vec3.inl
build/common/init.o: lib/glm/./core/type_vec4.hpp
build/common/init.o: lib/glm/./core/type_vec4.inl
build/common/init.o: lib/glm/./core/type_mat2x2.hpp
build/common/init.o: lib/glm/./core/type_mat.hpp
build/common/init.o: lib/glm/./core/type_mat2x2.inl
build/common/init.o: lib/glm/./core/type_mat2x3.hpp
build/common/init.o: lib/glm/./core/type_mat2x3.inl
build/common/init.o: lib/glm/./core/type_mat2x4.hpp
build/common/init.o: lib/glm/./core/type_mat2x4.inl
build/common/init.o: lib/glm/./core/type_mat3x2.hpp
build/common/init.o: lib/glm/./core/type_mat3x2.inl
build/common/init.o: lib/glm/./core/type_mat3x3.hpp
build/common/init.o: lib/glm/./core/type_mat3x3.inl
build/common/init.o: lib/glm/./core/type_mat3x4.hpp
build/common/init.o: lib/glm/./core/type_mat3x4.inl
build/common/init.o: lib/glm/./core/type_mat4x2.hpp
build/common/init.o: lib/glm/./core/type_mat4x2.inl
build/common/init.o: lib/glm/./core/type_mat4x3.hpp
build/common/init.o: lib/glm/./core/type_mat4x3.inl
build/common/init.o: lib/glm/./core/type_mat4x4.hpp
build/common/init.o: lib/glm/./core/type_mat4x4.inl
build/common/init.o: lib/glm/./core/func_trigonometric.hpp
build/common/init.o: lib/glm/./core/func_trigonometric.inl
build/common/init.o: lib/glm/./core/_vectorize.hpp
build/common/init.o: lib/glm/./core/func_exponential.hpp
build/common/init.o: lib/glm/./core/func_exponential.inl
build/common/init.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
build/common/init.o: lib/glm/./core/func_common.inl
build/common/init.o: lib/glm/./core/func_packing.hpp
build/common/init.o: lib/glm/./core/func_packing.inl
build/common/init.o: lib/glm/./core/func_geometric.hpp
build/common/init.o: lib/glm/./core/func_geometric.inl
build/common/init.o: lib/glm/./core/func_matrix.hpp
build/common/init.o: lib/glm/./core/func_matrix.inl
build/common/init.o: lib/glm/./core/func_vector_relational.hpp
build/common/init.o: lib/glm/./core/func_vector_relational.inl
build/common/init.o: lib/glm/./core/func_integer.hpp
build/common/init.o: lib/glm/./core/func_integer.inl
build/common/init.o: lib/glm/./core/func_noise.hpp
build/common/init.o: lib/glm/./core/func_noise.inl
build/common/init.o: lib/glm/./core/_swizzle.hpp lib/pez/pez.h lib/pez/gl3.h
build/common/init.o: lib/jsoncpp/json.h lib/jsoncpp/autolink.h
build/common/init.o: lib/jsoncpp/config.h lib/jsoncpp/value.h
build/common/init.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
build/common/init.o: lib/jsoncpp/features.h lib/jsoncpp/writer.h
build/common/init.o: lib/tetgen/tetgen.h
build/common/instancer.o: common/instancer.h lib/pez/pez.h lib/pez/gl3.h
build/common/instancer.o: common/drawable.h lib/glm/glm.hpp
build/common/instancer.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/instancer.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/instancer.o: lib/glm/./core/type.hpp
build/common/instancer.o: lib/glm/./core/type_half.hpp
build/common/instancer.o: lib/glm/./core/type_half.inl
build/common/instancer.o: lib/glm/./core/_detail.hpp
build/common/instancer.o: lib/glm/./core/type_float.hpp
build/common/instancer.o: lib/glm/./core/type_int.hpp
build/common/instancer.o: lib/glm/./core/type_gentype.hpp
build/common/instancer.o: lib/glm/./core/type_size.hpp
build/common/instancer.o: lib/glm/./core/type_vec1.hpp
build/common/instancer.o: lib/glm/./core/type_vec.hpp
build/common/instancer.o: lib/glm/./core/_swizzle.hpp
build/common/instancer.o: lib/glm/./core/_swizzle_func.hpp
build/common/instancer.o: lib/glm/./core/type_vec1.inl
build/common/instancer.o: lib/glm/./core/type_vec2.hpp
build/common/instancer.o: lib/glm/./core/type_vec2.inl
build/common/instancer.o: lib/glm/./core/type_vec3.hpp
build/common/instancer.o: lib/glm/./core/type_vec3.inl
build/common/instancer.o: lib/glm/./core/type_vec4.hpp
build/common/instancer.o: lib/glm/./core/type_vec4.inl
build/common/instancer.o: lib/glm/./core/type_mat2x2.hpp
build/common/instancer.o: lib/glm/./core/type_mat.hpp
build/common/instancer.o: lib/glm/./core/type_mat2x2.inl
build/common/instancer.o: lib/glm/./core/type_mat2x3.hpp
build/common/instancer.o: lib/glm/./core/type_mat2x3.inl
build/common/instancer.o: lib/glm/./core/type_mat2x4.hpp
build/common/instancer.o: lib/glm/./core/type_mat2x4.inl
build/common/instancer.o: lib/glm/./core/type_mat3x2.hpp
build/common/instancer.o: lib/glm/./core/type_mat3x2.inl
build/common/instancer.o: lib/glm/./core/type_mat3x3.hpp
build/common/instancer.o: lib/glm/./core/type_mat3x3.inl
build/common/instancer.o: lib/glm/./core/type_mat3x4.hpp
build/common/instancer.o: lib/glm/./core/type_mat3x4.inl
build/common/instancer.o: lib/glm/./core/type_mat4x2.hpp
build/common/instancer.o: lib/glm/./core/type_mat4x2.inl
build/common/instancer.o: lib/glm/./core/type_mat4x3.hpp
build/common/instancer.o: lib/glm/./core/type_mat4x3.inl
build/common/instancer.o: lib/glm/./core/type_mat4x4.hpp
build/common/instancer.o: lib/glm/./core/type_mat4x4.inl
build/common/instancer.o: lib/glm/./core/func_trigonometric.hpp
build/common/instancer.o: lib/glm/./core/func_trigonometric.inl
build/common/instancer.o: lib/glm/./core/_vectorize.hpp
build/common/instancer.o: lib/glm/./core/func_exponential.hpp
build/common/instancer.o: lib/glm/./core/func_exponential.inl
build/common/instancer.o: lib/glm/./core/func_common.hpp
build/common/instancer.o: lib/glm/./core/_fixes.hpp
build/common/instancer.o: lib/glm/./core/func_common.inl
build/common/instancer.o: lib/glm/./core/func_packing.hpp
build/common/instancer.o: lib/glm/./core/func_packing.inl
build/common/instancer.o: lib/glm/./core/func_geometric.hpp
build/common/instancer.o: lib/glm/./core/func_geometric.inl
build/common/instancer.o: lib/glm/./core/func_matrix.hpp
build/common/instancer.o: lib/glm/./core/func_matrix.inl
build/common/instancer.o: lib/glm/./core/func_vector_relational.hpp
build/common/instancer.o: lib/glm/./core/func_vector_relational.inl
build/common/instancer.o: lib/glm/./core/func_integer.hpp
build/common/instancer.o: lib/glm/./core/func_integer.inl
build/common/instancer.o: lib/glm/./core/func_noise.hpp
build/common/instancer.o: lib/glm/./core/func_noise.inl
build/common/instancer.o: lib/glm/./core/_swizzle.hpp common/vao.h
build/common/instancer.o: common/typedefs.h
build/common/programs.o: common/programs.h lib/pez/pez.h lib/pez/gl3.h
build/common/programs.o: common/init.h common/typedefs.h lib/glm/glm.hpp
build/common/programs.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/programs.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/programs.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
build/common/programs.o: lib/glm/./core/type_half.inl
build/common/programs.o: lib/glm/./core/_detail.hpp
build/common/programs.o: lib/glm/./core/type_float.hpp
build/common/programs.o: lib/glm/./core/type_int.hpp
build/common/programs.o: lib/glm/./core/type_gentype.hpp
build/common/programs.o: lib/glm/./core/type_size.hpp
build/common/programs.o: lib/glm/./core/type_vec1.hpp
build/common/programs.o: lib/glm/./core/type_vec.hpp
build/common/programs.o: lib/glm/./core/_swizzle.hpp
build/common/programs.o: lib/glm/./core/_swizzle_func.hpp
build/common/programs.o: lib/glm/./core/type_vec1.inl
build/common/programs.o: lib/glm/./core/type_vec2.hpp
build/common/programs.o: lib/glm/./core/type_vec2.inl
build/common/programs.o: lib/glm/./core/type_vec3.hpp
build/common/programs.o: lib/glm/./core/type_vec3.inl
build/common/programs.o: lib/glm/./core/type_vec4.hpp
build/common/programs.o: lib/glm/./core/type_vec4.inl
build/common/programs.o: lib/glm/./core/type_mat2x2.hpp
build/common/programs.o: lib/glm/./core/type_mat.hpp
build/common/programs.o: lib/glm/./core/type_mat2x2.inl
build/common/programs.o: lib/glm/./core/type_mat2x3.hpp
build/common/programs.o: lib/glm/./core/type_mat2x3.inl
build/common/programs.o: lib/glm/./core/type_mat2x4.hpp
build/common/programs.o: lib/glm/./core/type_mat2x4.inl
build/common/programs.o: lib/glm/./core/type_mat3x2.hpp
build/common/programs.o: lib/glm/./core/type_mat3x2.inl
build/common/programs.o: lib/glm/./core/type_mat3x3.hpp
build/common/programs.o: lib/glm/./core/type_mat3x3.inl
build/common/programs.o: lib/glm/./core/type_mat3x4.hpp
build/common/programs.o: lib/glm/./core/type_mat3x4.inl
build/common/programs.o: lib/glm/./core/type_mat4x2.hpp
build/common/programs.o: lib/glm/./core/type_mat4x2.inl
build/common/programs.o: lib/glm/./core/type_mat4x3.hpp
build/common/programs.o: lib/glm/./core/type_mat4x3.inl
build/common/programs.o: lib/glm/./core/type_mat4x4.hpp
build/common/programs.o: lib/glm/./core/type_mat4x4.inl
build/common/programs.o: lib/glm/./core/func_trigonometric.hpp
build/common/programs.o: lib/glm/./core/func_trigonometric.inl
build/common/programs.o: lib/glm/./core/_vectorize.hpp
build/common/programs.o: lib/glm/./core/func_exponential.hpp
build/common/programs.o: lib/glm/./core/func_exponential.inl
build/common/programs.o: lib/glm/./core/func_common.hpp
build/common/programs.o: lib/glm/./core/_fixes.hpp
build/common/programs.o: lib/glm/./core/func_common.inl
build/common/programs.o: lib/glm/./core/func_packing.hpp
build/common/programs.o: lib/glm/./core/func_packing.inl
build/common/programs.o: lib/glm/./core/func_geometric.hpp
build/common/programs.o: lib/glm/./core/func_geometric.inl
build/common/programs.o: lib/glm/./core/func_matrix.hpp
build/common/programs.o: lib/glm/./core/func_matrix.inl
build/common/programs.o: lib/glm/./core/func_vector_relational.hpp
build/common/programs.o: lib/glm/./core/func_vector_relational.inl
build/common/programs.o: lib/glm/./core/func_integer.hpp
build/common/programs.o: lib/glm/./core/func_integer.inl
build/common/programs.o: lib/glm/./core/func_noise.hpp
build/common/programs.o: lib/glm/./core/func_noise.inl
build/common/programs.o: lib/glm/./core/_swizzle.hpp lib/jsoncpp/json.h
build/common/programs.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h
build/common/programs.o: lib/jsoncpp/value.h lib/jsoncpp/forwards.h
build/common/programs.o: lib/jsoncpp/reader.h lib/jsoncpp/features.h
build/common/programs.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h
build/common/quad.o: common/quad.h common/drawable.h lib/pez/pez.h
build/common/quad.o: lib/pez/gl3.h lib/glm/glm.hpp lib/glm/core/_fixes.hpp
build/common/quad.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
build/common/quad.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
build/common/quad.o: lib/glm/./core/type_half.hpp
build/common/quad.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
build/common/quad.o: lib/glm/./core/type_float.hpp
build/common/quad.o: lib/glm/./core/type_int.hpp
build/common/quad.o: lib/glm/./core/type_gentype.hpp
build/common/quad.o: lib/glm/./core/type_size.hpp
build/common/quad.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
build/common/quad.o: lib/glm/./core/_swizzle.hpp
build/common/quad.o: lib/glm/./core/_swizzle_func.hpp
build/common/quad.o: lib/glm/./core/type_vec1.inl
build/common/quad.o: lib/glm/./core/type_vec2.hpp
build/common/quad.o: lib/glm/./core/type_vec2.inl
build/common/quad.o: lib/glm/./core/type_vec3.hpp
build/common/quad.o: lib/glm/./core/type_vec3.inl
build/common/quad.o: lib/glm/./core/type_vec4.hpp
build/common/quad.o: lib/glm/./core/type_vec4.inl
build/common/quad.o: lib/glm/./core/type_mat2x2.hpp
build/common/quad.o: lib/glm/./core/type_mat.hpp
build/common/quad.o: lib/glm/./core/type_mat2x2.inl
build/common/quad.o: lib/glm/./core/type_mat2x3.hpp
build/common/quad.o: lib/glm/./core/type_mat2x3.inl
build/common/quad.o: lib/glm/./core/type_mat2x4.hpp
build/common/quad.o: lib/glm/./core/type_mat2x4.inl
build/common/quad.o: lib/glm/./core/type_mat3x2.hpp
build/common/quad.o: lib/glm/./core/type_mat3x2.inl
build/common/quad.o: lib/glm/./core/type_mat3x3.hpp
build/common/quad.o: lib/glm/./core/type_mat3x3.inl
build/common/quad.o: lib/glm/./core/type_mat3x4.hpp
build/common/quad.o: lib/glm/./core/type_mat3x4.inl
build/common/quad.o: lib/glm/./core/type_mat4x2.hpp
build/common/quad.o: lib/glm/./core/type_mat4x2.inl
build/common/quad.o: lib/glm/./core/type_mat4x3.hpp
build/common/quad.o: lib/glm/./core/type_mat4x3.inl
build/common/quad.o: lib/glm/./core/type_mat4x4.hpp
build/common/quad.o: lib/glm/./core/type_mat4x4.inl
build/common/quad.o: lib/glm/./core/func_trigonometric.hpp
build/common/quad.o: lib/glm/./core/func_trigonometric.inl
build/common/quad.o: lib/glm/./core/_vectorize.hpp
build/common/quad.o: lib/glm/./core/func_exponential.hpp
build/common/quad.o: lib/glm/./core/func_exponential.inl
build/common/quad.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
build/common/quad.o: lib/glm/./core/func_common.inl
build/common/quad.o: lib/glm/./core/func_packing.hpp
build/common/quad.o: lib/glm/./core/func_packing.inl
build/common/quad.o: lib/glm/./core/func_geometric.hpp
build/common/quad.o: lib/glm/./core/func_geometric.inl
build/common/quad.o: lib/glm/./core/func_matrix.hpp
build/common/quad.o: lib/glm/./core/func_matrix.inl
build/common/quad.o: lib/glm/./core/func_vector_relational.hpp
build/common/quad.o: lib/glm/./core/func_vector_relational.inl
build/common/quad.o: lib/glm/./core/func_integer.hpp
build/common/quad.o: lib/glm/./core/func_integer.inl
build/common/quad.o: lib/glm/./core/func_noise.hpp
build/common/quad.o: lib/glm/./core/func_noise.inl
build/common/quad.o: lib/glm/./core/_swizzle.hpp common/vao.h
build/common/quad.o: common/typedefs.h
build/common/tetUtil.o: common/tetUtil.h common/typedefs.h lib/glm/glm.hpp
build/common/tetUtil.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/tetUtil.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/tetUtil.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
build/common/tetUtil.o: lib/glm/./core/type_half.inl
build/common/tetUtil.o: lib/glm/./core/_detail.hpp
build/common/tetUtil.o: lib/glm/./core/type_float.hpp
build/common/tetUtil.o: lib/glm/./core/type_int.hpp
build/common/tetUtil.o: lib/glm/./core/type_gentype.hpp
build/common/tetUtil.o: lib/glm/./core/type_size.hpp
build/common/tetUtil.o: lib/glm/./core/type_vec1.hpp
build/common/tetUtil.o: lib/glm/./core/type_vec.hpp
build/common/tetUtil.o: lib/glm/./core/_swizzle.hpp
build/common/tetUtil.o: lib/glm/./core/_swizzle_func.hpp
build/common/tetUtil.o: lib/glm/./core/type_vec1.inl
build/common/tetUtil.o: lib/glm/./core/type_vec2.hpp
build/common/tetUtil.o: lib/glm/./core/type_vec2.inl
build/common/tetUtil.o: lib/glm/./core/type_vec3.hpp
build/common/tetUtil.o: lib/glm/./core/type_vec3.inl
build/common/tetUtil.o: lib/glm/./core/type_vec4.hpp
build/common/tetUtil.o: lib/glm/./core/type_vec4.inl
build/common/tetUtil.o: lib/glm/./core/type_mat2x2.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat2x2.inl
build/common/tetUtil.o: lib/glm/./core/type_mat2x3.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat2x3.inl
build/common/tetUtil.o: lib/glm/./core/type_mat2x4.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat2x4.inl
build/common/tetUtil.o: lib/glm/./core/type_mat3x2.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat3x2.inl
build/common/tetUtil.o: lib/glm/./core/type_mat3x3.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat3x3.inl
build/common/tetUtil.o: lib/glm/./core/type_mat3x4.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat3x4.inl
build/common/tetUtil.o: lib/glm/./core/type_mat4x2.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat4x2.inl
build/common/tetUtil.o: lib/glm/./core/type_mat4x3.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat4x3.inl
build/common/tetUtil.o: lib/glm/./core/type_mat4x4.hpp
build/common/tetUtil.o: lib/glm/./core/type_mat4x4.inl
build/common/tetUtil.o: lib/glm/./core/func_trigonometric.hpp
build/common/tetUtil.o: lib/glm/./core/func_trigonometric.inl
build/common/tetUtil.o: lib/glm/./core/_vectorize.hpp
build/common/tetUtil.o: lib/glm/./core/func_exponential.hpp
build/common/tetUtil.o: lib/glm/./core/func_exponential.inl
build/common/tetUtil.o: lib/glm/./core/func_common.hpp
build/common/tetUtil.o: lib/glm/./core/_fixes.hpp
build/common/tetUtil.o: lib/glm/./core/func_common.inl
build/common/tetUtil.o: lib/glm/./core/func_packing.hpp
build/common/tetUtil.o: lib/glm/./core/func_packing.inl
build/common/tetUtil.o: lib/glm/./core/func_geometric.hpp
build/common/tetUtil.o: lib/glm/./core/func_geometric.inl
build/common/tetUtil.o: lib/glm/./core/func_matrix.hpp
build/common/tetUtil.o: lib/glm/./core/func_matrix.inl
build/common/tetUtil.o: lib/glm/./core/func_vector_relational.hpp
build/common/tetUtil.o: lib/glm/./core/func_vector_relational.inl
build/common/tetUtil.o: lib/glm/./core/func_integer.hpp
build/common/tetUtil.o: lib/glm/./core/func_integer.inl
build/common/tetUtil.o: lib/glm/./core/func_noise.hpp
build/common/tetUtil.o: lib/glm/./core/func_noise.inl
build/common/tetUtil.o: lib/glm/./core/_swizzle.hpp lib/tetgen/tetgen.h
build/common/tetUtil.o: lib/glm/gtx/constants.inl
build/common/texture.o: common/texture.h lib/pez/pez.h lib/pez/gl3.h
build/common/texture.o: common/typedefs.h lib/glm/glm.hpp
build/common/texture.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/texture.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/texture.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
build/common/texture.o: lib/glm/./core/type_half.inl
build/common/texture.o: lib/glm/./core/_detail.hpp
build/common/texture.o: lib/glm/./core/type_float.hpp
build/common/texture.o: lib/glm/./core/type_int.hpp
build/common/texture.o: lib/glm/./core/type_gentype.hpp
build/common/texture.o: lib/glm/./core/type_size.hpp
build/common/texture.o: lib/glm/./core/type_vec1.hpp
build/common/texture.o: lib/glm/./core/type_vec.hpp
build/common/texture.o: lib/glm/./core/_swizzle.hpp
build/common/texture.o: lib/glm/./core/_swizzle_func.hpp
build/common/texture.o: lib/glm/./core/type_vec1.inl
build/common/texture.o: lib/glm/./core/type_vec2.hpp
build/common/texture.o: lib/glm/./core/type_vec2.inl
build/common/texture.o: lib/glm/./core/type_vec3.hpp
build/common/texture.o: lib/glm/./core/type_vec3.inl
build/common/texture.o: lib/glm/./core/type_vec4.hpp
build/common/texture.o: lib/glm/./core/type_vec4.inl
build/common/texture.o: lib/glm/./core/type_mat2x2.hpp
build/common/texture.o: lib/glm/./core/type_mat.hpp
build/common/texture.o: lib/glm/./core/type_mat2x2.inl
build/common/texture.o: lib/glm/./core/type_mat2x3.hpp
build/common/texture.o: lib/glm/./core/type_mat2x3.inl
build/common/texture.o: lib/glm/./core/type_mat2x4.hpp
build/common/texture.o: lib/glm/./core/type_mat2x4.inl
build/common/texture.o: lib/glm/./core/type_mat3x2.hpp
build/common/texture.o: lib/glm/./core/type_mat3x2.inl
build/common/texture.o: lib/glm/./core/type_mat3x3.hpp
build/common/texture.o: lib/glm/./core/type_mat3x3.inl
build/common/texture.o: lib/glm/./core/type_mat3x4.hpp
build/common/texture.o: lib/glm/./core/type_mat3x4.inl
build/common/texture.o: lib/glm/./core/type_mat4x2.hpp
build/common/texture.o: lib/glm/./core/type_mat4x2.inl
build/common/texture.o: lib/glm/./core/type_mat4x3.hpp
build/common/texture.o: lib/glm/./core/type_mat4x3.inl
build/common/texture.o: lib/glm/./core/type_mat4x4.hpp
build/common/texture.o: lib/glm/./core/type_mat4x4.inl
build/common/texture.o: lib/glm/./core/func_trigonometric.hpp
build/common/texture.o: lib/glm/./core/func_trigonometric.inl
build/common/texture.o: lib/glm/./core/_vectorize.hpp
build/common/texture.o: lib/glm/./core/func_exponential.hpp
build/common/texture.o: lib/glm/./core/func_exponential.inl
build/common/texture.o: lib/glm/./core/func_common.hpp
build/common/texture.o: lib/glm/./core/_fixes.hpp
build/common/texture.o: lib/glm/./core/func_common.inl
build/common/texture.o: lib/glm/./core/func_packing.hpp
build/common/texture.o: lib/glm/./core/func_packing.inl
build/common/texture.o: lib/glm/./core/func_geometric.hpp
build/common/texture.o: lib/glm/./core/func_geometric.inl
build/common/texture.o: lib/glm/./core/func_matrix.hpp
build/common/texture.o: lib/glm/./core/func_matrix.inl
build/common/texture.o: lib/glm/./core/func_vector_relational.hpp
build/common/texture.o: lib/glm/./core/func_vector_relational.inl
build/common/texture.o: lib/glm/./core/func_integer.hpp
build/common/texture.o: lib/glm/./core/func_integer.inl
build/common/texture.o: lib/glm/./core/func_noise.hpp
build/common/texture.o: lib/glm/./core/func_noise.inl
build/common/texture.o: lib/glm/./core/_swizzle.hpp
build/common/timer.o: common/timer.h common/drawable.h lib/pez/pez.h
build/common/timer.o: lib/pez/gl3.h lib/glm/glm.hpp lib/glm/core/_fixes.hpp
build/common/timer.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
build/common/timer.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
build/common/timer.o: lib/glm/./core/type_half.hpp
build/common/timer.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
build/common/timer.o: lib/glm/./core/type_float.hpp
build/common/timer.o: lib/glm/./core/type_int.hpp
build/common/timer.o: lib/glm/./core/type_gentype.hpp
build/common/timer.o: lib/glm/./core/type_size.hpp
build/common/timer.o: lib/glm/./core/type_vec1.hpp
build/common/timer.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
build/common/timer.o: lib/glm/./core/_swizzle_func.hpp
build/common/timer.o: lib/glm/./core/type_vec1.inl
build/common/timer.o: lib/glm/./core/type_vec2.hpp
build/common/timer.o: lib/glm/./core/type_vec2.inl
build/common/timer.o: lib/glm/./core/type_vec3.hpp
build/common/timer.o: lib/glm/./core/type_vec3.inl
build/common/timer.o: lib/glm/./core/type_vec4.hpp
build/common/timer.o: lib/glm/./core/type_vec4.inl
build/common/timer.o: lib/glm/./core/type_mat2x2.hpp
build/common/timer.o: lib/glm/./core/type_mat.hpp
build/common/timer.o: lib/glm/./core/type_mat2x2.inl
build/common/timer.o: lib/glm/./core/type_mat2x3.hpp
build/common/timer.o: lib/glm/./core/type_mat2x3.inl
build/common/timer.o: lib/glm/./core/type_mat2x4.hpp
build/common/timer.o: lib/glm/./core/type_mat2x4.inl
build/common/timer.o: lib/glm/./core/type_mat3x2.hpp
build/common/timer.o: lib/glm/./core/type_mat3x2.inl
build/common/timer.o: lib/glm/./core/type_mat3x3.hpp
build/common/timer.o: lib/glm/./core/type_mat3x3.inl
build/common/timer.o: lib/glm/./core/type_mat3x4.hpp
build/common/timer.o: lib/glm/./core/type_mat3x4.inl
build/common/timer.o: lib/glm/./core/type_mat4x2.hpp
build/common/timer.o: lib/glm/./core/type_mat4x2.inl
build/common/timer.o: lib/glm/./core/type_mat4x3.hpp
build/common/timer.o: lib/glm/./core/type_mat4x3.inl
build/common/timer.o: lib/glm/./core/type_mat4x4.hpp
build/common/timer.o: lib/glm/./core/type_mat4x4.inl
build/common/timer.o: lib/glm/./core/func_trigonometric.hpp
build/common/timer.o: lib/glm/./core/func_trigonometric.inl
build/common/timer.o: lib/glm/./core/_vectorize.hpp
build/common/timer.o: lib/glm/./core/func_exponential.hpp
build/common/timer.o: lib/glm/./core/func_exponential.inl
build/common/timer.o: lib/glm/./core/func_common.hpp
build/common/timer.o: lib/glm/./core/_fixes.hpp
build/common/timer.o: lib/glm/./core/func_common.inl
build/common/timer.o: lib/glm/./core/func_packing.hpp
build/common/timer.o: lib/glm/./core/func_packing.inl
build/common/timer.o: lib/glm/./core/func_geometric.hpp
build/common/timer.o: lib/glm/./core/func_geometric.inl
build/common/timer.o: lib/glm/./core/func_matrix.hpp
build/common/timer.o: lib/glm/./core/func_matrix.inl
build/common/timer.o: lib/glm/./core/func_vector_relational.hpp
build/common/timer.o: lib/glm/./core/func_vector_relational.inl
build/common/timer.o: lib/glm/./core/func_integer.hpp
build/common/timer.o: lib/glm/./core/func_integer.inl
build/common/timer.o: lib/glm/./core/func_noise.hpp
build/common/timer.o: lib/glm/./core/func_noise.inl
build/common/timer.o: lib/glm/./core/_swizzle.hpp
build/common/tube.o: common/tube.h common/drawable.h lib/pez/pez.h
build/common/tube.o: lib/pez/gl3.h lib/glm/glm.hpp lib/glm/core/_fixes.hpp
build/common/tube.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
build/common/tube.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
build/common/tube.o: lib/glm/./core/type_half.hpp
build/common/tube.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
build/common/tube.o: lib/glm/./core/type_float.hpp
build/common/tube.o: lib/glm/./core/type_int.hpp
build/common/tube.o: lib/glm/./core/type_gentype.hpp
build/common/tube.o: lib/glm/./core/type_size.hpp
build/common/tube.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
build/common/tube.o: lib/glm/./core/_swizzle.hpp
build/common/tube.o: lib/glm/./core/_swizzle_func.hpp
build/common/tube.o: lib/glm/./core/type_vec1.inl
build/common/tube.o: lib/glm/./core/type_vec2.hpp
build/common/tube.o: lib/glm/./core/type_vec2.inl
build/common/tube.o: lib/glm/./core/type_vec3.hpp
build/common/tube.o: lib/glm/./core/type_vec3.inl
build/common/tube.o: lib/glm/./core/type_vec4.hpp
build/common/tube.o: lib/glm/./core/type_vec4.inl
build/common/tube.o: lib/glm/./core/type_mat2x2.hpp
build/common/tube.o: lib/glm/./core/type_mat.hpp
build/common/tube.o: lib/glm/./core/type_mat2x2.inl
build/common/tube.o: lib/glm/./core/type_mat2x3.hpp
build/common/tube.o: lib/glm/./core/type_mat2x3.inl
build/common/tube.o: lib/glm/./core/type_mat2x4.hpp
build/common/tube.o: lib/glm/./core/type_mat2x4.inl
build/common/tube.o: lib/glm/./core/type_mat3x2.hpp
build/common/tube.o: lib/glm/./core/type_mat3x2.inl
build/common/tube.o: lib/glm/./core/type_mat3x3.hpp
build/common/tube.o: lib/glm/./core/type_mat3x3.inl
build/common/tube.o: lib/glm/./core/type_mat3x4.hpp
build/common/tube.o: lib/glm/./core/type_mat3x4.inl
build/common/tube.o: lib/glm/./core/type_mat4x2.hpp
build/common/tube.o: lib/glm/./core/type_mat4x2.inl
build/common/tube.o: lib/glm/./core/type_mat4x3.hpp
build/common/tube.o: lib/glm/./core/type_mat4x3.inl
build/common/tube.o: lib/glm/./core/type_mat4x4.hpp
build/common/tube.o: lib/glm/./core/type_mat4x4.inl
build/common/tube.o: lib/glm/./core/func_trigonometric.hpp
build/common/tube.o: lib/glm/./core/func_trigonometric.inl
build/common/tube.o: lib/glm/./core/_vectorize.hpp
build/common/tube.o: lib/glm/./core/func_exponential.hpp
build/common/tube.o: lib/glm/./core/func_exponential.inl
build/common/tube.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
build/common/tube.o: lib/glm/./core/func_common.inl
build/common/tube.o: lib/glm/./core/func_packing.hpp
build/common/tube.o: lib/glm/./core/func_packing.inl
build/common/tube.o: lib/glm/./core/func_geometric.hpp
build/common/tube.o: lib/glm/./core/func_geometric.inl
build/common/tube.o: lib/glm/./core/func_matrix.hpp
build/common/tube.o: lib/glm/./core/func_matrix.inl
build/common/tube.o: lib/glm/./core/func_vector_relational.hpp
build/common/tube.o: lib/glm/./core/func_vector_relational.inl
build/common/tube.o: lib/glm/./core/func_integer.hpp
build/common/tube.o: lib/glm/./core/func_integer.inl
build/common/tube.o: lib/glm/./core/func_noise.hpp
build/common/tube.o: lib/glm/./core/func_noise.inl
build/common/tube.o: lib/glm/./core/_swizzle.hpp common/typedefs.h
build/common/vao.o: common/vao.h lib/glm/glm.hpp lib/glm/core/_fixes.hpp
build/common/vao.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
build/common/vao.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
build/common/vao.o: lib/glm/./core/type_half.hpp lib/glm/./core/type_half.inl
build/common/vao.o: lib/glm/./core/_detail.hpp lib/glm/./core/type_float.hpp
build/common/vao.o: lib/glm/./core/type_int.hpp
build/common/vao.o: lib/glm/./core/type_gentype.hpp
build/common/vao.o: lib/glm/./core/type_size.hpp lib/glm/./core/type_vec1.hpp
build/common/vao.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
build/common/vao.o: lib/glm/./core/_swizzle_func.hpp
build/common/vao.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
build/common/vao.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
build/common/vao.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
build/common/vao.o: lib/glm/./core/type_vec4.inl
build/common/vao.o: lib/glm/./core/type_mat2x2.hpp
build/common/vao.o: lib/glm/./core/type_mat.hpp
build/common/vao.o: lib/glm/./core/type_mat2x2.inl
build/common/vao.o: lib/glm/./core/type_mat2x3.hpp
build/common/vao.o: lib/glm/./core/type_mat2x3.inl
build/common/vao.o: lib/glm/./core/type_mat2x4.hpp
build/common/vao.o: lib/glm/./core/type_mat2x4.inl
build/common/vao.o: lib/glm/./core/type_mat3x2.hpp
build/common/vao.o: lib/glm/./core/type_mat3x2.inl
build/common/vao.o: lib/glm/./core/type_mat3x3.hpp
build/common/vao.o: lib/glm/./core/type_mat3x3.inl
build/common/vao.o: lib/glm/./core/type_mat3x4.hpp
build/common/vao.o: lib/glm/./core/type_mat3x4.inl
build/common/vao.o: lib/glm/./core/type_mat4x2.hpp
build/common/vao.o: lib/glm/./core/type_mat4x2.inl
build/common/vao.o: lib/glm/./core/type_mat4x3.hpp
build/common/vao.o: lib/glm/./core/type_mat4x3.inl
build/common/vao.o: lib/glm/./core/type_mat4x4.hpp
build/common/vao.o: lib/glm/./core/type_mat4x4.inl
build/common/vao.o: lib/glm/./core/func_trigonometric.hpp
build/common/vao.o: lib/glm/./core/func_trigonometric.inl
build/common/vao.o: lib/glm/./core/_vectorize.hpp
build/common/vao.o: lib/glm/./core/func_exponential.hpp
build/common/vao.o: lib/glm/./core/func_exponential.inl
build/common/vao.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
build/common/vao.o: lib/glm/./core/func_common.inl
build/common/vao.o: lib/glm/./core/func_packing.hpp
build/common/vao.o: lib/glm/./core/func_packing.inl
build/common/vao.o: lib/glm/./core/func_geometric.hpp
build/common/vao.o: lib/glm/./core/func_geometric.inl
build/common/vao.o: lib/glm/./core/func_matrix.hpp
build/common/vao.o: lib/glm/./core/func_matrix.inl
build/common/vao.o: lib/glm/./core/func_vector_relational.hpp
build/common/vao.o: lib/glm/./core/func_vector_relational.inl
build/common/vao.o: lib/glm/./core/func_integer.hpp
build/common/vao.o: lib/glm/./core/func_integer.inl
build/common/vao.o: lib/glm/./core/func_noise.hpp
build/common/vao.o: lib/glm/./core/func_noise.inl lib/glm/./core/_swizzle.hpp
build/common/vao.o: lib/pez/pez.h lib/pez/gl3.h common/typedefs.h
build/common/vao.o: common/init.h lib/jsoncpp/json.h lib/jsoncpp/autolink.h
build/common/vao.o: lib/jsoncpp/config.h lib/jsoncpp/value.h
build/common/vao.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
build/common/vao.o: lib/jsoncpp/features.h lib/jsoncpp/writer.h
build/common/vao.o: lib/tetgen/tetgen.h
