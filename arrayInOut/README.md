### Demonstrate how to pass arrays between c++ and JS



Generate `example.js` and `example.wasm`
```
emcc example.cpp -o example.js -sMODULARIZE -s "EXPORTED_FUNCTIONS=['_free','_malloc']"
```


Test:
```
node testArrayInOut.js
```
Reference: 
1. https://stackoverflow.com/questions/71681491/passing-arrays-and-objects-from-javascript-to-c-in-web-assembly
