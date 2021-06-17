workspace "Ember"
	architecture "x86"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SDL2"] = "libs/SDL2/include"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Ember/include",
		"%{IncludeDir.SDL2}"
	}

	links
	{
		"Ember"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "EMBER_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EMBER_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EMBER_DIST"
		runtime "Release"
		optimize "on"

project "Ember"
	location "Ember"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Ember/include",
		"%{IncludeDir.SDL2}"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"STB_IMAGE_IMPLEMENTATION"
	}

	libdirs {
		"libs/SDL2/lib/x86"
	}

	links {
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_ttf.lib",
		"SDL2_mixer.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "EMBER_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EMBER_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EMBER_DIST"
		runtime "Release"
		optimize "on"