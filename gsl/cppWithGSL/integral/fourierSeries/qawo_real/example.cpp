#include <stdio.h>
#include <math.h>
#include <iostream>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_errno.h>
#include <emscripten/emscripten.h>
#include "fourier.h"
#include <stdexcept>
using namespace std;
extern "C" {
  double func(double x, void * params) {

    double f_of_x = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    }, x);
    
    return (double) f_of_x;
  }

  EMSCRIPTEN_KEEPALIVE
  Result* approxFourierSeries(double lower_bound, double upper_bound, double omega, double k)
  {
    
    int status = 0, i; 
    double result = 0, abserr = 0.1;
    double alpha = 1.0;
    gsl_function fc;
    fc.function = &func;
    fc.params = &alpha;

    gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000) ;
    gsl_integration_qawo_table * wo;




    wo = gsl_integration_qawo_table_alloc (omega * k, upper_bound - lower_bound, GSL_INTEG_COSINE, 1000) ;
    /* All results are for GSL_IEEE_MODE=double-precision */

    Result* resultPtr = new Result;
    resultPtr->real = 0.0;
    resultPtr->img = 0.0;
    resultPtr->fail = false;



    try {
      status = gsl_integration_qawo (&fc, lower_bound, 1e-3, 1e-7, w->limit,w, wo, &result, &abserr) ;
    } catch (exception& e){
      cerr << "Cosine Exception: " << e.what() << endl;
      resultPtr->fail = true;
      gsl_integration_qawo_table_free (wo) ;
      gsl_integration_workspace_free (w) ;
      return resultPtr;
    } 

    resultPtr->real = result;
    
    if(k!=0){
      wo = gsl_integration_qawo_table_alloc (omega * k, upper_bound - lower_bound, GSL_INTEG_SINE, 1000) ;
      try {
        status = gsl_integration_qawo (&fc, lower_bound, 1e-3, 1e-7, w->limit,w, wo, &result, &abserr) ;
      } catch (exception& e){
        cerr << "Sine Exception: " << e.what() << endl;
        resultPtr->fail = true;
        gsl_integration_qawo_table_free (wo) ;
        gsl_integration_workspace_free (w) ;
        return resultPtr;
      } 
      resultPtr->img = result;
    }
    gsl_integration_qawo_table_free (wo) ;
    gsl_integration_workspace_free (w) ;
    return resultPtr;

  }

}