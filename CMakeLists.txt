cmake_minimum_required(VERSION 2.6)
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wunknown-pragmas -O0 -g3")
set(CMAKE_C_FLAGS_DEBUG   "-Wall -Wunknown-pragmas -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wunknown-pragmas -O0 -g3")
set(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wunknown-pragmas -O0 -g3")
set(CMAKE_BUILD_TYPE Release)

project(gmacs)
set(DESIRED_QT_VERSION 4)

#find_package(Qt COMPONENTS QtCore QtGui REQUIRED)
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)

set(PACKAGE_SOURCE_CODE
	src/Gmacs.cpp
	src/GmacsCompleter.cpp
	src/GmacsInteractiveDesigner.cpp
	src/GmacsKeyBind.cpp
	src/GmacsLineField.cpp
	src/GmacsLineNumberArea.cpp
	src/GmacsMainField.cpp
	src/GmacsMainWidget.cpp
	src/GmacsMainWindow.cpp
	src/GmacsParser.cpp
	src/GmacsPreprocessor.cpp
	src/GmacsScriptLoader.cpp
	src/GmacsStatusBar.cpp
	src/GmacsSyntaxHighlighter.cpp
	src/GmacsTabWidget.cpp
	src/GmacsTextField.cpp
	src/GmacsTokenizer.cpp
	src/GmacsWidget.cpp
	src/main.cpp
	include/moc_gmacs.cxx
)


set(PACKAGE_HEADERS include/gmacs.hpp)
qt4_wrap_cpp(PACKAGE_HEADERS_MOC ${PACKAGE_HEADERS})

include(${QT_USE_FILE})

set(PACKAGE_EXTRA_LIBRARY
	${QT_QTCORE_LIBRARY}
	${QT_QTWEBKIT_LIBRARY}
	${QT_QTOPENGL_LIBRARY}
	${QT_QTGUI_LIBRARY}
	)


set(PACKAGE_NAME    ${PROJECT_NAME})
set(PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(PACKAGE_STRING  ${CMAKE_PROJECT_NAME}-${PACKAGE_VERSION})

add_definitions(-DREDUCED -DANSI_DECLARATORS -DTRILIBRARY -DCDT_ONLY)
add_definitions(-DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/include  ${CMAKE_CURRENT_BINARY_DIR}
		${KONOHA_INCLUDE_DIRS})

find_path(HAVE_QTGUI QtGui  PATHS ${include_locations})
if(HAVE_QTGUI)
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ${HAVE_QTGUI})
endif()
include_directories(${INCLUDE_DIRS})

find_path(HAVE_QTOPENGL QtOpenGL PATHS ${include_locations})
if(HAVE_QTOPENGL)
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ${HAVE_QTOPENGL})
endif()
include_directories(${INCLUDE_DIRS})

find_path(HAVE_CLANG clang-c/Index.h PATHS ${include_locations})
if(HAVE_CLANG)
    add_definitions(-DGMACS_USING_CLANG=1)
    include_directories(${HAVE_CLANG}/clang-c)
endif()

find_path(HAVE_KONOHA konoha1.h PATHS ${include_locations})
if(HAVE_CLANG)
    add_definitions(-DGMACS_USING_KONOHA=1)
endif()

add_executable(${PACKAGE_NAME} ${PACKAGE_SOURCE_CODE} ${PACKAGE_HEADERS_MOC})

find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
   message(ERROR "OpenGL not found!!")
endif(NOT OPENGL_FOUND)

target_link_libraries(${PACKAGE_NAME} ${PACKAGE_EXTRA_LIBRARY} ${OPENGL_LIBRARIES} clang konoha)
