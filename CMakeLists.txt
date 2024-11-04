cmake_minimum_required(VERSION 3.15)
project(Node_editor LANGUAGES CXX)

set(HEADER_PATH ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src)

file(GLOB HEADERS_FILES
	"${HEADER_PATH}/*.h"
	"${HEADER_PATH}/*.hpp"
	"${HEADER_PATH}/*.inl")
	
file(GLOB SOURCE_FILES
	"${SOURCE_PATH}/*.cpp")
	
set(MAIN_FILE ${CMAKE_SOURCE_DIR}/main.cpp)

add_subdirectory(external)

add_executable(${PROJECT_NAME} ${HEADERS_FILES} ${SOURCE_FILES} ${MAIN_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER_PATH}/)
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)

source_group("include" FILES ${HEADERS_FILES})
source_group("source" FILES ${SOURCE_FILES})

# Copy DLLs needed for runtime on Windows
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          #$<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
		  #$<TARGET_FILE:futw::futw>
		  #$<TARGET_FILE:cfao::cfao>
		  #$<TARGET_FILE:TGUI::TGUI>
          $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  endif()
endif()

# PROJECT SETTINGS
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMakePredefinedTargets")