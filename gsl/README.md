generate gls.js and gsl.wasm to used by testGSL.js

```
git clone https://github.com/ampl/gsl
cd gsl
git checkout tags/v2.7.0

autoreconf -i && emconfigure ./configure && emmake make LDFLAGS=-all-static

emcc -g -O2 -o .libs/gsl.js -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=\[ccall\] -s LINKABLE=1 -s EXPORT_ALL=1  ./cblas/.libs/libgslcblas.a .libs/libgsl.a -lm 
```



test:

```
cd ../
cp gsl/.libs/gls.js ./
cp gsl/.libs/gsl.wasm ./
node testGSL.js
```



Reference:
1. https://stackoverflow.com/questions/67159161/how-to-compile-the-c-gnu-scientific-library-gsl-to-web-assembly-using-emscript