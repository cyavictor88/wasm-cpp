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
    // if you pass in a double array for params
    // double *array = (double *)params;
    // double wk = array[0];
    // double getReal = array[1];

    FTParams *p = static_cast<FTParams*>(params);
    double w = p->w;
    bool getReal = p->getReal;

    double f_of_x = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    }, x);
    double f_of_neg_x = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    },  -x);
    if(getReal) return (double) f_of_x + f_of_neg_x;
    return (double) f_of_x - f_of_neg_x;
  }


/* f457(x) = 1/sqrt(x) */
/* integ(f457*cos(pi x / 2),x,0,+inf) = 1 */


  EMSCRIPTEN_KEEPALIVE
  Result* approxFourierTransform(double lower_bound, double omega)
  {
    double result = 0, abserr=0;

    gsl_integration_workspace* w = gsl_integration_workspace_alloc (1000) ;
    gsl_integration_workspace* wc = gsl_integration_workspace_alloc (1000) ;
    gsl_integration_qawo_table* wo = gsl_integration_qawo_table_alloc (omega, 1.0, GSL_INTEG_COSINE, 1000) ;
    FTParams params = {omega, true};

    gsl_function fc;
    fc.function = &func;
    fc.params = &params;

    Result* resultPtr = new Result;
    resultPtr->real = 0.0;
    resultPtr->img = 0.0;
    resultPtr->fail = false;

    int status = 0;


    try {
      status = gsl_integration_qawf (&fc, lower_bound, 1e-3, w->limit, w, wc, wo, &result, &abserr) ;
    } catch (exception& e){
      cout << "Real Exception: " << e.what() << endl;
      resultPtr->fail = true;
      gsl_integration_qawo_table_free (wo) ;
      gsl_integration_workspace_free (wc) ;
      gsl_integration_workspace_free (w) ;
      return resultPtr;
    } 

    if(status != GSL_SUCCESS){
      resultPtr->fail = true;
    }
    resultPtr->real = result;


    try {
      params.getReal = false;
      wo = gsl_integration_qawo_table_alloc (omega, 1.0, GSL_INTEG_SINE, 1000) ;
      status = gsl_integration_qawf (&fc, lower_bound, 1e-3, w->limit, w, wc, wo, &result, &abserr) ;
    } catch (exception& e){
      cout << "Img Exception: " << e.what() << endl;
      resultPtr->fail = true;
      gsl_integration_qawo_table_free (wo) ;
      gsl_integration_workspace_free (wc) ;
      gsl_integration_workspace_free (w) ;
      return resultPtr;
    } 
    if(status != GSL_SUCCESS){
      resultPtr->fail = true;
    }          
    resultPtr->img = result;

    gsl_integration_qawo_table_free (wo) ;
    gsl_integration_workspace_free (wc) ;
    gsl_integration_workspace_free (w) ;

    return resultPtr;
  }

}