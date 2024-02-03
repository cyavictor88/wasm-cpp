#include <iostream>
#include <emscripten/emscripten.h>
#include <vector>


extern "C" {

    EMSCRIPTEN_KEEPALIVE
    int sumJSArray(int* arr, int size) {
      int sum = 0;
      for (size_t i = 0; i < size; i++)
      {
        sum = sum + arr[i];
      }
      return sum;
    }

    EMSCRIPTEN_KEEPALIVE
    int* getCPPArray(int size) {
      std::vector<int> vec(size);
      // Populate the integer vec with some values
      for (int i = 0; i < size; ++i) {
          vec[i] = i; // Fill the array with values 1, 2, 3, ..., size
      }

      int* array = new int[vec.size()]; // Allocate memory for the array
      std::copy(vec.begin(), vec.end(), array); // Copy vector elements to the array
      return array;
    }


    EMSCRIPTEN_KEEPALIVE
    int* inOutArray(int* arr, int size) {
      std::vector<int> vec(size);
      // Populate the integer vec with some values
      for (int i = 0; i < size; ++i) {
          vec[i] = arr[i] + 10; // Fill the array with values 1, 2, 3, ..., size
      }

      int* array = new int[vec.size()]; // Allocate memory for the array
      std::copy(vec.begin(), vec.end(), array); // Copy vector elements to the array
      return array;
    }

}



