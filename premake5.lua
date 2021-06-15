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
IncludeDir["GLAD"] = "libs/GLAD/include"
IncludeDir["stb"] = "libs/stb_image"
IncludeDir["glm"] = "libs/glm"
IncludeDir["imgui"] = "libs/imgui"

include "libs/GLAD"
include "libs/imgui"

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
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}"
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
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"STB_IMAGE_IMPLEMENTATION",
		"EMBER_OPENGL_ACTIVATED"
	}

	libdirs {
		"libs/SDL2/lib/x86"
	}

	links {
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_ttf.lib",
		"SDL2_mixer.lib",
		"GLAD",
		"ImGui"
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