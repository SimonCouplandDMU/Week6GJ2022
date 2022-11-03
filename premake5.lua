workspace "GLGJ"
	architecture "x64"
	startproject "Game"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project "Game"
	location "code"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")
		
	files
	{
		"code/**.h",
		"code/**.cpp",
		"scripts/*.h",
		"scripts/*.cpp"
	}

	includedirs
	{
		"code/include",
		"vendor/spdlog/include",
		"vendor/glfw/include",
		"vendor/Glad/include",
		"vendor/glm/",
		"vendor/STBimage",
		"vendor/enTT/single_include",
		"vendor/box2d/include"
	}
	
	links 
	{
		"GLFW",
		"Glad",
		"box2d"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"


group "Vendor"

	include "vendor/glfw"
	include "vendor/Glad"
	include "vendor/box2d"
	

