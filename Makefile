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

build/main.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/stddef.h
build/main.o: /usr/include/_types.h /usr/include/sys/_types.h
build/main.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
build/main.o: /usr/include/sys/_posix_availability.h
build/main.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
build/main.o: /usr/include/inttypes.h /usr/include/stdint.h
build/main.o: /usr/include/stdbool.h common/camera.h lib/glm/glm.hpp
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
build/main.o: /usr/include/stdio.h /usr/include/Availability.h
build/main.o: /usr/include/AvailabilityInternal.h
build/main.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
build/main.o: /usr/include/stdlib.h /usr/include/sys/wait.h
build/main.o: /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h
build/main.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
build/main.o: /usr/include/i386/_structs.h /usr/include/sys/_structs.h
build/main.o: /usr/include/machine/_structs.h /usr/include/sys/resource.h
build/main.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
build/main.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
build/main.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
build/main.o: /usr/include/machine/types.h /usr/include/i386/types.h
build/main.o: /usr/include/string.h /usr/include/strings.h
build/main.o: /usr/include/secure/_string.h /usr/include/math.h
build/main.o: /usr/include/architecture/i386/math.h /usr/include/time.h
build/main.o: /usr/include/_structs.h /usr/include/assert.h common/programs.h
build/main.o: common/surface.h fx/all.h fx/quads.h common/effect.h
build/main.o: common/timer.h common/instancer.h common/vao.h common/texture.h
build/main.o: common/typedefs.h common/quad.h
build/tetknot.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/stddef.h
build/tetknot.o: /usr/include/_types.h /usr/include/sys/_types.h
build/tetknot.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
build/tetknot.o: /usr/include/sys/_posix_availability.h
build/tetknot.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
build/tetknot.o: /usr/include/inttypes.h /usr/include/stdint.h
build/tetknot.o: /usr/include/stdbool.h lib/jsoncpp/json.h
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
build/tetknot.o: lib/tetgen/tetgen.h /usr/include/stdio.h
build/tetknot.o: /usr/include/Availability.h
build/tetknot.o: /usr/include/AvailabilityInternal.h
build/tetknot.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
build/tetknot.o: /usr/include/stdlib.h /usr/include/sys/wait.h
build/tetknot.o: /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h
build/tetknot.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
build/tetknot.o: /usr/include/i386/_structs.h /usr/include/sys/_structs.h
build/tetknot.o: /usr/include/machine/_structs.h /usr/include/sys/resource.h
build/tetknot.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
build/tetknot.o: /usr/include/sys/_endian.h
build/tetknot.o: /usr/include/libkern/_OSByteOrder.h
build/tetknot.o: /usr/include/libkern/i386/_OSByteOrder.h
build/tetknot.o: /usr/include/alloca.h /usr/include/machine/types.h
build/tetknot.o: /usr/include/i386/types.h /usr/include/string.h
build/tetknot.o: /usr/include/strings.h /usr/include/secure/_string.h
build/tetknot.o: /usr/include/math.h /usr/include/architecture/i386/math.h
build/tetknot.o: /usr/include/time.h /usr/include/_structs.h
build/tetknot.o: /usr/include/assert.h common/programs.h common/tube.h
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
build/common/camera.o: lib/pez/gl3.h /usr/include/stddef.h
build/common/camera.o: /usr/include/_types.h /usr/include/sys/_types.h
build/common/camera.o: /usr/include/sys/cdefs.h
build/common/camera.o: /usr/include/sys/_symbol_aliasing.h
build/common/camera.o: /usr/include/sys/_posix_availability.h
build/common/camera.o: /usr/include/machine/_types.h
build/common/camera.o: /usr/include/i386/_types.h /usr/include/inttypes.h
build/common/camera.o: /usr/include/stdint.h /usr/include/stdbool.h
build/common/camera.o: common/init.h common/typedefs.h lib/jsoncpp/json.h
build/common/camera.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h
build/common/camera.o: lib/jsoncpp/value.h lib/jsoncpp/forwards.h
build/common/camera.o: lib/jsoncpp/reader.h lib/jsoncpp/features.h
build/common/camera.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h
build/common/camera.o: /usr/include/stdio.h /usr/include/Availability.h
build/common/camera.o: /usr/include/AvailabilityInternal.h
build/common/camera.o: /usr/include/secure/_stdio.h
build/common/camera.o: /usr/include/secure/_common.h /usr/include/stdlib.h
build/common/camera.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
build/common/camera.o: /usr/include/sys/appleapiopts.h
build/common/camera.o: /usr/include/machine/signal.h
build/common/camera.o: /usr/include/i386/signal.h
build/common/camera.o: /usr/include/i386/_structs.h
build/common/camera.o: /usr/include/sys/_structs.h
build/common/camera.o: /usr/include/machine/_structs.h
build/common/camera.o: /usr/include/sys/resource.h
build/common/camera.o: /usr/include/machine/endian.h
build/common/camera.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
build/common/camera.o: /usr/include/libkern/_OSByteOrder.h
build/common/camera.o: /usr/include/libkern/i386/_OSByteOrder.h
build/common/camera.o: /usr/include/alloca.h /usr/include/machine/types.h
build/common/camera.o: /usr/include/i386/types.h /usr/include/string.h
build/common/camera.o: /usr/include/strings.h /usr/include/secure/_string.h
build/common/camera.o: /usr/include/math.h
build/common/camera.o: /usr/include/architecture/i386/math.h
build/common/camera.o: /usr/include/time.h /usr/include/_structs.h
build/common/camera.o: /usr/include/assert.h
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
build/common/demoContext.o: lib/pez/gl3.h /usr/include/stddef.h
build/common/demoContext.o: /usr/include/_types.h /usr/include/sys/_types.h
build/common/demoContext.o: /usr/include/sys/cdefs.h
build/common/demoContext.o: /usr/include/sys/_symbol_aliasing.h
build/common/demoContext.o: /usr/include/sys/_posix_availability.h
build/common/demoContext.o: /usr/include/machine/_types.h
build/common/demoContext.o: /usr/include/i386/_types.h
build/common/demoContext.o: /usr/include/inttypes.h /usr/include/stdint.h
build/common/demoContext.o: /usr/include/stdbool.h common/camera.h
build/common/demoContext.o: lib/glm/gtc/matrix_transform.hpp lib/glm/glm.hpp
build/common/demoContext.o: lib/glm/gtc/matrix_transform.inl
build/common/demoContext.o: common/drawable.h
build/common/drawable.o: common/drawable.h lib/pez/pez.h lib/pez/gl3.h
build/common/drawable.o: /usr/include/stddef.h /usr/include/_types.h
build/common/drawable.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
build/common/drawable.o: /usr/include/sys/_symbol_aliasing.h
build/common/drawable.o: /usr/include/sys/_posix_availability.h
build/common/drawable.o: /usr/include/machine/_types.h
build/common/drawable.o: /usr/include/i386/_types.h /usr/include/inttypes.h
build/common/drawable.o: /usr/include/stdint.h /usr/include/stdbool.h
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
build/common/effect.o: lib/pez/gl3.h /usr/include/stddef.h
build/common/effect.o: /usr/include/_types.h /usr/include/sys/_types.h
build/common/effect.o: /usr/include/sys/cdefs.h
build/common/effect.o: /usr/include/sys/_symbol_aliasing.h
build/common/effect.o: /usr/include/sys/_posix_availability.h
build/common/effect.o: /usr/include/machine/_types.h
build/common/effect.o: /usr/include/i386/_types.h /usr/include/inttypes.h
build/common/effect.o: /usr/include/stdint.h /usr/include/stdbool.h
build/common/effect.o: lib/glm/glm.hpp lib/glm/core/_fixes.hpp
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
build/common/effect.o: /usr/include/time.h /usr/include/_structs.h
build/common/effect.o: /usr/include/sys/_structs.h
build/common/effect.o: /usr/include/machine/_structs.h
build/common/effect.o: /usr/include/i386/_structs.h
build/common/effect.o: /usr/include/sys/appleapiopts.h
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
build/common/init.o: /usr/include/stddef.h /usr/include/_types.h
build/common/init.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
build/common/init.o: /usr/include/sys/_symbol_aliasing.h
build/common/init.o: /usr/include/sys/_posix_availability.h
build/common/init.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
build/common/init.o: /usr/include/inttypes.h /usr/include/stdint.h
build/common/init.o: /usr/include/stdbool.h lib/jsoncpp/json.h
build/common/init.o: lib/jsoncpp/autolink.h lib/jsoncpp/config.h
build/common/init.o: lib/jsoncpp/value.h lib/jsoncpp/forwards.h
build/common/init.o: lib/jsoncpp/reader.h lib/jsoncpp/features.h
build/common/init.o: lib/jsoncpp/writer.h lib/tetgen/tetgen.h
build/common/init.o: /usr/include/stdio.h /usr/include/Availability.h
build/common/init.o: /usr/include/AvailabilityInternal.h
build/common/init.o: /usr/include/secure/_stdio.h
build/common/init.o: /usr/include/secure/_common.h /usr/include/stdlib.h
build/common/init.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
build/common/init.o: /usr/include/sys/appleapiopts.h
build/common/init.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
build/common/init.o: /usr/include/i386/_structs.h /usr/include/sys/_structs.h
build/common/init.o: /usr/include/machine/_structs.h
build/common/init.o: /usr/include/sys/resource.h
build/common/init.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
build/common/init.o: /usr/include/sys/_endian.h
build/common/init.o: /usr/include/libkern/_OSByteOrder.h
build/common/init.o: /usr/include/libkern/i386/_OSByteOrder.h
build/common/init.o: /usr/include/alloca.h /usr/include/machine/types.h
build/common/init.o: /usr/include/i386/types.h /usr/include/string.h
build/common/init.o: /usr/include/strings.h /usr/include/secure/_string.h
build/common/init.o: /usr/include/math.h
build/common/init.o: /usr/include/architecture/i386/math.h
build/common/init.o: /usr/include/time.h /usr/include/_structs.h
build/common/init.o: /usr/include/assert.h
build/common/instancer.o: common/instancer.h lib/pez/pez.h lib/pez/gl3.h
build/common/instancer.o: /usr/include/stddef.h /usr/include/_types.h
build/common/instancer.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
build/common/instancer.o: /usr/include/sys/_symbol_aliasing.h
build/common/instancer.o: /usr/include/sys/_posix_availability.h
build/common/instancer.o: /usr/include/machine/_types.h
build/common/instancer.o: /usr/include/i386/_types.h /usr/include/inttypes.h
build/common/instancer.o: /usr/include/stdint.h /usr/include/stdbool.h
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
build/common/programs.o: /usr/include/stddef.h /usr/include/_types.h
build/common/programs.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
build/common/programs.o: /usr/include/sys/_symbol_aliasing.h
build/common/programs.o: /usr/include/sys/_posix_availability.h
build/common/programs.o: /usr/include/machine/_types.h
build/common/programs.o: /usr/include/i386/_types.h /usr/include/inttypes.h
build/common/programs.o: /usr/include/stdint.h /usr/include/stdbool.h
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
build/common/programs.o: /usr/include/stdio.h /usr/include/Availability.h
build/common/programs.o: /usr/include/AvailabilityInternal.h
build/common/programs.o: /usr/include/secure/_stdio.h
build/common/programs.o: /usr/include/secure/_common.h /usr/include/stdlib.h
build/common/programs.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
build/common/programs.o: /usr/include/sys/appleapiopts.h
build/common/programs.o: /usr/include/machine/signal.h
build/common/programs.o: /usr/include/i386/signal.h
build/common/programs.o: /usr/include/i386/_structs.h
build/common/programs.o: /usr/include/sys/_structs.h
build/common/programs.o: /usr/include/machine/_structs.h
build/common/programs.o: /usr/include/sys/resource.h
build/common/programs.o: /usr/include/machine/endian.h
build/common/programs.o: /usr/include/i386/endian.h
build/common/programs.o: /usr/include/sys/_endian.h
build/common/programs.o: /usr/include/libkern/_OSByteOrder.h
build/common/programs.o: /usr/include/libkern/i386/_OSByteOrder.h
build/common/programs.o: /usr/include/alloca.h /usr/include/machine/types.h
build/common/programs.o: /usr/include/i386/types.h /usr/include/string.h
build/common/programs.o: /usr/include/strings.h /usr/include/secure/_string.h
build/common/programs.o: /usr/include/math.h
build/common/programs.o: /usr/include/architecture/i386/math.h
build/common/programs.o: /usr/include/time.h /usr/include/_structs.h
build/common/programs.o: /usr/include/assert.h
build/common/quad.o: common/quad.h common/drawable.h lib/pez/pez.h
build/common/quad.o: lib/pez/gl3.h /usr/include/stddef.h
build/common/quad.o: /usr/include/_types.h /usr/include/sys/_types.h
build/common/quad.o: /usr/include/sys/cdefs.h
build/common/quad.o: /usr/include/sys/_symbol_aliasing.h
build/common/quad.o: /usr/include/sys/_posix_availability.h
build/common/quad.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
build/common/quad.o: /usr/include/inttypes.h /usr/include/stdint.h
build/common/quad.o: /usr/include/stdbool.h lib/glm/glm.hpp
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
build/common/tetUtil.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
build/common/tetUtil.o: /usr/include/sys/_symbol_aliasing.h
build/common/tetUtil.o: /usr/include/sys/_posix_availability.h
build/common/tetUtil.o: /usr/include/Availability.h
build/common/tetUtil.o: /usr/include/AvailabilityInternal.h
build/common/tetUtil.o: /usr/include/_types.h /usr/include/sys/_types.h
build/common/tetUtil.o: /usr/include/machine/_types.h
build/common/tetUtil.o: /usr/include/i386/_types.h
build/common/tetUtil.o: /usr/include/secure/_stdio.h
build/common/tetUtil.o: /usr/include/secure/_common.h /usr/include/stdlib.h
build/common/tetUtil.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
build/common/tetUtil.o: /usr/include/sys/appleapiopts.h
build/common/tetUtil.o: /usr/include/machine/signal.h
build/common/tetUtil.o: /usr/include/i386/signal.h
build/common/tetUtil.o: /usr/include/i386/_structs.h
build/common/tetUtil.o: /usr/include/sys/_structs.h
build/common/tetUtil.o: /usr/include/machine/_structs.h
build/common/tetUtil.o: /usr/include/sys/resource.h
build/common/tetUtil.o: /usr/include/machine/endian.h
build/common/tetUtil.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
build/common/tetUtil.o: /usr/include/libkern/_OSByteOrder.h
build/common/tetUtil.o: /usr/include/libkern/i386/_OSByteOrder.h
build/common/tetUtil.o: /usr/include/alloca.h /usr/include/machine/types.h
build/common/tetUtil.o: /usr/include/i386/types.h /usr/include/string.h
build/common/tetUtil.o: /usr/include/strings.h /usr/include/secure/_string.h
build/common/tetUtil.o: /usr/include/math.h
build/common/tetUtil.o: /usr/include/architecture/i386/math.h
build/common/tetUtil.o: /usr/include/time.h /usr/include/_structs.h
build/common/tetUtil.o: /usr/include/assert.h /usr/include/stdint.h
build/common/tetUtil.o: lib/glm/gtx/constants.inl
build/common/texture.o: common/texture.h lib/pez/pez.h lib/pez/gl3.h
build/common/texture.o: /usr/include/stddef.h /usr/include/_types.h
build/common/texture.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
build/common/texture.o: /usr/include/sys/_symbol_aliasing.h
build/common/texture.o: /usr/include/sys/_posix_availability.h
build/common/texture.o: /usr/include/machine/_types.h
build/common/texture.o: /usr/include/i386/_types.h /usr/include/inttypes.h
build/common/texture.o: /usr/include/stdint.h /usr/include/stdbool.h
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
build/common/timer.o: common/timer.h /usr/include/time.h
build/common/timer.o: /usr/include/_types.h /usr/include/sys/_types.h
build/common/timer.o: /usr/include/sys/cdefs.h
build/common/timer.o: /usr/include/sys/_symbol_aliasing.h
build/common/timer.o: /usr/include/sys/_posix_availability.h
build/common/timer.o: /usr/include/machine/_types.h
build/common/timer.o: /usr/include/i386/_types.h /usr/include/_structs.h
build/common/timer.o: /usr/include/sys/_structs.h
build/common/timer.o: /usr/include/machine/_structs.h
build/common/timer.o: /usr/include/i386/_structs.h
build/common/timer.o: /usr/include/sys/appleapiopts.h common/drawable.h
build/common/timer.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/stddef.h
build/common/timer.o: /usr/include/inttypes.h /usr/include/stdint.h
build/common/timer.o: /usr/include/stdbool.h lib/glm/glm.hpp
build/common/timer.o: lib/glm/core/_fixes.hpp lib/glm/core/setup.hpp
build/common/timer.o: lib/glm/./core/_detail.hpp lib/glm/./core/setup.hpp
build/common/timer.o: lib/glm/./core/type.hpp lib/glm/./core/type_half.hpp
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
build/common/tube.o: lib/pez/gl3.h /usr/include/stddef.h
build/common/tube.o: /usr/include/_types.h /usr/include/sys/_types.h
build/common/tube.o: /usr/include/sys/cdefs.h
build/common/tube.o: /usr/include/sys/_symbol_aliasing.h
build/common/tube.o: /usr/include/sys/_posix_availability.h
build/common/tube.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
build/common/tube.o: /usr/include/inttypes.h /usr/include/stdint.h
build/common/tube.o: /usr/include/stdbool.h lib/glm/glm.hpp
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
build/common/vao.o: lib/pez/pez.h lib/pez/gl3.h /usr/include/stddef.h
build/common/vao.o: /usr/include/_types.h /usr/include/sys/_types.h
build/common/vao.o: /usr/include/sys/cdefs.h
build/common/vao.o: /usr/include/sys/_symbol_aliasing.h
build/common/vao.o: /usr/include/sys/_posix_availability.h
build/common/vao.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
build/common/vao.o: /usr/include/inttypes.h /usr/include/stdint.h
build/common/vao.o: /usr/include/stdbool.h common/typedefs.h common/init.h
build/common/vao.o: lib/jsoncpp/json.h lib/jsoncpp/autolink.h
build/common/vao.o: lib/jsoncpp/config.h lib/jsoncpp/value.h
build/common/vao.o: lib/jsoncpp/forwards.h lib/jsoncpp/reader.h
build/common/vao.o: lib/jsoncpp/features.h lib/jsoncpp/writer.h
build/common/vao.o: lib/tetgen/tetgen.h /usr/include/stdio.h
build/common/vao.o: /usr/include/Availability.h
build/common/vao.o: /usr/include/AvailabilityInternal.h
build/common/vao.o: /usr/include/secure/_stdio.h
build/common/vao.o: /usr/include/secure/_common.h /usr/include/stdlib.h
build/common/vao.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
build/common/vao.o: /usr/include/sys/appleapiopts.h
build/common/vao.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
build/common/vao.o: /usr/include/i386/_structs.h /usr/include/sys/_structs.h
build/common/vao.o: /usr/include/machine/_structs.h
build/common/vao.o: /usr/include/sys/resource.h /usr/include/machine/endian.h
build/common/vao.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
build/common/vao.o: /usr/include/libkern/_OSByteOrder.h
build/common/vao.o: /usr/include/libkern/i386/_OSByteOrder.h
build/common/vao.o: /usr/include/alloca.h /usr/include/machine/types.h
build/common/vao.o: /usr/include/i386/types.h /usr/include/string.h
build/common/vao.o: /usr/include/strings.h /usr/include/secure/_string.h
build/common/vao.o: /usr/include/math.h /usr/include/architecture/i386/math.h
build/common/vao.o: /usr/include/time.h /usr/include/_structs.h
build/common/vao.o: /usr/include/assert.h
