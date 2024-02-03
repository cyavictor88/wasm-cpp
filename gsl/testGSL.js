var factory = require('./gsl.js');

factory().then((instance) => {
  // Compute the value of the Bessel function for x = 5.0:
  var besselRes = instance._gsl_sf_bessel_J0(5.0);

  // Calculate the hypergeometric cumulative probability distribution for:
  // 4: Number of successes (white balls among the taken)
  // 7: Number of white balls in the urn
  // 19: Number of black balls in the urn
  // 13: Number of balls taken
  var hg = instance._gsl_cdf_hypergeometric_P(4, 7, 19, 13);

  // Do the same using `ccall`
  var hg_ccal = instance.ccall("gsl_cdf_hypergeometric_P",
    "number",
    ["number", "number", "number", "number" ],
    [4, 7, 19, 13]);

  console.log(`besselRes is: ${besselRes}`);
  console.log(`gsl_cdf_hypergeometric_P(4,7,19,13): ${hg}`);
  console.log(`ccall("gsl_cdf_hypergeometric_P",4,7,19,13): ${hg_ccal}`);
});


// using async to run
async function asyncFun(){
  const instance = await factory();
  var besselRes = instance._gsl_sf_bessel_J0(5.0);
  console.log('async await result: ',besselRes);
  return besselRes;
}

asyncFun();