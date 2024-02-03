// make sure example.js and example.wasm is in the same directory as this testBoost.ts file

import boostModule from "./example";

export async function testBoost(){


  const wasmInstance = await boostModule();
  // simple function without paramenter
  console.log('daysInWeek:',wasmInstance._daysInWeek()); // values can be returned, etc.


  const res = wasmInstance._mypow(2,4); // direct calling works
  console.log('result 1: calling mypow,',res);

  // testing getting array from cpp
  const cppOutputArrPointer = wasmInstance._createIntArray(5); 
  const js_output_array = new Uint32Array(wasmInstance.HEAP32.buffer, cppOutputArrPointer, 5);
  console.log('returned i32 array from cpp:',js_output_array);

  // testing passing array to cpp
  const TYPES = {
    i8: { array: Int8Array, heap: "HEAP8" },
    i16: { array: Int16Array, heap: "HEAP16" },
    i32: { array: Int32Array, heap: "HEAP32" },
    f32: { array: Float32Array, heap: "HEAPF32" },
    f64: { array: Float64Array, heap: "HEAPF64" },
    u8: { array: Uint8Array, heap: "HEAPU8" },
    u16: { array: Uint16Array, heap: "HEAPU16" },
    u32: { array: Uint32Array, heap: "HEAPU32" }
  };

  const jsInputArr = [3,4];
  const type = TYPES.i32;
  const typedArray = type.array.from(jsInputArr);
  // Allocate memory for the integer array
  const heapPointer = wasmInstance._malloc(typedArray.length * typedArray.BYTES_PER_ELEMENT);
  wasmInstance[type.heap].set(typedArray, heapPointer >> 2);

  // Call the WebAssembly function with the integer array
  const resArr = wasmInstance._powArr(heapPointer, jsInputArr.length);
  console.log("result of using powArr function", resArr);

  // Free the allocated memory
  wasmInstance._free(heapPointer);

  return resArr;

}