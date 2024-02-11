#include <stdio.h>
#include <math.h>
#include <iostream>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <emscripten/emscripten.h>

struct FuncParams {             
  double w;     
  bool getReal;  
} ; 

extern "C" {
  double func(double x, void * params) {
    // if you pass in a double array for params
    // double *array = (double *)params;
    // double w = array[0];
    // double getReal = array[1];

    FuncParams *p = static_cast<FuncParams*>(params);
    double w = p->w;
    bool getReal = p->getReal;
    const gsl_complex I = gsl_complex_rect(0.0, 1.0); // Imaginary unit
    const gsl_complex negative_w_x_times_i = gsl_complex_mul_real(I,  -1 * w * x); 
    const gsl_complex expo = gsl_complex_exp(negative_w_x_times_i); // e^(-i * w *x)
  
    double f_of_x = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    }, x);
    const gsl_complex expo_times_f_of_x = gsl_complex_mul_real(expo, f_of_x); // f(x) * e^(i * w )
    
    if(getReal) return (double) GSL_REAL(expo_times_f_of_x);
    return (double) GSL_IMAG(expo_times_f_of_x);
  }

  EMSCRIPTEN_KEEPALIVE
  double* approxFourierTransform(double lower_bound, double upper_bound, double w)
  {
    gsl_integration_workspace *work_space = gsl_integration_workspace_alloc (1000);
    double eps_abs = 1e-7;
    double eps_rel = 1e-7;
    double result, error;

    // if you want to pass a double array as param for function
    // double *params = (double *)malloc(2 * sizeof(double));
    // params[0] = w;
    // params[1] = 1.0;

    FuncParams params = {w, true};
    gsl_function Func;
    Func.function = &func;
    Func.params = &params;

    gsl_integration_qags(&Func, lower_bound, upper_bound, eps_abs, eps_rel, 1000, work_space, &result, &error); 
    double resultReal = result;
    double errorReal = error;

    params.getReal = false;
    gsl_integration_qags(&Func, lower_bound, upper_bound, eps_abs, eps_rel, 1000, work_space, &result, &error); 
    double resultImg  =  result;
    double errorImg =  error;

    gsl_integration_workspace_free(work_space);

    double* output = new double[2];
    output[0] = resultReal;
    output[1] = resultImg;
    return output;
  }

}
