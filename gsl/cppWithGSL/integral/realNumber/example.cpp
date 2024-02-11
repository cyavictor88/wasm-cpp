#include <stdio.h>
#include <math.h>
#include <iostream>
#include <gsl/gsl_integration.h>
#include <emscripten/emscripten.h>

extern "C" {

  double funcFromJS(double x) {
    // Call the JavaScript function from C++
    double result = EM_ASM_DOUBLE({
        // Use the exported JavaScript function
        return Module.jsFunc($0);
    }, x);
    std::cout << "(x,y) =(" << x <<","<< result << ")\n";
    return result;
  }

  double f (double x, void * params) {
    // some function written in c 
    // double alpha = *(double *) params;
    // double f = log(alpha*x) / sqrt(x);
    // return f;

    // use function from passed in from js
    return funcFromJS(x);
  }

  EMSCRIPTEN_KEEPALIVE
  double takeIntegral(double lower_bound, double upper_bound)
  {
    gsl_integration_workspace *work_space = gsl_integration_workspace_alloc (1000);
    
    double result, error;
    double expected = 30.11686789397568;
    double alpha = 1.0;

    gsl_function F;
    F.function = &f;
    F.params = &alpha;

    gsl_integration_qags (&F, lower_bound, upper_bound, 0, 1e-7, 1000, work_space, &result, &error); 

    printf ("result          = % .18f\n", result);
    printf ("exact result    = % .18f\n", expected);
    printf ("estimated error = % .18f\n", error);
    printf ("intervals =  %d\n", work_space->size);

    return result;
  }

}