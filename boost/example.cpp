#include <iostream>
#include <emscripten/emscripten.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>

namespace mp = boost::multiprecision;

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    int daysInWeek() {
        return 7;
    }

    EMSCRIPTEN_KEEPALIVE
    int mypow(int a,int b) {
      mp::cpp_int x = mp::pow(mp::cpp_int(a), b);
      // std::cout << x << "\n";
      return (int) x;

    }
    EMSCRIPTEN_KEEPALIVE
    int* createIntArray(int size) {
      std::vector<int> vec(size);
      // Populate the integer vec with some values
      for (int i = 0; i < size; ++i) {
          vec[i] = i + 99; // Fill the array with values 1, 2, 3, ..., size
      }

      int* array = new int[vec.size()]; // Allocate memory for the array
      std::copy(vec.begin(), vec.end(), array); // Copy vector elements to the array
      return array;
    }


    EMSCRIPTEN_KEEPALIVE
    int powArr(int* a, int size) {
      mp::cpp_int x = mp::pow(mp::cpp_int(a[0]), a[1]);
      std::cout<<"Passed in array:[";
      for (size_t i = 0; i < size; i++)
      {
        std::cout<< a[i];
        if(i<size-1) std::cout<<",";
      }
      std::cout<<"]\n";
      
      return (int) x;

    }

}



