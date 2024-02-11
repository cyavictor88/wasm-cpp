var factory = require('./example.js');

factory().then((instance) => {
  const upperBound = 1;
  const lowerBound = -0.;
  const period = upperBound - lowerBound;

  let start = -5;
  let end = 5;
  
  let ks = [];
  for (let i = start; i <= end; i += 1) {
    ks.push(i);
  }

  function functionToIntegrate(x) {
    return Math.sin(2*Math.PI* 4*x);
  }
  instance['jsFunc'] = functionToIntegrate;

  for (let i = 0; i < ks.length; i++) {
    const k = ks[i];
    const res = instance._approxFourierSeries(lowerBound, upperBound, period, k);
    const outputArr = new Float64Array(instance.HEAPF64.buffer, res, 2);
    const real = outputArr[0];
    const img = outputArr[1];
    const mag = (real**2 + img **2)**0.5
    const phase = 180*Math.atan(img/real) /Math.PI
    console.log('k:',k,'=>','mag',parseFloat(mag.toFixed(5)),'phase',parseFloat(phase.toFixed(5)));
  }


});