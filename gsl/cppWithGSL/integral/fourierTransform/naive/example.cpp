#include <stdio.h>
#include <math.h>
#include <iostream>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <emscripten/emscripten.h>
#include <gsl/gsl_errno.h>
#include "fourier.h"

extern "C" {
  double func(double x, void * params) {
    // if you pass in a double array for params
    // double *array = (double *)params;
    // double w = array[0];
    // double getReal = array[1];

    FTParams *p = static_cast<FTParams*>(params);
    double w = p->w;
    bool getReal = p->getReal;
    const gsl_complex I = gsl_complex_rect(0.0, 1.0); // Imaginary unit
    const gsl_complex negative_w_x_times_i = gsl_complex_mul_real(I,  -1 * w * x); 
    const gsl_complex expo = gsl_complex_exp(negative_w_x_times_i); // e^(-iwx)
  
    double f_of_x = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    }, x);
    const gsl_complex expo_times_f_of_x = gsl_complex_mul_real(expo, f_of_x); // f(x) * e^(-iwx)
    
    if(getReal) return (double) GSL_REAL(expo_times_f_of_x);
    return (double) GSL_IMAG(expo_times_f_of_x);

    // return f_of_x;
  }

  EMSCRIPTEN_KEEPALIVE
  Result* approxFourierTransform(double lower_bound, double upper_bound, double w)
  {
    gsl_integration_workspace *work_space = gsl_integration_workspace_alloc (1000);
    double eps_rel =1e-7;
    
    double eps_abs =  1e-7;
    double result, error;

    // if you want to pass a double array as param for function
    // double *params = (double *)malloc(2 * sizeof(double));
    // params[0] = w;
    // params[1] = 1.0;

    FTParams params = {w, true};
    gsl_function Func;
    Func.function = &func;
    Func.params = &params;

    Result* resultPtr = new Result;
    resultPtr->real = 0.0;
    resultPtr->img = 0.0;
    resultPtr->fail = false;
    int status = 0;
    // definite integral
    status = gsl_integration_qags(&Func, lower_bound, upper_bound, eps_abs, eps_rel, 1000, work_space, &result, &error); 
    // indefinite integral
    // status = gsl_integration_qagi(&Func, eps_abs, eps_rel, work_space->limit, work_space, &result, &error); 
    if(status != GSL_SUCCESS){
      resultPtr->fail = true;
      return resultPtr;
    }
    double resultReal = result;
    double errorReal = error;

    params.getReal = false;
    // definite integral
    status = gsl_integration_qags(&Func, lower_bound, upper_bound, eps_abs, eps_rel, 1000, work_space, &result, &error); 
    // indefinite integral
    // gsl_integration_qawf
    // status = gsl_integration_qagi(&Func, eps_abs, eps_rel, 1000, work_space, &result, &error); 
    if(status != GSL_SUCCESS){
      resultPtr->fail = true;
      return resultPtr;
    }
    double resultImg  = result;
    double errorImg =  error;

    gsl_integration_workspace_free(work_space);

    resultPtr->real = resultReal;
    resultPtr->img = resultImg;
    return resultPtr;
  }

}
