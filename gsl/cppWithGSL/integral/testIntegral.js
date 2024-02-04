var factory = require('./example.js');

factory().then((instance) => {
  const lowerBound = 0.0;
  const upperBound = 1.0
  function functionToIntegrate(x) {
    return x * 100 * Math.sin(x) ;
  }
  instance['jsFunc'] = functionToIntegrate;
  const res = instance._takeIntegral(lowerBound,upperBound); // direct calling works
  console.log('result in JS:', res);
});