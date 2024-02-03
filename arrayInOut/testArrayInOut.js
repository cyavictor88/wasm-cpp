

var factory = require('./example.js');

factory().then((wasmInstance) => {


  // testing getting array from cpp
  const arrLen = 5;
  const cppOutputArrPointer = wasmInstance._getCPPArray(arrLen); 
  var js_output_array = new Uint32Array(wasmInstance.HEAP32.buffer, cppOutputArrPointer, arrLen);
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
  const sum = wasmInstance._sumJSArray(heapPointer, jsInputArr.length);
  console.log("result of sum of",jsInputArr,'=',sum);

  // Free the allocated memory
  wasmInstance._free(heapPointer);


  // testing In Out with arrays

  // Allocate memory for the integer array
  const heapPointer2 = wasmInstance._malloc(typedArray.length * typedArray.BYTES_PER_ELEMENT);
  wasmInstance[type.heap].set(typedArray, heapPointer2 >> 2);

  // Call the WebAssembly function with the integer array
  const cppOutputArrPointer2 = wasmInstance._inOutArray(heapPointer2, jsInputArr.length); 
  const js_output_array2 = new Uint32Array(wasmInstance.HEAP32.buffer, cppOutputArrPointer2,  jsInputArr.length);
  console.log('returned i32 array from cpp:',js_output_array2);
  // Free the allocated memory
  wasmInstance._free(heapPointer2);






});