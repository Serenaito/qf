cmake_minimum_required(VERSION 3.5)

project(QuietFriend LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0079 NEW)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

add_library(Live2DCubismCore STATIC IMPORTED)
set(CRT MT)
set_target_properties(Live2DCubismCore
  PROPERTIES
    IMPORTED_LOCATION_DEBUG
    ${CMAKE_CURRENT_SOURCE_DIR}/third/live2d/lib/Live2DCubismCore_${CRT}d.lib
    IMPORTED_LOCATION_RELEASE
    ${CMAKE_CURRENT_SOURCE_DIR}/third/live2d/lib/Live2DCubismCore_${CRT}.lib
    INTERFACE_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/third/live2d/inc/
)

set(GLEW_PATH third/glew)
set(GLFW_PATH third/glfw)
add_subdirectory(${GLEW_PATH}/build/cmake)
#add_subdirectory(${GLFW_PATH})
set(FRAMEWORK_SOURCE OpenGL)
add_subdirectory(Framework)
target_compile_definitions(Framework PUBLIC CSM_TARGET_WIN_GL)
target_include_directories(Framework PUBLIC ${GLEW_PATH}/include)
target_link_libraries(Framework Live2DCubismCore glew_s)
find_package(OpenGL REQUIRED)
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQF_DEBUG")
include_directories(inc)
include_directories(src)
include_directories(${GLEW_PATH}/include)
include_directories(Framework/src)
include_directories(src/glwidget.h)
include_directories(third/stb)
aux_source_directory(src QF_SOURCES_FILE)

add_executable(QuietFriend ${QF_SOURCES_FILE})
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup")
target_link_libraries(QuietFriend PRIVATE Qt5::Widgets Qt5::OpenGL glew_s Framework winmm.lib user32.lib dwmapi.lib
    debug -NODEFAULTLIB:libcmtd.lib
    optimized -NODEFAULTLIB:libcmt.lib)
