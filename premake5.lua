workspace "Presto"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Presto"
	location "Presto"
	kind "SharedLib" -- DLL
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "prpch.h" -- PRE-COMPILED HEADER
	pchsource "Presto/src/prpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
	}

	filter "system:windows"  -- WINDOWS ONLY
		cppdialect "C++20"
		staticruntime "On"
		systemversion "10.0"  -- WINDOWS SDK VERSION

		defines {
			"PR_PLATFORM_WINDOWS", "PR_BUILD_DLL"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Duality")
		}
		
		links {"XInput9_1_0.lib"}

	filter "configurations:Debug"
		defines "PR_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PR_DIST"
		optimize "On"

project "Duality"
	location "Duality"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"Presto/vendor/spdlog/include",
		"Presto/src"
	}

	links {
		"Presto"
	}

	filter "system:windows"  -- WINDOWS ONLY
		cppdialect "C++20"
		staticruntime "On"
		systemversion "10.0"  -- WINDOWS SDK VERSION

		defines {
			"PR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PR_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PR_DIST"
		optimize "On"