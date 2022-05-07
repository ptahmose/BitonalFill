# CMakeList.txt : CMake project for BitonalFill, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

INCLUDE(CheckIncludeFiles)

# Add source to this project's executable.
add_executable (BitonalFill 
                "BitonalFill.cpp" 
                "BitonalFill.h" 
                "FillFromBitonal.h" 
                "FillFromBitonal.cpp" 
                "FillFromBitonal_AVX.cpp" 
                "ColorPixelStructs.h" 
                "FillFromBitonalFillTest.cpp" 
                "FillFromBitonalFillTest.h" 
                "FillFromBitonal_NEON.cpp" 
                "FillRemainderLine.h")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

# check whether we can use AVX2-instructions -> we check for the presence of the file
CHECK_INCLUDE_FILES(immintrin.h IMMINTRINFOUND)
if (IMMINTRINFOUND)
 set(BITONALFILL_HAS_AVX 1)
else()
 set(BITONALFILL_HAS_AVX 0)
endif()


# check whether we can use ARM-Neon-instructions -> we check for the presence of the file
CHECK_INCLUDE_FILES(arm_neon.h ARMNEONFOUND)
if (ARMNEONFOUND)
  set(BITONALFILL_HAS_NEON 1)
else()
  set(BITONALFILL_HAS_NEON 0)
endif()

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
	ESCAPE_QUOTES @ONLY)     
  
# this is necessary so that we can find "config.h" which we created above
target_include_directories(BitonalFill PRIVATE  ${CMAKE_CURRENT_BINARY_DIR})  

# TODO: Add tests and install targets if needed.
