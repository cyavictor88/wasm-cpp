### Demonstrate how to use gsl in cpp wasm

Generate `example.js` and `example.wasm`

```
git clone https://github.com/ampl/gsl
cd gsl
git checkout tags/v2.7.0
emconfigure ./configure && emmake make 
```
(you will see errors about **duplicate symbol**, but that's ok, as long as **./.libs/libgsl.so.25** is produced)

to test:

```
cd ../
emcc ./gsl/.libs/libgsl.so.25  example.cpp -I ./gsl -lm -o example.js -sMODULARIZE
node testGSL.js
```



Reference:
1. https://emscripten.org/docs/compiling/Building-Projects.html
2. https://stackoverflow.com/questions/67159161/how-to-compile-the-c-gnu-scientific-library-gsl-to-web-assembly-using-emscript
3. https://gist.github.com/charcode78/e2214522cbe709adaa56546b430c2bc2 