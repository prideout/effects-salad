-- A solution contains projects, and defines the available configurations
solution "Salad"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
   project "Salad"
      kind "ConsoleApp"
      language "C++"
      buildoptions { "-Wall", "-std=c99", "-Wc++-compat", "-O3" }

      includedirs { "../lib", "../" }

      linkoptions { "../lib/tetgen/libtet.a", 
                    "../lib/jsoncpp/libjson_linux-gcc-4.4.6_libmt.a",
                    }
      links { "X11", "GL", "pthread" }

      os.rmdir("shaders")
      os.rmdir("data")

      postbuildcommands { 
                "mkdir -p shaders data",
                "cp ../shaders/* shaders",
                "cp ../data/* data" }

      libdirs { os.findlib("X11"),
                os.findlib("GL"),
                os.findlib("pthread"),
                "../lib/jsoncpp",
                "../lib",
                "../lib/tetgen"}

      files { 
              "../shaders/*",
              "../lib/noise/**.h", 
              "../lib/noise/**.cpp", 
              "../lib/tthread/**.h", 
              "../lib/tthread/**.cpp", 
              "../lib/pez/**.h", 
              "../lib/pez/**.c", 
              "../lib/lodepng/**.h", 
              "../lib/lodepng/**.c", 
              "../common/**.h", 
              "../common/**.cpp", 
              "../fx/**.hh", 
              "../fx/**.cpp", 
              "../main.cpp" }
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    

