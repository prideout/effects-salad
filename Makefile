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
	$(OBJDIR)/common/programs.o \
	$(OBJDIR)/common/quad.o \
	$(OBJDIR)/common/tube.o \
	$(OBJDIR)/common/surface.o \
	$(OBJDIR)/common/texture.o \
	$(OBJDIR)/common/timer.o \
	$(OBJDIR)/common/vao.o \
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
	makedepend -pbuild/ -- $(CFLAGS) -- *.cpp common/*.cpp fx/*.cpp 

# DO NOT DELETE

build/main.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/inttypes.h
build/main.o: /usr/include/features.h /usr/include/sys/cdefs.h
build/main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
build/main.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/main.o: /usr/include/bits/wchar.h common/camera.h lib/glm/glm.hpp
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
build/main.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h /usr/include/stdio.h
build/main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
build/main.o: /usr/include/libio.h /usr/include/_G_config.h
build/main.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
build/main.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
build/main.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
build/main.o: /usr/include/endian.h /usr/include/bits/endian.h
build/main.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
build/main.o: /usr/include/time.h /usr/include/sys/select.h
build/main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
build/main.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
build/main.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
build/main.o: /usr/include/string.h /usr/include/xlocale.h
build/main.o: /usr/include/math.h /usr/include/bits/huge_val.h
build/main.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
build/main.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
build/main.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
build/main.o: /usr/include/assert.h common/programs.h common/surface.h
build/main.o: fx/all.h fx/quads.h common/effect.h common/instancer.h
build/main.o: common/vao.h common/texture.h common/quad.h
build/tetknot.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/inttypes.h
build/tetknot.o: /usr/include/features.h /usr/include/sys/cdefs.h
build/tetknot.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
build/tetknot.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/tetknot.o: /usr/include/bits/wchar.h lib/jsoncpp/json.h
build/tetknot.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h
build/tetknot.o: lib/jsoncpp/value.h lib/jsoncpp/forwards.h
build/tetknot.o: lib/jsoncpp/reader.h lib/jsoncpp/writer.h
build/tetknot.o: lib/tetgen/tetgen.h /usr/include/stdio.h
build/tetknot.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
build/tetknot.o: /usr/include/libio.h /usr/include/_G_config.h
build/tetknot.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
build/tetknot.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
build/tetknot.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
build/tetknot.o: /usr/include/endian.h /usr/include/bits/endian.h
build/tetknot.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
build/tetknot.o: /usr/include/time.h /usr/include/sys/select.h
build/tetknot.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
build/tetknot.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
build/tetknot.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
build/tetknot.o: /usr/include/string.h /usr/include/xlocale.h
build/tetknot.o: /usr/include/math.h /usr/include/bits/huge_val.h
build/tetknot.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
build/tetknot.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
build/tetknot.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
build/tetknot.o: /usr/include/assert.h common/init.h common/typedefs.h
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
build/tetknot.o: lib/glm/./core/_swizzle.hpp lib/pez/pez.h common/programs.h
build/tetknot.o: common/tube.h common/drawable.h
build/tetknot.o: lib/glm/gtc/matrix_transform.hpp lib/glm/glm.hpp
build/tetknot.o: lib/glm/gtc/matrix_transform.inl lib/glm/gtx/constants.inl
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
build/common/camera.o: lib/pez/gl3.h /usr/include/inttypes.h
build/common/camera.o: /usr/include/features.h /usr/include/sys/cdefs.h
build/common/camera.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
build/common/camera.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/common/camera.o: /usr/include/bits/wchar.h common/init.h
build/common/camera.o: common/typedefs.h lib/jsoncpp/json.h
build/common/camera.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h
build/common/camera.o: lib/jsoncpp/value.h lib/jsoncpp/forwards.h
build/common/camera.o: lib/jsoncpp/reader.h lib/jsoncpp/writer.h
build/common/camera.o: lib/tetgen/tetgen.h /usr/include/stdio.h
build/common/camera.o: /usr/include/bits/types.h
build/common/camera.o: /usr/include/bits/typesizes.h /usr/include/libio.h
build/common/camera.o: /usr/include/_G_config.h /usr/include/wchar.h
build/common/camera.o: /usr/include/bits/stdio_lim.h
build/common/camera.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
build/common/camera.o: /usr/include/bits/waitflags.h
build/common/camera.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
build/common/camera.o: /usr/include/bits/endian.h
build/common/camera.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
build/common/camera.o: /usr/include/time.h /usr/include/sys/select.h
build/common/camera.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
build/common/camera.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
build/common/camera.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
build/common/camera.o: /usr/include/string.h /usr/include/xlocale.h
build/common/camera.o: /usr/include/math.h /usr/include/bits/huge_val.h
build/common/camera.o: /usr/include/bits/huge_valf.h
build/common/camera.o: /usr/include/bits/huge_vall.h /usr/include/bits/inf.h
build/common/camera.o: /usr/include/bits/nan.h /usr/include/bits/mathdef.h
build/common/camera.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
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
build/common/demoContext.o: lib/pez/gl3.h /usr/include/inttypes.h
build/common/demoContext.o: /usr/include/features.h /usr/include/sys/cdefs.h
build/common/demoContext.o: /usr/include/bits/wordsize.h
build/common/demoContext.o: /usr/include/gnu/stubs.h
build/common/demoContext.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/common/demoContext.o: /usr/include/bits/wchar.h common/camera.h
build/common/demoContext.o: lib/glm/gtc/matrix_transform.hpp lib/glm/glm.hpp
build/common/demoContext.o: lib/glm/gtc/matrix_transform.inl
build/common/demoContext.o: common/drawable.h
build/common/drawable.o: common/drawable.h lib/pez/pez.h lib/pez/gl3.h
build/common/drawable.o: /usr/include/inttypes.h /usr/include/features.h
build/common/drawable.o: /usr/include/sys/cdefs.h
build/common/drawable.o: /usr/include/bits/wordsize.h
build/common/drawable.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
build/common/drawable.o: /usr/include/stdint.h /usr/include/bits/wchar.h
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
build/common/effect.o: lib/pez/gl3.h /usr/include/inttypes.h
build/common/effect.o: /usr/include/features.h /usr/include/sys/cdefs.h
build/common/effect.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
build/common/effect.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/common/effect.o: /usr/include/bits/wchar.h lib/glm/glm.hpp
build/common/effect.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/effect.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/effect.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
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
build/common/effect.o: lib/glm/./core/_swizzle.hpp
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
build/common/init.o: /usr/include/inttypes.h /usr/include/features.h
build/common/init.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
build/common/init.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
build/common/init.o: /usr/include/stdint.h /usr/include/bits/wchar.h
build/common/init.o: lib/jsoncpp/json.h lib/jsoncpp/autolink.h
build/common/init.o: lib/jsoncpp/config.h lib/jsoncpp/value.h
build/common/init.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
build/common/init.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h
build/common/init.o: /usr/include/stdio.h /usr/include/bits/types.h
build/common/init.o: /usr/include/bits/typesizes.h /usr/include/libio.h
build/common/init.o: /usr/include/_G_config.h /usr/include/wchar.h
build/common/init.o: /usr/include/bits/stdio_lim.h
build/common/init.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
build/common/init.o: /usr/include/bits/waitflags.h
build/common/init.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
build/common/init.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
build/common/init.o: /usr/include/sys/types.h /usr/include/time.h
build/common/init.o: /usr/include/sys/select.h /usr/include/bits/select.h
build/common/init.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
build/common/init.o: /usr/include/sys/sysmacros.h
build/common/init.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
build/common/init.o: /usr/include/string.h /usr/include/xlocale.h
build/common/init.o: /usr/include/math.h /usr/include/bits/huge_val.h
build/common/init.o: /usr/include/bits/huge_valf.h
build/common/init.o: /usr/include/bits/huge_vall.h /usr/include/bits/inf.h
build/common/init.o: /usr/include/bits/nan.h /usr/include/bits/mathdef.h
build/common/init.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
build/common/instancer.o: common/instancer.h lib/pez/pez.h lib/pez/gl3.h
build/common/instancer.o: /usr/include/inttypes.h /usr/include/features.h
build/common/instancer.o: /usr/include/sys/cdefs.h
build/common/instancer.o: /usr/include/bits/wordsize.h
build/common/instancer.o: /usr/include/gnu/stubs.h
build/common/instancer.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/common/instancer.o: /usr/include/bits/wchar.h common/drawable.h
build/common/instancer.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp
build/common/instancer.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
build/common/instancer.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
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
build/common/programs.o: /usr/include/inttypes.h /usr/include/features.h
build/common/programs.o: /usr/include/sys/cdefs.h
build/common/programs.o: /usr/include/bits/wordsize.h
build/common/programs.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
build/common/programs.o: /usr/include/stdint.h /usr/include/bits/wchar.h
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
build/common/programs.o: lib/jsoncpp/reader.h lib/jsoncpp/writer.h
build/common/programs.o: lib/tetgen/tetgen.h /usr/include/stdio.h
build/common/programs.o: /usr/include/bits/types.h
build/common/programs.o: /usr/include/bits/typesizes.h /usr/include/libio.h
build/common/programs.o: /usr/include/_G_config.h /usr/include/wchar.h
build/common/programs.o: /usr/include/bits/stdio_lim.h
build/common/programs.o: /usr/include/bits/sys_errlist.h
build/common/programs.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
build/common/programs.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
build/common/programs.o: /usr/include/bits/endian.h
build/common/programs.o: /usr/include/bits/byteswap.h
build/common/programs.o: /usr/include/sys/types.h /usr/include/time.h
build/common/programs.o: /usr/include/sys/select.h /usr/include/bits/select.h
build/common/programs.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
build/common/programs.o: /usr/include/sys/sysmacros.h
build/common/programs.o: /usr/include/bits/pthreadtypes.h
build/common/programs.o: /usr/include/alloca.h /usr/include/string.h
build/common/programs.o: /usr/include/xlocale.h /usr/include/math.h
build/common/programs.o: /usr/include/bits/huge_val.h
build/common/programs.o: /usr/include/bits/huge_valf.h
build/common/programs.o: /usr/include/bits/huge_vall.h
build/common/programs.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
build/common/programs.o: /usr/include/bits/mathdef.h
build/common/programs.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
build/common/quad.o: common/quad.h common/drawable.h lib/pez/pez.h
build/common/quad.o: lib/pez/gl3.h /usr/include/inttypes.h
build/common/quad.o: /usr/include/features.h /usr/include/sys/cdefs.h
build/common/quad.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
build/common/quad.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/common/quad.o: /usr/include/bits/wchar.h lib/glm/glm.hpp
build/common/quad.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/quad.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/quad.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
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
build/common/texture.o: common/texture.h lib/pez/pez.h lib/pez/gl3.h
build/common/texture.o: /usr/include/inttypes.h /usr/include/features.h
build/common/texture.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
build/common/texture.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
build/common/texture.o: /usr/include/stdint.h /usr/include/bits/wchar.h
build/common/timer.o: common/timer.h /usr/include/time.h
build/common/timer.o: /usr/include/bits/types.h /usr/include/features.h
build/common/timer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
build/common/timer.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
build/common/timer.o: /usr/include/bits/typesizes.h common/drawable.h
build/common/timer.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/inttypes.h
build/common/timer.o: /usr/include/stdint.h /usr/include/bits/wchar.h
build/common/timer.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp
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
build/common/tube.o: lib/pez/gl3.h /usr/include/inttypes.h
build/common/tube.o: /usr/include/features.h /usr/include/sys/cdefs.h
build/common/tube.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
build/common/tube.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/common/tube.o: /usr/include/bits/wchar.h lib/glm/glm.hpp
build/common/tube.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/tube.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/tube.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
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
build/common/vao.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/inttypes.h
build/common/vao.o: /usr/include/features.h /usr/include/sys/cdefs.h
build/common/vao.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
build/common/vao.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
build/common/vao.o: /usr/include/bits/wchar.h common/typedefs.h common/init.h
build/common/vao.o: lib/jsoncpp/json.h lib/jsoncpp/autolink.h
build/common/vao.o: lib/jsoncpp/config.h lib/jsoncpp/value.h
build/common/vao.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
build/common/vao.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h
build/common/vao.o: /usr/include/stdio.h /usr/include/bits/types.h
build/common/vao.o: /usr/include/bits/typesizes.h /usr/include/libio.h
build/common/vao.o: /usr/include/_G_config.h /usr/include/wchar.h
build/common/vao.o: /usr/include/bits/stdio_lim.h
build/common/vao.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
build/common/vao.o: /usr/include/bits/waitflags.h
build/common/vao.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
build/common/vao.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
build/common/vao.o: /usr/include/sys/types.h /usr/include/time.h
build/common/vao.o: /usr/include/sys/select.h /usr/include/bits/select.h
build/common/vao.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
build/common/vao.o: /usr/include/sys/sysmacros.h
build/common/vao.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
build/common/vao.o: /usr/include/string.h /usr/include/xlocale.h
build/common/vao.o: /usr/include/math.h /usr/include/bits/huge_val.h
build/common/vao.o: /usr/include/bits/huge_valf.h
build/common/vao.o: /usr/include/bits/huge_vall.h /usr/include/bits/inf.h
build/common/vao.o: /usr/include/bits/nan.h /usr/include/bits/mathdef.h
build/common/vao.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
