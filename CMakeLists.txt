cmake_minimum_required(VERSION 3.0)
project(SdlGui)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/MwLibrary2/cmake/Modules/")
if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "-std=c++1y")
	option(Debug "Set projects to debug mode.")
	if (Debug)
		set(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "Build type.")
		add_definitions(-D_DEBUG)
	else ()
		set(CMAKE_BUILD_TYPE "Release" CACHE INTERNAL "Build type.")
	endif ()
endif ()

# Source files.
set(SOURCES
	src/gui/borderlayout.h
	src/gui/button.cpp
	src/gui/button.h
	src/gui/checkbox.cpp
	src/gui/checkbox.h
	src/gui/combobox.cpp
	src/gui/combobox.h
	src/gui/component.cpp
	src/gui/component.h
	src/gui/dimension.h
	src/gui/flowlayout.h
	src/gui/frame.cpp
	src/gui/frame.h
	src/gui/graphic.cpp
	src/gui/graphic.h
	src/gui/gridlayout.h
	src/gui/guishader.cpp
	src/gui/guishader.h
	src/gui/guivertexdata.h
	src/gui/horizontallayout.h
	src/gui/inputformatter.h
	src/gui/label.cpp
	src/gui/label.h
	src/gui/layoutmanager.h
	src/gui/panel.cpp
	src/gui/panel.h
	src/gui/textfield.cpp
	src/gui/textfield.h
	src/gui/traversalgroup.cpp
	src/gui/traversalgroup.h
	src/gui/verticallayout.h
)

set(SOURCES_TEST
	srcTest/main.cpp
)
# End of source files.

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(MwLibrary2)
add_subdirectory(Signal)

option(SdlGuiTest "SdlGuiTest project is added" OFF)

include_directories(
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_MIXER_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	MwLibrary2/src/
	Signal/src/
)

add_library(SdlGui ${SOURCES})

set(ALL_MW_LIBRARIES
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
)

if (SdlGuiTest)
	include_directories(src)
	add_executable(SdlGuiTest ${SOURCES_TEST})

	if (MSVC)
		set_target_properties(SdlGuiTest PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt")
	endif (MSVC)

	target_link_libraries(SdlGuiTest SdlGui MwLibrary2 ${ALL_MW_LIBRARIES})

	file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.
endif (SdlGuiTest)
