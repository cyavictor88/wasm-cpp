### Demonstrate how to use gsl in cpp wasm, and pass js function into cpp to take integral with gsl


Once you have compiled gsl source code with emscripten (see /gsl/cppWithGSL/REAME.md):

Generate `example.js` and `example.wasm`:

for Ubuntu:
```
cd ../
emcc [path/to/built/gsl]/.libs/libgsl.so.25  example.cpp -I [path/to/built/gsl] -lm -o example.js -sMODULARIZE
```

for MacOS:
```
cd ../
emcc [path/to/built/gsl]/.libs/libgsl.25.dylib  example.cpp -I [path/to/built/gsl] -lm -o example.js -sMODULARIZE
```

Test:

```
node testIntegral.js
```



Reference:
1. chatGPT
