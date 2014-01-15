CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(SdlGui)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
if (MSVC)
else (MSVC)
	SET(CMAKE_CXX_FLAGS "-std=c++11")
endif (MSVC)

# Source files.
SET(SOURCES
	src/gui/borderlayout.h
	src/gui/button.cpp
	src/gui/button.h
	src/gui/checkbox.cpp
	src/gui/checkbox.h
	src/gui/component.cpp
	src/gui/component.h
	src/gui/dimension.h
	src/gui/flowlayout.h
	src/gui/frame.cpp
	src/gui/frame.h
	src/gui/gridlayout.h
	src/gui/traversalgroup.cpp
	src/gui/traversalgroup.h
	src/gui/inputformatter.h
	src/gui/label.cpp
	src/gui/label.h
	src/gui/layoutmanager.h
	src/gui/panel.cpp
	src/gui/panel.h
	src/gui/verticallayout.h
	src/gui/horizontallayout.h
	src/gui/textfield.cpp
	src/gui/textfield.h
)

SET(SOURCES_TEST
	srcTest/main.cpp
)
# End of source files.

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(SDL2_mixer REQUIRED)
FIND_PACKAGE(SDL2_ttf REQUIRED)

ADD_SUBDIRECTORY(MwLibrary2)
ADD_SUBDIRECTORY(Signal)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_MIXER_INCLUDE_DIR}
	MwLibrary2/src/
	Signal/src/
)

ADD_LIBRARY(SdlGui ${SOURCES})

SET(ALL_SDL_LIBRARIES
	${SDL2_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
)

INCLUDE_DIRECTORIES(src)
ADD_EXECUTABLE(SdlGuiTest ${SOURCES_TEST})

TARGET_LINK_LIBRARIES(SdlGuiTest 
	SdlGui
	MwLibrary2
	Signal
	${ALL_SDL_LIBRARIES}
	${OPENGL_LIBRARIES}
)

FILE(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.
