@echo off
set GLSLC_PATH=C:\VulkanSDK\1.3.268.0\Bin

"%GLSLC_PATH%\glslc.exe" basic_triangle.vert -o vert.spv
"%GLSLC_PATH%\glslc.exe" basic_triangle.frag -o frag.spv

PAUSE