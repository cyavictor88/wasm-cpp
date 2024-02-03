

Generate `api_example.js` and `api_example.wasm`
```
emcc api_example.cpp -o api_example.js -sMODULARIZE -sEXPORTED_RUNTIME_METHODS=ccall,cwrap -sUSE_BOOST_HEADERS=1 -s "EXPORTED_FUNCTIONS=['_free','_malloc']"
```

if dont want to use `ccall` or `cwrap` in js
```
emcc api_example.cpp -o api_example.js -sMODULARIZE -sUSE_BOOST_HEADERS=1 -s "EXPORTED_FUNCTIONS=['_free','_malloc']"
```

test:
```
node testBoost.js
```
Reference: 
1. https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-compiled-c-functions-from-javascript-using-ccall-cwrap
2. https://stackoverflow.com/questions/71681491/passing-arrays-and-objects-from-javascript-to-c-in-web-assembly