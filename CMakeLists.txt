cmake_minimum_required(VERSION 3.1...3.12)
project(SdlGui
	DESCRIPTION
		"A java inspired GUI based on SDL2 and OpenGl"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
	src/gui/progressbar.cpp
	src/gui/progressbar.h
	src/gui/textfield.cpp
	src/gui/textfield.h
	src/gui/traversalgroup.cpp
	src/gui/traversalgroup.h
	src/gui/verticallayout.h
)

add_subdirectory(MwLibrary2)
add_subdirectory(Signal)

add_library(SdlGui ${SOURCES})

target_include_directories(SdlGui
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)

target_link_libraries(SdlGui
	PUBLIC
		MwLibrary2
		Signal
)

option(SdlGuiTest "SdlGuiTest project is added" OFF)

if (SdlGuiTest)
	set(SOURCES_TEST
		srcTest/main.cpp
	)
	
	add_executable(SdlGuiTest ${SOURCES_TEST})
	
	target_include_directories(SdlGuiTest
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/srcTest
	)

	target_link_libraries(SdlGuiTest SdlGui)

	# Copy data to build folder.
	file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif (SdlGuiTest)
