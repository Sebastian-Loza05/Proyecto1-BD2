# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-src"
  "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-build"
  "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-subbuild/ds-populate-prefix"
  "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-subbuild/ds-populate-prefix/tmp"
  "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp"
  "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src"
  "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/BD2/PROYECTO1/GUI/build-GUI-Desktop_Qt_6_5_1_MinGW_64_bit-Debug/_deps/ds-subbuild/ds-populate-prefix/src/ds-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
