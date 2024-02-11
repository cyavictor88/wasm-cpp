#include <stdio.h>
#include <math.h>
#include <iostream>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <emscripten/emscripten.h>

struct FuncParams {             
  double wk;     
  bool getReal;  
} ; 

extern "C" {
  double func(double x, void * params) {
    // if you pass in a double array for params
    // double *array = (double *)params;
    // double wk = array[0];
    // double getReal = array[1];

    FuncParams *p = static_cast<FuncParams*>(params);
    double wk = p->wk;
    bool getReal = p->getReal;
    const gsl_complex I = gsl_complex_rect(0.0, 1.0); // Imaginary unit
    const gsl_complex negative_wk_x_times_i = gsl_complex_mul_real(I,  -1 * wk * x); 
    const gsl_complex expo = gsl_complex_exp(negative_wk_x_times_i); // e^(-iwkx)
  
    double f_of_x = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    }, x);
    const gsl_complex expo_times_f_of_x = gsl_complex_mul_real(expo, f_of_x); // f(x) * e^(iwkx)
    
    if(getReal) return (double) GSL_REAL(expo_times_f_of_x);
    return (double) GSL_IMAG(expo_times_f_of_x);
  }

  EMSCRIPTEN_KEEPALIVE
  double* approxFourierSeries(double lower_bound, double upper_bound, double period, double k)
  {

    double wk = (2 * M_PI / period) * k;

    gsl_integration_workspace *work_space = gsl_integration_workspace_alloc (1000);
    double eps_abs = 1e-7;
    double eps_rel = 1e-7;
    double result, error;

    // if you want to pass a double array as param for function
    // double *params = (double *)malloc(2 * sizeof(double));
    // params[0] = wk;
    // params[1] = 1.0;

    FuncParams params = {wk, true};
    gsl_function Func;
    Func.function = &func;
    Func.params = &params;

    gsl_integration_qags(&Func, lower_bound, upper_bound, eps_abs, eps_rel, 1000, work_space, &result, &error); 
    double resultReal = result/period;
    double errorReal = error;

    params.getReal = false;
    gsl_integration_qags(&Func, lower_bound, upper_bound, eps_abs, eps_rel, 1000, work_space, &result, &error); 
    double resultImg  =  result/period;
    double errorImg =  error;

    gsl_integration_workspace_free(work_space);

    double* complexResult = new double[2];
    complexResult[0] = resultReal;
    complexResult[1] = resultImg;
    return complexResult;
  }

}