project "SDL2_ttf"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/SDL2/SDL_ttf.h",
	}

	filter "system:windows"
		buildoptions { "-std=c11", "-lgdi32" }
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter { "system:windows", "configurations:release" }
		buildoptions "/MT"