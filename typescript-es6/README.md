### To use the generated module with ES6 or TypeSscript 


I will use the same example.cpp in /boost as example:

when compiling:

instead of running:

```
emcc example.cpp -o example.js -sMODULARIZE -sUSE_BOOST_HEADERS=1 -s "EXPORTED_FUNCTIONS=['_free','_malloc']"
```

run
```
emcc example.cpp -o example.js -sMODULARIZE -sUSE_BOOST_HEADERS=1 -s "EXPORTED_FUNCTIONS=['_free','_malloc']" -s EXPORT_ES6=1 -s EXPORT_NAME="boostModule"
```

now to use in typescript project:

checkout `testBoost.ts` for the usage

remember to enable JS in typescript config:
```
"compilerOptions": {
  "allowJS":true
}
```

Reference:
1. https://stackoverflow.com/questions/53309095/how-load-an-emscripten-generated-module-with-es6-import