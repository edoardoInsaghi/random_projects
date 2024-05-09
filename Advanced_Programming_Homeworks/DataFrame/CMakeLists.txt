cmake_minimum_required(VERSION 3.10)

project(Homework2_Alessio_Barrasso_Insaghi
        VERSION 1.0
        DESCRIPTION "Second and Third homeworks for the advanced programming course."
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GSL REQUIRED)
find_package(cxxopts REQUIRED)
find_package(Python3 3.10 COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

if(GSL_FOUND)
  message(STATUS "GSL found. Include directories: ${GSL_INCLUDE_DIRS}")
  include_directories(${GSL_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "GSL not found. Please install it here: https://www.gnu.org/software/gsl/")
endif()

if(cxxopts_FOUND)
  message(STATUS "cxxopts found. Include directories: ${GSL_INCLUDE_DIRS}")
  include_directories(${cxxopts_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "cxxopts not found. Please install it here: https://github.com/jarro2783/cxxopts.")
endif()

if(Python3_FOUND)
  message(STATUS "Python found. Include directories: ${Python3_INCLUDE_DIRS}")
  include_directories(${Python3_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "Python not found.")
endif()

if(pybind11_FOUND)
  message(STATUS "Pybind11 found. Include directories: ${pybind11_INCLUDE_DIRS}")
  include_directories(${pybind11_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "Pybind11 not found.")
endif()

add_library(statsMod SHARED src/statsMod.cpp)
add_library(integrMod SHARED ${GSL_INCLUDE_DIRS} src/integrMod.cpp)
target_link_libraries(integrMod PRIVATE ${GSL_LIBRARIES})

add_executable(main examples/mainc.cpp)
target_include_directories(main PRIVATE data)
target_link_libraries(main PRIVATE statsMod integrMod cxxopts::cxxopts)

target_compile_options(main PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

add_library(statsModPy MODULE bindings/statsMod_bindings.cpp src/statsMod.cpp)
target_link_libraries(statsModPy PRIVATE pybind11::module)

add_library(integrModPy MODULE bindings/integrMod_bindings.cpp src/integrMod.cpp)
target_link_libraries(integrModPy PRIVATE ${GSL_LIBRARIES} pybind11::module)