cmake_minimum_required(VERSION 3.27.4)
project(ShaderToyExample)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(include ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} extern/glad/include extern/stb/include)
file(GLOB MY_SOURCES "src/*.cpp")
file(GLOB EXTERN_SOURCES "extern/glad/src/glad.c")

foreach (FILE ${MY_SOURCES})
	message(STATUS "File: '${FILE}'")
endforeach()
add_executable(ShaderToyExample ${MY_SOURCES} ${EXTERN_SOURCES})

target_link_libraries(ShaderToyExample ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} dl)
