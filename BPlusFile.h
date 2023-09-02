#pragma once
#include <iostream>
#include <sys/utsname.h>
#include <unistd.h>
//Si usas windows descomenta la linea de abajo y comenta la de arriba.
// #include <windows.h>


using namespace std;

class BPlusFile{
  string filename;
  long page_size;

public:
  BPlusFile(){
    this->filename = "bplus_datos.dat";
    struct utsname unameData;

    if (uname(&unameData) == 0) {
      page_size = sysconf(_SC_PAGESIZE);

      //Si usas windows descomenta estas lineas de abajo y comenta la de arriba.
      /*
      SYSTEM_INFO system_info;
      GetSystemInfo(&system_info);
      DWORD p_size = system_info.dwPageSize;
      page_size = static_cast<long>(p_size);
      */
    } else {
      page_size = 4096;
    }
  }

  ~BPlusFile(){}
};
