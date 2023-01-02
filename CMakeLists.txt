cmake_minimum_required (VERSION 3.11)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # GIT_TAG v3.0.0
  # Version 3.0.0 does not support resizing when in a browser window. Using a manually specified commit here.
  GIT_TAG 9f9571190a9ce66f31189f65b404d5da849d3b03
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(tralloy
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(tralloy src/main.cpp src/addons/src/tralloy_data.cpp)
target_include_directories(tralloy PRIVATE src)

target_link_libraries(tralloy
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

string(APPEND CMAKE_CXX_FLAGS " -Wall") 
string(APPEND CMAKE_CXX_FLAGS " -Wextra") 
string(APPEND CMAKE_CXX_FLAGS " -pedantic") 
# string(APPEND CMAKE_EXE_LINKER_FLAGS " -O3") # Release. Comment out for faster build.

if (EMSCRIPTEN) 
  string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS") 
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s ASYNCIFY") 
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s PROXY_TO_PTHREAD") 

  # string(APPEND CMAKE_EXE_LINKER_FLAGS " -lidbfs.js") 

  foreach(file "index.html" "sw.js" "favicon.png")
    configure_file("src/${file}" ${file} COPYONLY)
  endforeach(file)
endif() 
