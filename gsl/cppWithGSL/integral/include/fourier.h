#ifndef FOURIER_H
#define FOURIER_H

struct FuncParams {             
  double wk;     
  bool getReal;  
}; 

struct FSParams {
  double wk;
  bool getReal;
};

struct FTParams {
  double w;
  bool getReal;
};

struct Result {             
  double real;     
  double img;     
  bool fail;  
};
 
#endif // FOURIER_H