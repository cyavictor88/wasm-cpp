

var factory = require('./api_example.js');

factory().then((wasmInstance) => {

  // simple function without paramenter
  console.log('daysInWeek:',wasmInstance._daysInWeek()); // values can be returned, etc.


  const res = wasmInstance._mypow(2,4); // direct calling works
  console.log('result 1: calling mypow,',res);

  // # using ccall
  // const res2 = wasmInstance.ccall("mypow",'number',['number','number'], [3,2]); // using ccall etc. also work
  // console.log('result 2',res2);

  // # using cwrap
  // var mypower = wasmInstance.cwrap('mypow', // name of C function
  // 'number', // return type
  // ['number', 'number']); // ar
  // const res3 = mypower(5,2);
  // console.log('result 3',res3);


  // testing getting array from cpp
  const cppOutputArrPointer = wasmInstance._createIntArray(5); 
  var js_output_array = new Uint32Array(wasmInstance.HEAP32.buffer, cppOutputArrPointer, 5);
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


});