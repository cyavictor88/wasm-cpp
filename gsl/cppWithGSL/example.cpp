#include <iostream>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h> 
#include <emscripten/emscripten.h>


extern "C" {
    EMSCRIPTEN_KEEPALIVE
    double bessel(){
        double x = 15.0;
        double y = gsl_sf_bessel_J0(x);
        std::cout << "J0(" << x << ") = " << y << std::endl;
        return y;
    }

    EMSCRIPTEN_KEEPALIVE
    double powAtoB(double a, int b){
        double y = gsl_pow_int (a,b)  ;
        std::cout << a << " ^ " << b << " = " << y << std::endl;
        return y;
    }
}

