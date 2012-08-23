CC=gcc
CXX=g++
CFLAGS=-std=c99 -Wall -c -Wc++-compat -O3 -Ilib -I. -g
CXXFLAGS=-Wall -c -O3 -Ilib -I. -g
OBJDIR=build

SHARED := \
	$(OBJDIR)/common/camera.o \
	$(OBJDIR)/common/demoContext.o \
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
	makedepend -- $(CFLAGS) -- *.cpp common/*.cpp fx/*.cpp 

# DO NOT DELETE

main.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/inttypes.h
main.o: /usr/include/features.h /usr/include/sys/cdefs.h
main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
main.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
main.o: /usr/include/bits/wchar.h common/camera.h lib/glm/glm.hpp
main.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
main.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
main.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
main.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
main.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
main.o: lib/glm/./core/type_gentype.hpp lib/glm/./core/type_size.hpp
main.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
main.o: lib/glm/./core/_swizzle.hpp lib/glm/./core/_swizzle_func.hpp
main.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
main.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
main.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
main.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
main.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
main.o: lib/glm/./core/type_mat2x3.hpp lib/glm/./core/type_mat2x3.inl
main.o: lib/glm/./core/type_mat2x4.hpp lib/glm/./core/type_mat2x4.inl
main.o: lib/glm/./core/type_mat3x2.hpp lib/glm/./core/type_mat3x2.inl
main.o: lib/glm/./core/type_mat3x3.hpp lib/glm/./core/type_mat3x3.inl
main.o: lib/glm/./core/type_mat3x4.hpp lib/glm/./core/type_mat3x4.inl
main.o: lib/glm/./core/type_mat4x2.hpp lib/glm/./core/type_mat4x2.inl
main.o: lib/glm/./core/type_mat4x3.hpp lib/glm/./core/type_mat4x3.inl
main.o: lib/glm/./core/type_mat4x4.hpp lib/glm/./core/type_mat4x4.inl
main.o: lib/glm/./core/func_trigonometric.hpp
main.o: lib/glm/./core/func_trigonometric.inl lib/glm/./core/_vectorize.hpp
main.o: lib/glm/./core/func_exponential.hpp
main.o: lib/glm/./core/func_exponential.inl lib/glm/./core/func_common.hpp
main.o: lib/glm/./core/_fixes.hpp lib/glm/./core/func_common.inl
main.o: lib/glm/./core/func_packing.hpp lib/glm/./core/func_packing.inl
main.o: lib/glm/./core/func_geometric.hpp lib/glm/./core/func_geometric.inl
main.o: lib/glm/./core/func_matrix.hpp lib/glm/./core/func_matrix.inl
main.o: lib/glm/./core/func_vector_relational.hpp
main.o: lib/glm/./core/func_vector_relational.inl
main.o: lib/glm/./core/func_integer.hpp lib/glm/./core/func_integer.inl
main.o: lib/glm/./core/func_noise.hpp lib/glm/./core/func_noise.inl
main.o: lib/glm/./core/_swizzle.hpp lib/glm/gtc/matrix_transform.hpp
main.o: lib/glm/glm.hpp lib/glm/gtc/matrix_transform.inl common/demoContext.h
main.o: lib/pez/pez.h common/camera.h common/drawable.h common/init.h
main.o: common/typedefs.h lib/jsoncpp/json.h lib/jsoncpp/autolink.h
main.o: lib/jsoncpp/config.h lib/jsoncpp/value.h lib/jsoncpp/forwards.h
main.o: lib/jsoncpp/reader.h lib/jsoncpp/writer.h lib/tetgen/tetgen.h
main.o: /usr/include/stdio.h /usr/include/bits/types.h
main.o: /usr/include/bits/typesizes.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
main.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/sys/types.h /usr/include/time.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
main.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
main.o: /usr/include/alloca.h /usr/include/string.h /usr/include/xlocale.h
main.o: /usr/include/math.h /usr/include/bits/huge_val.h
main.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
main.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
main.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
main.o: /usr/include/assert.h common/programs.h common/surface.h fx/all.h
main.o: fx/quads.h common/effect.h common/instancer.h common/vao.h
main.o: common/texture.h common/quad.h
tetknot.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/inttypes.h
tetknot.o: /usr/include/features.h /usr/include/sys/cdefs.h
tetknot.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
tetknot.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
tetknot.o: /usr/include/bits/wchar.h lib/jsoncpp/json.h
tetknot.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h lib/jsoncpp/value.h
tetknot.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h lib/jsoncpp/writer.h
tetknot.o: lib/tetgen/tetgen.h /usr/include/stdio.h /usr/include/bits/types.h
tetknot.o: /usr/include/bits/typesizes.h /usr/include/libio.h
tetknot.o: /usr/include/_G_config.h /usr/include/wchar.h
tetknot.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
tetknot.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
tetknot.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
tetknot.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
tetknot.o: /usr/include/sys/types.h /usr/include/time.h
tetknot.o: /usr/include/sys/select.h /usr/include/bits/select.h
tetknot.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
tetknot.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
tetknot.o: /usr/include/alloca.h /usr/include/string.h /usr/include/xlocale.h
tetknot.o: /usr/include/math.h /usr/include/bits/huge_val.h
tetknot.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
tetknot.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
tetknot.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
tetknot.o: /usr/include/assert.h common/init.h common/typedefs.h
tetknot.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
tetknot.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
tetknot.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
tetknot.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
tetknot.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
tetknot.o: lib/glm/./core/type_gentype.hpp lib/glm/./core/type_size.hpp
tetknot.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
tetknot.o: lib/glm/./core/_swizzle.hpp lib/glm/./core/_swizzle_func.hpp
tetknot.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
tetknot.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
tetknot.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
tetknot.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
tetknot.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
tetknot.o: lib/glm/./core/type_mat2x3.hpp lib/glm/./core/type_mat2x3.inl
tetknot.o: lib/glm/./core/type_mat2x4.hpp lib/glm/./core/type_mat2x4.inl
tetknot.o: lib/glm/./core/type_mat3x2.hpp lib/glm/./core/type_mat3x2.inl
tetknot.o: lib/glm/./core/type_mat3x3.hpp lib/glm/./core/type_mat3x3.inl
tetknot.o: lib/glm/./core/type_mat3x4.hpp lib/glm/./core/type_mat3x4.inl
tetknot.o: lib/glm/./core/type_mat4x2.hpp lib/glm/./core/type_mat4x2.inl
tetknot.o: lib/glm/./core/type_mat4x3.hpp lib/glm/./core/type_mat4x3.inl
tetknot.o: lib/glm/./core/type_mat4x4.hpp lib/glm/./core/type_mat4x4.inl
tetknot.o: lib/glm/./core/func_trigonometric.hpp
tetknot.o: lib/glm/./core/func_trigonometric.inl
tetknot.o: lib/glm/./core/_vectorize.hpp lib/glm/./core/func_exponential.hpp
tetknot.o: lib/glm/./core/func_exponential.inl lib/glm/./core/func_common.hpp
tetknot.o: lib/glm/./core/_fixes.hpp lib/glm/./core/func_common.inl
tetknot.o: lib/glm/./core/func_packing.hpp lib/glm/./core/func_packing.inl
tetknot.o: lib/glm/./core/func_geometric.hpp
tetknot.o: lib/glm/./core/func_geometric.inl lib/glm/./core/func_matrix.hpp
tetknot.o: lib/glm/./core/func_matrix.inl
tetknot.o: lib/glm/./core/func_vector_relational.hpp
tetknot.o: lib/glm/./core/func_vector_relational.inl
tetknot.o: lib/glm/./core/func_integer.hpp lib/glm/./core/func_integer.inl
tetknot.o: lib/glm/./core/func_noise.hpp lib/glm/./core/func_noise.inl
tetknot.o: lib/glm/./core/_swizzle.hpp lib/pez/pez.h common/programs.h
tetknot.o: common/tube.h common/drawable.h lib/glm/gtc/matrix_transform.hpp
tetknot.o: lib/glm/glm.hpp lib/glm/gtc/matrix_transform.inl
tetknot.o: lib/glm/gtx/constants.inl
common/camera.o: common/camera.h lib/glm/glm.hpp lib/glm/core/_fixes.hpp
common/camera.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
common/camera.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
common/camera.o: lib/glm/./core/type_half.hpp lib/glm/./core/type_half.inl
common/camera.o: lib/glm/./core/_detail.hpp lib/glm/./core/type_float.hpp
common/camera.o: lib/glm/./core/type_int.hpp lib/glm/./core/type_gentype.hpp
common/camera.o: lib/glm/./core/type_size.hpp lib/glm/./core/type_vec1.hpp
common/camera.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
common/camera.o: lib/glm/./core/_swizzle_func.hpp
common/camera.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/camera.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/camera.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/camera.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
common/camera.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
common/camera.o: lib/glm/./core/type_mat2x3.hpp
common/camera.o: lib/glm/./core/type_mat2x3.inl
common/camera.o: lib/glm/./core/type_mat2x4.hpp
common/camera.o: lib/glm/./core/type_mat2x4.inl
common/camera.o: lib/glm/./core/type_mat3x2.hpp
common/camera.o: lib/glm/./core/type_mat3x2.inl
common/camera.o: lib/glm/./core/type_mat3x3.hpp
common/camera.o: lib/glm/./core/type_mat3x3.inl
common/camera.o: lib/glm/./core/type_mat3x4.hpp
common/camera.o: lib/glm/./core/type_mat3x4.inl
common/camera.o: lib/glm/./core/type_mat4x2.hpp
common/camera.o: lib/glm/./core/type_mat4x2.inl
common/camera.o: lib/glm/./core/type_mat4x3.hpp
common/camera.o: lib/glm/./core/type_mat4x3.inl
common/camera.o: lib/glm/./core/type_mat4x4.hpp
common/camera.o: lib/glm/./core/type_mat4x4.inl
common/camera.o: lib/glm/./core/func_trigonometric.hpp
common/camera.o: lib/glm/./core/func_trigonometric.inl
common/camera.o: lib/glm/./core/_vectorize.hpp
common/camera.o: lib/glm/./core/func_exponential.hpp
common/camera.o: lib/glm/./core/func_exponential.inl
common/camera.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/camera.o: lib/glm/./core/func_common.inl
common/camera.o: lib/glm/./core/func_packing.hpp
common/camera.o: lib/glm/./core/func_packing.inl
common/camera.o: lib/glm/./core/func_geometric.hpp
common/camera.o: lib/glm/./core/func_geometric.inl
common/camera.o: lib/glm/./core/func_matrix.hpp
common/camera.o: lib/glm/./core/func_matrix.inl
common/camera.o: lib/glm/./core/func_vector_relational.hpp
common/camera.o: lib/glm/./core/func_vector_relational.inl
common/camera.o: lib/glm/./core/func_integer.hpp
common/camera.o: lib/glm/./core/func_integer.inl
common/camera.o: lib/glm/./core/func_noise.hpp lib/glm/./core/func_noise.inl
common/camera.o: lib/glm/./core/_swizzle.hpp lib/glm/gtc/matrix_transform.hpp
common/camera.o: lib/glm/glm.hpp lib/glm/gtc/matrix_transform.inl
common/camera.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/inttypes.h
common/camera.o: /usr/include/features.h /usr/include/sys/cdefs.h
common/camera.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
common/camera.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
common/camera.o: /usr/include/bits/wchar.h common/init.h common/typedefs.h
common/camera.o: lib/jsoncpp/json.h lib/jsoncpp/autolink.h
common/camera.o: lib/jsoncpp/config.h lib/jsoncpp/value.h
common/camera.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
common/camera.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h
common/camera.o: /usr/include/stdio.h /usr/include/bits/types.h
common/camera.o: /usr/include/bits/typesizes.h /usr/include/libio.h
common/camera.o: /usr/include/_G_config.h /usr/include/wchar.h
common/camera.o: /usr/include/bits/stdio_lim.h
common/camera.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
common/camera.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
common/camera.o: /usr/include/endian.h /usr/include/bits/endian.h
common/camera.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
common/camera.o: /usr/include/time.h /usr/include/sys/select.h
common/camera.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
common/camera.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
common/camera.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
common/camera.o: /usr/include/string.h /usr/include/xlocale.h
common/camera.o: /usr/include/math.h /usr/include/bits/huge_val.h
common/camera.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
common/camera.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
common/camera.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
common/camera.o: /usr/include/assert.h
common/demoContext.o: common/demoContext.h lib/glm/glm.hpp
common/demoContext.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
common/demoContext.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
common/demoContext.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
common/demoContext.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
common/demoContext.o: lib/glm/./core/type_float.hpp
common/demoContext.o: lib/glm/./core/type_int.hpp
common/demoContext.o: lib/glm/./core/type_gentype.hpp
common/demoContext.o: lib/glm/./core/type_size.hpp
common/demoContext.o: lib/glm/./core/type_vec1.hpp
common/demoContext.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
common/demoContext.o: lib/glm/./core/_swizzle_func.hpp
common/demoContext.o: lib/glm/./core/type_vec1.inl
common/demoContext.o: lib/glm/./core/type_vec2.hpp
common/demoContext.o: lib/glm/./core/type_vec2.inl
common/demoContext.o: lib/glm/./core/type_vec3.hpp
common/demoContext.o: lib/glm/./core/type_vec3.inl
common/demoContext.o: lib/glm/./core/type_vec4.hpp
common/demoContext.o: lib/glm/./core/type_vec4.inl
common/demoContext.o: lib/glm/./core/type_mat2x2.hpp
common/demoContext.o: lib/glm/./core/type_mat.hpp
common/demoContext.o: lib/glm/./core/type_mat2x2.inl
common/demoContext.o: lib/glm/./core/type_mat2x3.hpp
common/demoContext.o: lib/glm/./core/type_mat2x3.inl
common/demoContext.o: lib/glm/./core/type_mat2x4.hpp
common/demoContext.o: lib/glm/./core/type_mat2x4.inl
common/demoContext.o: lib/glm/./core/type_mat3x2.hpp
common/demoContext.o: lib/glm/./core/type_mat3x2.inl
common/demoContext.o: lib/glm/./core/type_mat3x3.hpp
common/demoContext.o: lib/glm/./core/type_mat3x3.inl
common/demoContext.o: lib/glm/./core/type_mat3x4.hpp
common/demoContext.o: lib/glm/./core/type_mat3x4.inl
common/demoContext.o: lib/glm/./core/type_mat4x2.hpp
common/demoContext.o: lib/glm/./core/type_mat4x2.inl
common/demoContext.o: lib/glm/./core/type_mat4x3.hpp
common/demoContext.o: lib/glm/./core/type_mat4x3.inl
common/demoContext.o: lib/glm/./core/type_mat4x4.hpp
common/demoContext.o: lib/glm/./core/type_mat4x4.inl
common/demoContext.o: lib/glm/./core/func_trigonometric.hpp
common/demoContext.o: lib/glm/./core/func_trigonometric.inl
common/demoContext.o: lib/glm/./core/_vectorize.hpp
common/demoContext.o: lib/glm/./core/func_exponential.hpp
common/demoContext.o: lib/glm/./core/func_exponential.inl
common/demoContext.o: lib/glm/./core/func_common.hpp
common/demoContext.o: lib/glm/./core/_fixes.hpp
common/demoContext.o: lib/glm/./core/func_common.inl
common/demoContext.o: lib/glm/./core/func_packing.hpp
common/demoContext.o: lib/glm/./core/func_packing.inl
common/demoContext.o: lib/glm/./core/func_geometric.hpp
common/demoContext.o: lib/glm/./core/func_geometric.inl
common/demoContext.o: lib/glm/./core/func_matrix.hpp
common/demoContext.o: lib/glm/./core/func_matrix.inl
common/demoContext.o: lib/glm/./core/func_vector_relational.hpp
common/demoContext.o: lib/glm/./core/func_vector_relational.inl
common/demoContext.o: lib/glm/./core/func_integer.hpp
common/demoContext.o: lib/glm/./core/func_integer.inl
common/demoContext.o: lib/glm/./core/func_noise.hpp
common/demoContext.o: lib/glm/./core/func_noise.inl
common/demoContext.o: lib/glm/./core/_swizzle.hpp lib/pez/pez.h lib/pez/gl3.h
common/demoContext.o: /usr/include/inttypes.h /usr/include/features.h
common/demoContext.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
common/demoContext.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
common/demoContext.o: /usr/include/stdint.h /usr/include/bits/wchar.h
common/demoContext.o: common/camera.h lib/glm/gtc/matrix_transform.hpp
common/demoContext.o: lib/glm/glm.hpp lib/glm/gtc/matrix_transform.inl
common/demoContext.o: common/drawable.h
common/drawable.o: common/drawable.h lib/pez/pez.h lib/pez/gl3.h
common/drawable.o: /usr/include/inttypes.h /usr/include/features.h
common/drawable.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
common/drawable.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
common/drawable.o: /usr/include/stdint.h /usr/include/bits/wchar.h
common/drawable.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp
common/drawable.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
common/drawable.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
common/drawable.o: lib/glm/./core/type_half.hpp lib/glm/./core/type_half.inl
common/drawable.o: lib/glm/./core/_detail.hpp lib/glm/./core/type_float.hpp
common/drawable.o: lib/glm/./core/type_int.hpp
common/drawable.o: lib/glm/./core/type_gentype.hpp
common/drawable.o: lib/glm/./core/type_size.hpp lib/glm/./core/type_vec1.hpp
common/drawable.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
common/drawable.o: lib/glm/./core/_swizzle_func.hpp
common/drawable.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/drawable.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/drawable.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/drawable.o: lib/glm/./core/type_vec4.inl
common/drawable.o: lib/glm/./core/type_mat2x2.hpp lib/glm/./core/type_mat.hpp
common/drawable.o: lib/glm/./core/type_mat2x2.inl
common/drawable.o: lib/glm/./core/type_mat2x3.hpp
common/drawable.o: lib/glm/./core/type_mat2x3.inl
common/drawable.o: lib/glm/./core/type_mat2x4.hpp
common/drawable.o: lib/glm/./core/type_mat2x4.inl
common/drawable.o: lib/glm/./core/type_mat3x2.hpp
common/drawable.o: lib/glm/./core/type_mat3x2.inl
common/drawable.o: lib/glm/./core/type_mat3x3.hpp
common/drawable.o: lib/glm/./core/type_mat3x3.inl
common/drawable.o: lib/glm/./core/type_mat3x4.hpp
common/drawable.o: lib/glm/./core/type_mat3x4.inl
common/drawable.o: lib/glm/./core/type_mat4x2.hpp
common/drawable.o: lib/glm/./core/type_mat4x2.inl
common/drawable.o: lib/glm/./core/type_mat4x3.hpp
common/drawable.o: lib/glm/./core/type_mat4x3.inl
common/drawable.o: lib/glm/./core/type_mat4x4.hpp
common/drawable.o: lib/glm/./core/type_mat4x4.inl
common/drawable.o: lib/glm/./core/func_trigonometric.hpp
common/drawable.o: lib/glm/./core/func_trigonometric.inl
common/drawable.o: lib/glm/./core/_vectorize.hpp
common/drawable.o: lib/glm/./core/func_exponential.hpp
common/drawable.o: lib/glm/./core/func_exponential.inl
common/drawable.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/drawable.o: lib/glm/./core/func_common.inl
common/drawable.o: lib/glm/./core/func_packing.hpp
common/drawable.o: lib/glm/./core/func_packing.inl
common/drawable.o: lib/glm/./core/func_geometric.hpp
common/drawable.o: lib/glm/./core/func_geometric.inl
common/drawable.o: lib/glm/./core/func_matrix.hpp
common/drawable.o: lib/glm/./core/func_matrix.inl
common/drawable.o: lib/glm/./core/func_vector_relational.hpp
common/drawable.o: lib/glm/./core/func_vector_relational.inl
common/drawable.o: lib/glm/./core/func_integer.hpp
common/drawable.o: lib/glm/./core/func_integer.inl
common/drawable.o: lib/glm/./core/func_noise.hpp
common/drawable.o: lib/glm/./core/func_noise.inl lib/glm/./core/_swizzle.hpp
common/drawable.o: common/demoContext.h common/camera.h
common/drawable.o: lib/glm/gtc/matrix_transform.hpp lib/glm/glm.hpp
common/drawable.o: lib/glm/gtc/matrix_transform.inl
common/effect.o: common/effect.h common/drawable.h lib/pez/pez.h
common/effect.o: lib/pez/gl3.h /usr/include/inttypes.h
common/effect.o: /usr/include/features.h /usr/include/sys/cdefs.h
common/effect.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
common/effect.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
common/effect.o: /usr/include/bits/wchar.h lib/glm/glm.hpp
common/effect.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
common/effect.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
common/effect.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
common/effect.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
common/effect.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
common/effect.o: lib/glm/./core/type_gentype.hpp lib/glm/./core/type_size.hpp
common/effect.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
common/effect.o: lib/glm/./core/_swizzle.hpp lib/glm/./core/_swizzle_func.hpp
common/effect.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/effect.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/effect.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/effect.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
common/effect.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
common/effect.o: lib/glm/./core/type_mat2x3.hpp
common/effect.o: lib/glm/./core/type_mat2x3.inl
common/effect.o: lib/glm/./core/type_mat2x4.hpp
common/effect.o: lib/glm/./core/type_mat2x4.inl
common/effect.o: lib/glm/./core/type_mat3x2.hpp
common/effect.o: lib/glm/./core/type_mat3x2.inl
common/effect.o: lib/glm/./core/type_mat3x3.hpp
common/effect.o: lib/glm/./core/type_mat3x3.inl
common/effect.o: lib/glm/./core/type_mat3x4.hpp
common/effect.o: lib/glm/./core/type_mat3x4.inl
common/effect.o: lib/glm/./core/type_mat4x2.hpp
common/effect.o: lib/glm/./core/type_mat4x2.inl
common/effect.o: lib/glm/./core/type_mat4x3.hpp
common/effect.o: lib/glm/./core/type_mat4x3.inl
common/effect.o: lib/glm/./core/type_mat4x4.hpp
common/effect.o: lib/glm/./core/type_mat4x4.inl
common/effect.o: lib/glm/./core/func_trigonometric.hpp
common/effect.o: lib/glm/./core/func_trigonometric.inl
common/effect.o: lib/glm/./core/_vectorize.hpp
common/effect.o: lib/glm/./core/func_exponential.hpp
common/effect.o: lib/glm/./core/func_exponential.inl
common/effect.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/effect.o: lib/glm/./core/func_common.inl
common/effect.o: lib/glm/./core/func_packing.hpp
common/effect.o: lib/glm/./core/func_packing.inl
common/effect.o: lib/glm/./core/func_geometric.hpp
common/effect.o: lib/glm/./core/func_geometric.inl
common/effect.o: lib/glm/./core/func_matrix.hpp
common/effect.o: lib/glm/./core/func_matrix.inl
common/effect.o: lib/glm/./core/func_vector_relational.hpp
common/effect.o: lib/glm/./core/func_vector_relational.inl
common/effect.o: lib/glm/./core/func_integer.hpp
common/effect.o: lib/glm/./core/func_integer.inl
common/effect.o: lib/glm/./core/func_noise.hpp lib/glm/./core/func_noise.inl
common/effect.o: lib/glm/./core/_swizzle.hpp
common/init.o: common/init.h common/typedefs.h lib/glm/glm.hpp
common/init.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
common/init.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
common/init.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
common/init.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
common/init.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
common/init.o: lib/glm/./core/type_gentype.hpp lib/glm/./core/type_size.hpp
common/init.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
common/init.o: lib/glm/./core/_swizzle.hpp lib/glm/./core/_swizzle_func.hpp
common/init.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/init.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/init.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/init.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
common/init.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
common/init.o: lib/glm/./core/type_mat2x3.hpp lib/glm/./core/type_mat2x3.inl
common/init.o: lib/glm/./core/type_mat2x4.hpp lib/glm/./core/type_mat2x4.inl
common/init.o: lib/glm/./core/type_mat3x2.hpp lib/glm/./core/type_mat3x2.inl
common/init.o: lib/glm/./core/type_mat3x3.hpp lib/glm/./core/type_mat3x3.inl
common/init.o: lib/glm/./core/type_mat3x4.hpp lib/glm/./core/type_mat3x4.inl
common/init.o: lib/glm/./core/type_mat4x2.hpp lib/glm/./core/type_mat4x2.inl
common/init.o: lib/glm/./core/type_mat4x3.hpp lib/glm/./core/type_mat4x3.inl
common/init.o: lib/glm/./core/type_mat4x4.hpp lib/glm/./core/type_mat4x4.inl
common/init.o: lib/glm/./core/func_trigonometric.hpp
common/init.o: lib/glm/./core/func_trigonometric.inl
common/init.o: lib/glm/./core/_vectorize.hpp
common/init.o: lib/glm/./core/func_exponential.hpp
common/init.o: lib/glm/./core/func_exponential.inl
common/init.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/init.o: lib/glm/./core/func_common.inl lib/glm/./core/func_packing.hpp
common/init.o: lib/glm/./core/func_packing.inl
common/init.o: lib/glm/./core/func_geometric.hpp
common/init.o: lib/glm/./core/func_geometric.inl
common/init.o: lib/glm/./core/func_matrix.hpp lib/glm/./core/func_matrix.inl
common/init.o: lib/glm/./core/func_vector_relational.hpp
common/init.o: lib/glm/./core/func_vector_relational.inl
common/init.o: lib/glm/./core/func_integer.hpp
common/init.o: lib/glm/./core/func_integer.inl lib/glm/./core/func_noise.hpp
common/init.o: lib/glm/./core/func_noise.inl lib/glm/./core/_swizzle.hpp
common/init.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/inttypes.h
common/init.o: /usr/include/features.h /usr/include/sys/cdefs.h
common/init.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
common/init.o: /usr/include/gnu/stubs-64.h /usr/include/stdint.h
common/init.o: /usr/include/bits/wchar.h lib/jsoncpp/json.h
common/init.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h
common/init.o: lib/jsoncpp/value.h lib/jsoncpp/forwards.h
common/init.o: lib/jsoncpp/reader.h lib/jsoncpp/writer.h lib/tetgen/tetgen.h
common/init.o: /usr/include/stdio.h /usr/include/bits/types.h
common/init.o: /usr/include/bits/typesizes.h /usr/include/libio.h
common/init.o: /usr/include/_G_config.h /usr/include/wchar.h
common/init.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
common/init.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
common/init.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
common/init.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
common/init.o: /usr/include/sys/types.h /usr/include/time.h
common/init.o: /usr/include/sys/select.h /usr/include/bits/select.h
common/init.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
common/init.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
common/init.o: /usr/include/alloca.h /usr/include/string.h
common/init.o: /usr/include/xlocale.h /usr/include/math.h
common/init.o: /usr/include/bits/huge_val.h /usr/include/bits/huge_valf.h
common/init.o: /usr/include/bits/huge_vall.h /usr/include/bits/inf.h
common/init.o: /usr/include/bits/nan.h /usr/include/bits/mathdef.h
common/init.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
common/instancer.o: common/instancer.h lib/pez/pez.h lib/pez/gl3.h
common/instancer.o: /usr/include/inttypes.h /usr/include/features.h
common/instancer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
common/instancer.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
common/instancer.o: /usr/include/stdint.h /usr/include/bits/wchar.h
common/instancer.o: common/drawable.h lib/glm/glm.hpp lib/glm/core/_fixes.hpp
common/instancer.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
common/instancer.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
common/instancer.o: lib/glm/./core/type_half.hpp lib/glm/./core/type_half.inl
common/instancer.o: lib/glm/./core/_detail.hpp lib/glm/./core/type_float.hpp
common/instancer.o: lib/glm/./core/type_int.hpp
common/instancer.o: lib/glm/./core/type_gentype.hpp
common/instancer.o: lib/glm/./core/type_size.hpp lib/glm/./core/type_vec1.hpp
common/instancer.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
common/instancer.o: lib/glm/./core/_swizzle_func.hpp
common/instancer.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/instancer.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/instancer.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/instancer.o: lib/glm/./core/type_vec4.inl
common/instancer.o: lib/glm/./core/type_mat2x2.hpp
common/instancer.o: lib/glm/./core/type_mat.hpp
common/instancer.o: lib/glm/./core/type_mat2x2.inl
common/instancer.o: lib/glm/./core/type_mat2x3.hpp
common/instancer.o: lib/glm/./core/type_mat2x3.inl
common/instancer.o: lib/glm/./core/type_mat2x4.hpp
common/instancer.o: lib/glm/./core/type_mat2x4.inl
common/instancer.o: lib/glm/./core/type_mat3x2.hpp
common/instancer.o: lib/glm/./core/type_mat3x2.inl
common/instancer.o: lib/glm/./core/type_mat3x3.hpp
common/instancer.o: lib/glm/./core/type_mat3x3.inl
common/instancer.o: lib/glm/./core/type_mat3x4.hpp
common/instancer.o: lib/glm/./core/type_mat3x4.inl
common/instancer.o: lib/glm/./core/type_mat4x2.hpp
common/instancer.o: lib/glm/./core/type_mat4x2.inl
common/instancer.o: lib/glm/./core/type_mat4x3.hpp
common/instancer.o: lib/glm/./core/type_mat4x3.inl
common/instancer.o: lib/glm/./core/type_mat4x4.hpp
common/instancer.o: lib/glm/./core/type_mat4x4.inl
common/instancer.o: lib/glm/./core/func_trigonometric.hpp
common/instancer.o: lib/glm/./core/func_trigonometric.inl
common/instancer.o: lib/glm/./core/_vectorize.hpp
common/instancer.o: lib/glm/./core/func_exponential.hpp
common/instancer.o: lib/glm/./core/func_exponential.inl
common/instancer.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/instancer.o: lib/glm/./core/func_common.inl
common/instancer.o: lib/glm/./core/func_packing.hpp
common/instancer.o: lib/glm/./core/func_packing.inl
common/instancer.o: lib/glm/./core/func_geometric.hpp
common/instancer.o: lib/glm/./core/func_geometric.inl
common/instancer.o: lib/glm/./core/func_matrix.hpp
common/instancer.o: lib/glm/./core/func_matrix.inl
common/instancer.o: lib/glm/./core/func_vector_relational.hpp
common/instancer.o: lib/glm/./core/func_vector_relational.inl
common/instancer.o: lib/glm/./core/func_integer.hpp
common/instancer.o: lib/glm/./core/func_integer.inl
common/instancer.o: lib/glm/./core/func_noise.hpp
common/instancer.o: lib/glm/./core/func_noise.inl lib/glm/./core/_swizzle.hpp
common/instancer.o: common/vao.h common/typedefs.h
common/programs.o: common/programs.h lib/pez/pez.h lib/pez/gl3.h
common/programs.o: /usr/include/inttypes.h /usr/include/features.h
common/programs.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
common/programs.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
common/programs.o: /usr/include/stdint.h /usr/include/bits/wchar.h
common/programs.o: common/init.h common/typedefs.h lib/glm/glm.hpp
common/programs.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
common/programs.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
common/programs.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
common/programs.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
common/programs.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
common/programs.o: lib/glm/./core/type_gentype.hpp
common/programs.o: lib/glm/./core/type_size.hpp lib/glm/./core/type_vec1.hpp
common/programs.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
common/programs.o: lib/glm/./core/_swizzle_func.hpp
common/programs.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/programs.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/programs.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/programs.o: lib/glm/./core/type_vec4.inl
common/programs.o: lib/glm/./core/type_mat2x2.hpp lib/glm/./core/type_mat.hpp
common/programs.o: lib/glm/./core/type_mat2x2.inl
common/programs.o: lib/glm/./core/type_mat2x3.hpp
common/programs.o: lib/glm/./core/type_mat2x3.inl
common/programs.o: lib/glm/./core/type_mat2x4.hpp
common/programs.o: lib/glm/./core/type_mat2x4.inl
common/programs.o: lib/glm/./core/type_mat3x2.hpp
common/programs.o: lib/glm/./core/type_mat3x2.inl
common/programs.o: lib/glm/./core/type_mat3x3.hpp
common/programs.o: lib/glm/./core/type_mat3x3.inl
common/programs.o: lib/glm/./core/type_mat3x4.hpp
common/programs.o: lib/glm/./core/type_mat3x4.inl
common/programs.o: lib/glm/./core/type_mat4x2.hpp
common/programs.o: lib/glm/./core/type_mat4x2.inl
common/programs.o: lib/glm/./core/type_mat4x3.hpp
common/programs.o: lib/glm/./core/type_mat4x3.inl
common/programs.o: lib/glm/./core/type_mat4x4.hpp
common/programs.o: lib/glm/./core/type_mat4x4.inl
common/programs.o: lib/glm/./core/func_trigonometric.hpp
common/programs.o: lib/glm/./core/func_trigonometric.inl
common/programs.o: lib/glm/./core/_vectorize.hpp
common/programs.o: lib/glm/./core/func_exponential.hpp
common/programs.o: lib/glm/./core/func_exponential.inl
common/programs.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/programs.o: lib/glm/./core/func_common.inl
common/programs.o: lib/glm/./core/func_packing.hpp
common/programs.o: lib/glm/./core/func_packing.inl
common/programs.o: lib/glm/./core/func_geometric.hpp
common/programs.o: lib/glm/./core/func_geometric.inl
common/programs.o: lib/glm/./core/func_matrix.hpp
common/programs.o: lib/glm/./core/func_matrix.inl
common/programs.o: lib/glm/./core/func_vector_relational.hpp
common/programs.o: lib/glm/./core/func_vector_relational.inl
common/programs.o: lib/glm/./core/func_integer.hpp
common/programs.o: lib/glm/./core/func_integer.inl
common/programs.o: lib/glm/./core/func_noise.hpp
common/programs.o: lib/glm/./core/func_noise.inl lib/glm/./core/_swizzle.hpp
common/programs.o: lib/jsoncpp/json.h lib/jsoncpp/autolink.h
common/programs.o: lib/jsoncpp/config.h lib/jsoncpp/value.h
common/programs.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
common/programs.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h
common/programs.o: /usr/include/stdio.h /usr/include/bits/types.h
common/programs.o: /usr/include/bits/typesizes.h /usr/include/libio.h
common/programs.o: /usr/include/_G_config.h /usr/include/wchar.h
common/programs.o: /usr/include/bits/stdio_lim.h
common/programs.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
common/programs.o: /usr/include/bits/waitflags.h
common/programs.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
common/programs.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
common/programs.o: /usr/include/sys/types.h /usr/include/time.h
common/programs.o: /usr/include/sys/select.h /usr/include/bits/select.h
common/programs.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
common/programs.o: /usr/include/sys/sysmacros.h
common/programs.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
common/programs.o: /usr/include/string.h /usr/include/xlocale.h
common/programs.o: /usr/include/math.h /usr/include/bits/huge_val.h
common/programs.o: /usr/include/bits/huge_valf.h
common/programs.o: /usr/include/bits/huge_vall.h /usr/include/bits/inf.h
common/programs.o: /usr/include/bits/nan.h /usr/include/bits/mathdef.h
common/programs.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
common/quad.o: common/quad.h common/drawable.h lib/pez/pez.h lib/pez/gl3.h
common/quad.o: /usr/include/inttypes.h /usr/include/features.h
common/quad.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
common/quad.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
common/quad.o: /usr/include/stdint.h /usr/include/bits/wchar.h
common/quad.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
common/quad.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
common/quad.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
common/quad.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
common/quad.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
common/quad.o: lib/glm/./core/type_gentype.hpp lib/glm/./core/type_size.hpp
common/quad.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
common/quad.o: lib/glm/./core/_swizzle.hpp lib/glm/./core/_swizzle_func.hpp
common/quad.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/quad.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/quad.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/quad.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
common/quad.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
common/quad.o: lib/glm/./core/type_mat2x3.hpp lib/glm/./core/type_mat2x3.inl
common/quad.o: lib/glm/./core/type_mat2x4.hpp lib/glm/./core/type_mat2x4.inl
common/quad.o: lib/glm/./core/type_mat3x2.hpp lib/glm/./core/type_mat3x2.inl
common/quad.o: lib/glm/./core/type_mat3x3.hpp lib/glm/./core/type_mat3x3.inl
common/quad.o: lib/glm/./core/type_mat3x4.hpp lib/glm/./core/type_mat3x4.inl
common/quad.o: lib/glm/./core/type_mat4x2.hpp lib/glm/./core/type_mat4x2.inl
common/quad.o: lib/glm/./core/type_mat4x3.hpp lib/glm/./core/type_mat4x3.inl
common/quad.o: lib/glm/./core/type_mat4x4.hpp lib/glm/./core/type_mat4x4.inl
common/quad.o: lib/glm/./core/func_trigonometric.hpp
common/quad.o: lib/glm/./core/func_trigonometric.inl
common/quad.o: lib/glm/./core/_vectorize.hpp
common/quad.o: lib/glm/./core/func_exponential.hpp
common/quad.o: lib/glm/./core/func_exponential.inl
common/quad.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/quad.o: lib/glm/./core/func_common.inl lib/glm/./core/func_packing.hpp
common/quad.o: lib/glm/./core/func_packing.inl
common/quad.o: lib/glm/./core/func_geometric.hpp
common/quad.o: lib/glm/./core/func_geometric.inl
common/quad.o: lib/glm/./core/func_matrix.hpp lib/glm/./core/func_matrix.inl
common/quad.o: lib/glm/./core/func_vector_relational.hpp
common/quad.o: lib/glm/./core/func_vector_relational.inl
common/quad.o: lib/glm/./core/func_integer.hpp
common/quad.o: lib/glm/./core/func_integer.inl lib/glm/./core/func_noise.hpp
common/quad.o: lib/glm/./core/func_noise.inl lib/glm/./core/_swizzle.hpp
common/quad.o: common/vao.h common/typedefs.h
common/texture.o: common/texture.h lib/pez/pez.h lib/pez/gl3.h
common/texture.o: /usr/include/inttypes.h /usr/include/features.h
common/texture.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
common/texture.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
common/texture.o: /usr/include/stdint.h /usr/include/bits/wchar.h
common/timer.o: common/timer.h /usr/include/time.h /usr/include/bits/types.h
common/timer.o: /usr/include/features.h /usr/include/sys/cdefs.h
common/timer.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
common/timer.o: /usr/include/gnu/stubs-64.h /usr/include/bits/typesizes.h
common/timer.o: common/drawable.h lib/pez/pez.h lib/pez/gl3.h
common/timer.o: /usr/include/inttypes.h /usr/include/stdint.h
common/timer.o: /usr/include/bits/wchar.h lib/glm/glm.hpp
common/timer.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
common/timer.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
common/timer.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
common/timer.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
common/timer.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
common/timer.o: lib/glm/./core/type_gentype.hpp lib/glm/./core/type_size.hpp
common/timer.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
common/timer.o: lib/glm/./core/_swizzle.hpp lib/glm/./core/_swizzle_func.hpp
common/timer.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/timer.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/timer.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/timer.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
common/timer.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
common/timer.o: lib/glm/./core/type_mat2x3.hpp lib/glm/./core/type_mat2x3.inl
common/timer.o: lib/glm/./core/type_mat2x4.hpp lib/glm/./core/type_mat2x4.inl
common/timer.o: lib/glm/./core/type_mat3x2.hpp lib/glm/./core/type_mat3x2.inl
common/timer.o: lib/glm/./core/type_mat3x3.hpp lib/glm/./core/type_mat3x3.inl
common/timer.o: lib/glm/./core/type_mat3x4.hpp lib/glm/./core/type_mat3x4.inl
common/timer.o: lib/glm/./core/type_mat4x2.hpp lib/glm/./core/type_mat4x2.inl
common/timer.o: lib/glm/./core/type_mat4x3.hpp lib/glm/./core/type_mat4x3.inl
common/timer.o: lib/glm/./core/type_mat4x4.hpp lib/glm/./core/type_mat4x4.inl
common/timer.o: lib/glm/./core/func_trigonometric.hpp
common/timer.o: lib/glm/./core/func_trigonometric.inl
common/timer.o: lib/glm/./core/_vectorize.hpp
common/timer.o: lib/glm/./core/func_exponential.hpp
common/timer.o: lib/glm/./core/func_exponential.inl
common/timer.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/timer.o: lib/glm/./core/func_common.inl
common/timer.o: lib/glm/./core/func_packing.hpp
common/timer.o: lib/glm/./core/func_packing.inl
common/timer.o: lib/glm/./core/func_geometric.hpp
common/timer.o: lib/glm/./core/func_geometric.inl
common/timer.o: lib/glm/./core/func_matrix.hpp lib/glm/./core/func_matrix.inl
common/timer.o: lib/glm/./core/func_vector_relational.hpp
common/timer.o: lib/glm/./core/func_vector_relational.inl
common/timer.o: lib/glm/./core/func_integer.hpp
common/timer.o: lib/glm/./core/func_integer.inl lib/glm/./core/func_noise.hpp
common/timer.o: lib/glm/./core/func_noise.inl lib/glm/./core/_swizzle.hpp
common/tube.o: common/tube.h common/drawable.h lib/pez/pez.h lib/pez/gl3.h
common/tube.o: /usr/include/inttypes.h /usr/include/features.h
common/tube.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
common/tube.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
common/tube.o: /usr/include/stdint.h /usr/include/bits/wchar.h
common/tube.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
common/tube.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
common/tube.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
common/tube.o: lib/glm/./core/type_half.inl lib/glm/./core/_detail.hpp
common/tube.o: lib/glm/./core/type_float.hpp lib/glm/./core/type_int.hpp
common/tube.o: lib/glm/./core/type_gentype.hpp lib/glm/./core/type_size.hpp
common/tube.o: lib/glm/./core/type_vec1.hpp lib/glm/./core/type_vec.hpp
common/tube.o: lib/glm/./core/_swizzle.hpp lib/glm/./core/_swizzle_func.hpp
common/tube.o: lib/glm/./core/type_vec1.inl lib/glm/./core/type_vec2.hpp
common/tube.o: lib/glm/./core/type_vec2.inl lib/glm/./core/type_vec3.hpp
common/tube.o: lib/glm/./core/type_vec3.inl lib/glm/./core/type_vec4.hpp
common/tube.o: lib/glm/./core/type_vec4.inl lib/glm/./core/type_mat2x2.hpp
common/tube.o: lib/glm/./core/type_mat.hpp lib/glm/./core/type_mat2x2.inl
common/tube.o: lib/glm/./core/type_mat2x3.hpp lib/glm/./core/type_mat2x3.inl
common/tube.o: lib/glm/./core/type_mat2x4.hpp lib/glm/./core/type_mat2x4.inl
common/tube.o: lib/glm/./core/type_mat3x2.hpp lib/glm/./core/type_mat3x2.inl
common/tube.o: lib/glm/./core/type_mat3x3.hpp lib/glm/./core/type_mat3x3.inl
common/tube.o: lib/glm/./core/type_mat3x4.hpp lib/glm/./core/type_mat3x4.inl
common/tube.o: lib/glm/./core/type_mat4x2.hpp lib/glm/./core/type_mat4x2.inl
common/tube.o: lib/glm/./core/type_mat4x3.hpp lib/glm/./core/type_mat4x3.inl
common/tube.o: lib/glm/./core/type_mat4x4.hpp lib/glm/./core/type_mat4x4.inl
common/tube.o: lib/glm/./core/func_trigonometric.hpp
common/tube.o: lib/glm/./core/func_trigonometric.inl
common/tube.o: lib/glm/./core/_vectorize.hpp
common/tube.o: lib/glm/./core/func_exponential.hpp
common/tube.o: lib/glm/./core/func_exponential.inl
common/tube.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/tube.o: lib/glm/./core/func_common.inl lib/glm/./core/func_packing.hpp
common/tube.o: lib/glm/./core/func_packing.inl
common/tube.o: lib/glm/./core/func_geometric.hpp
common/tube.o: lib/glm/./core/func_geometric.inl
common/tube.o: lib/glm/./core/func_matrix.hpp lib/glm/./core/func_matrix.inl
common/tube.o: lib/glm/./core/func_vector_relational.hpp
common/tube.o: lib/glm/./core/func_vector_relational.inl
common/tube.o: lib/glm/./core/func_integer.hpp
common/tube.o: lib/glm/./core/func_integer.inl lib/glm/./core/func_noise.hpp
common/tube.o: lib/glm/./core/func_noise.inl lib/glm/./core/_swizzle.hpp
common/tube.o: common/typedefs.h
common/vao.o: common/vao.h lib/glm/glm.hpp lib/glm/core/_fixes.hpp
common/vao.o: lib/glm/core/setup.hpp lib/glm/./core/_detail.hpp
common/vao.o: lib/glm/./core/setup.hpp lib/glm/./core/type.hpp
common/vao.o: lib/glm/./core/type_half.hpp lib/glm/./core/type_half.inl
common/vao.o: lib/glm/./core/_detail.hpp lib/glm/./core/type_float.hpp
common/vao.o: lib/glm/./core/type_int.hpp lib/glm/./core/type_gentype.hpp
common/vao.o: lib/glm/./core/type_size.hpp lib/glm/./core/type_vec1.hpp
common/vao.o: lib/glm/./core/type_vec.hpp lib/glm/./core/_swizzle.hpp
common/vao.o: lib/glm/./core/_swizzle_func.hpp lib/glm/./core/type_vec1.inl
common/vao.o: lib/glm/./core/type_vec2.hpp lib/glm/./core/type_vec2.inl
common/vao.o: lib/glm/./core/type_vec3.hpp lib/glm/./core/type_vec3.inl
common/vao.o: lib/glm/./core/type_vec4.hpp lib/glm/./core/type_vec4.inl
common/vao.o: lib/glm/./core/type_mat2x2.hpp lib/glm/./core/type_mat.hpp
common/vao.o: lib/glm/./core/type_mat2x2.inl lib/glm/./core/type_mat2x3.hpp
common/vao.o: lib/glm/./core/type_mat2x3.inl lib/glm/./core/type_mat2x4.hpp
common/vao.o: lib/glm/./core/type_mat2x4.inl lib/glm/./core/type_mat3x2.hpp
common/vao.o: lib/glm/./core/type_mat3x2.inl lib/glm/./core/type_mat3x3.hpp
common/vao.o: lib/glm/./core/type_mat3x3.inl lib/glm/./core/type_mat3x4.hpp
common/vao.o: lib/glm/./core/type_mat3x4.inl lib/glm/./core/type_mat4x2.hpp
common/vao.o: lib/glm/./core/type_mat4x2.inl lib/glm/./core/type_mat4x3.hpp
common/vao.o: lib/glm/./core/type_mat4x3.inl lib/glm/./core/type_mat4x4.hpp
common/vao.o: lib/glm/./core/type_mat4x4.inl
common/vao.o: lib/glm/./core/func_trigonometric.hpp
common/vao.o: lib/glm/./core/func_trigonometric.inl
common/vao.o: lib/glm/./core/_vectorize.hpp
common/vao.o: lib/glm/./core/func_exponential.hpp
common/vao.o: lib/glm/./core/func_exponential.inl
common/vao.o: lib/glm/./core/func_common.hpp lib/glm/./core/_fixes.hpp
common/vao.o: lib/glm/./core/func_common.inl lib/glm/./core/func_packing.hpp
common/vao.o: lib/glm/./core/func_packing.inl
common/vao.o: lib/glm/./core/func_geometric.hpp
common/vao.o: lib/glm/./core/func_geometric.inl
common/vao.o: lib/glm/./core/func_matrix.hpp lib/glm/./core/func_matrix.inl
common/vao.o: lib/glm/./core/func_vector_relational.hpp
common/vao.o: lib/glm/./core/func_vector_relational.inl
common/vao.o: lib/glm/./core/func_integer.hpp lib/glm/./core/func_integer.inl
common/vao.o: lib/glm/./core/func_noise.hpp lib/glm/./core/func_noise.inl
common/vao.o: lib/glm/./core/_swizzle.hpp lib/pez/pez.h lib/pez/gl3.h
common/vao.o: /usr/include/inttypes.h /usr/include/features.h
common/vao.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
common/vao.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
common/vao.o: /usr/include/stdint.h /usr/include/bits/wchar.h
common/vao.o: common/typedefs.h common/init.h lib/jsoncpp/json.h
common/vao.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h lib/jsoncpp/value.h
common/vao.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
common/vao.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h /usr/include/stdio.h
common/vao.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
common/vao.o: /usr/include/libio.h /usr/include/_G_config.h
common/vao.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
common/vao.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
common/vao.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
common/vao.o: /usr/include/endian.h /usr/include/bits/endian.h
common/vao.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
common/vao.o: /usr/include/time.h /usr/include/sys/select.h
common/vao.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
common/vao.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
common/vao.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
common/vao.o: /usr/include/string.h /usr/include/xlocale.h
common/vao.o: /usr/include/math.h /usr/include/bits/huge_val.h
common/vao.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
common/vao.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
common/vao.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
common/vao.o: /usr/include/assert.h
