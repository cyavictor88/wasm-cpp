var factory = require('./example.js');

factory().then((instance) => {
  const res = instance._bessel(); // direct calling works
  console.log('result in JS:',res);
  const res2 = instance._powAtoB(3.3,2); // direct calling works
  console.log('result in JS:',res2);
});