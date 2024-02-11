#include <stdio.h>
#include <math.h>
#include <iostream>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <emscripten/emscripten.h>

extern "C" {

  EMSCRIPTEN_KEEPALIVE
  double funcRealFromJS(double x, double w) {
    const gsl_complex I = gsl_complex_rect(0.0, 1.0); // Imaginary unit
    const gsl_complex negative_wx_times_i = gsl_complex_mul_real(I,  -1 * w * x); // x * w
    const gsl_complex expo = gsl_complex_exp(negative_wx_times_i); // e^(i * w * x)
  

    // Call the JavaScript function from C++
    double f_of_x = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    }, x);

    const gsl_complex expo_times_f_of_x = gsl_complex_mul_real(expo, f_of_x); // f(x) * e^(i * w)
    
    // std::cout << "(x,y) =(" << x <<","<< f_of_x << ")\n";
    return (double) GSL_REAL(expo_times_f_of_x);
  }

  double funcImgFromJS(double x, double w){
    const gsl_complex I = gsl_complex_rect(0.0, 1.0); // Imaginary unit
    const gsl_complex negative_wx_times_i = gsl_complex_mul_real(I,  -1 * w * x); // x * w
    const gsl_complex expo = gsl_complex_exp(negative_wx_times_i); // e^(i * w * x)
  
    double f_of_x = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    }, x);

    const gsl_complex expo_times_f_of_x = gsl_complex_mul_real(expo, f_of_x); // f(x) * e^(i * w)
    
    // std::cout << "(x,y) =(" << x <<","<< f_of_x << ")\n";
    return (double) GSL_IMAG(expo_times_f_of_x);
  }

  EMSCRIPTEN_KEEPALIVE
  double fReal (double x, void * params) {
    // some function written in c 
    double w = *(double *) params;
    // double f = log(alpha*x) / sqrt(x);
    // return f;

    // use function from passed in from js
    return funcRealFromJS(x,w);
  }

  EMSCRIPTEN_KEEPALIVE
  double fImg (double x, void * params) {
    // some function written in c 
    double w = *(double *) params;
    // double f = log(alpha*x) / sqrt(x);
    // return f;

    // use function from passed in from js
    return funcImgFromJS(x,w);
  }


  EMSCRIPTEN_KEEPALIVE
  double* approxFourierTransform(double lower_bound, double upper_bound, double w)
  {
    gsl_integration_workspace *work_spaceReal = gsl_integration_workspace_alloc (1000);
    gsl_integration_workspace *work_spaceImg = gsl_integration_workspace_alloc (1000);
    
    double resultReal, errorReal;
    double resultImg, errorImg;
    // double expected = 30.11686789397568;
    // double w = 1.0;

    gsl_function FReal;
    FReal.function = &fReal;
    FReal.params = &w;

    gsl_function FImg;
    FImg.function = &fImg;
    FImg.params = &w;
    double eps_abs = 1e-7;
    double eps_rel = 1e-7;

    gsl_integration_qags(&FReal, lower_bound, upper_bound, eps_abs, eps_rel, 1000, work_spaceReal, &resultReal, &errorReal); 
    gsl_integration_qags (&FImg, lower_bound, upper_bound, eps_abs, eps_rel, 1000, work_spaceImg, &resultImg, &errorImg); 

    // printf ("result          = % .18f\n", result);
    // printf ("exact result    = % .18f\n", expected);
    // printf ("estimated error = % .18f\n", error);
    // printf ("intervals =  %d\n", work_space->size);
    double* output = new double[2]; // Allocate memory for the array
    output[0] = resultReal;
    output[1] = resultImg;
    return output;
  }

}
