# CMakeList.txt : CMake project for BitonalFill, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (BitonalFill 
                "BitonalFill.cpp" 
                "BitonalFill.h" 
                "FillFromBitonal.h" 
                "FillFromBitonal.cpp" 
                "FillFromBitonal_AVX2.cpp" 
                "ColorPixelStructs.h" 
                "FillFromBitonalFillTest.cpp" 
                "FillFromBitonalFillTest.h")

 if (MSVC)
  set_source_files_properties(FillFromBitonal_AVX2.cpp PROPERTIES COMPILE_FLAGS /arch:AVX2)
endif()

# TODO: Add tests and install targets if needed.
