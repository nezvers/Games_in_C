workspace "Games in C"
	configurations { "Debug", "Release", "Debug.DLL", "Release.DLL" }
	platforms { "x64"}
	defaultplatform "x64"
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Debug.DLL"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter "configurations:Release.DLL"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter { "platforms:x64" }
		architecture "x86_64"
		
	targetdir "bin/%{cfg.buildcfg}/"
		
project "raylib"
	filter "configurations:Debug.DLL OR Release.DLL"
		kind "SharedLib"
		defines {"BUILD_LIBTYPE_SHARED"}
		
	filter "configurations:Debug OR Release"
		kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
	
	filter "action:gmake*"
		links {"winmm"}
				
	filter{}
	
	location "build"
	language "C"
    cdialect "C11"
	targetdir "bin/%{cfg.buildcfg}"
	
	includedirs { "raylib/src", "raylib/src/external/glfw/include"}
	vpaths 
	{
		["Header Files"] = { "raylib/src/**.h"},
		["Source Files/*"] = {"raylib/src/**.c"},
	}
	files {"raylib/src/*.h", "raylib/src/*.c"}
	
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
	

project "NearMissSnake"
	kind "ConsoleApp"
	location "build"
	language "C"
    cdialect "C11"
	targetdir "bin/%{cfg.buildcfg}"
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
	
	vpaths 
	{
		["Header Files"] = { "NearMissSnake/*.h"},
		["Source Files"] = {"NearMissSnake/*.c"},
	}
	files {"NearMissSnake/*.c", "NearMissSnake/*.h"}

	links {"raylib", "winmm", "kernel32", "opengl32", "gdi32"}
	
	includedirs { "raylib/src", "include", "NearMissSnake" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib"}
		links {"raylib.lib", "winmm", "kernel32"}
		libdirs {"bin/%{cfg.buildcfg}"}
	
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
	

project "Tetris"
	kind "ConsoleApp"
	location "build"
	language "C"
    cdialect "C11"
	targetdir "bin/%{cfg.buildcfg}"
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
	
	vpaths 
	{
		["Header Files"] = { "Tetris/*.h"},
		["Source Files"] = {"Tetris/*.c"},
	}
	files {"Tetris/*.c", "Tetris/*.h"}

	links {"raylib", "winmm", "kernel32", "opengl32", "gdi32"}
	
	includedirs { "raylib/src", "include", "Tetris" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib"}
		links {"raylib.lib", "winmm", "kernel32"}
		libdirs {"bin/%{cfg.buildcfg}"}
	
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}






