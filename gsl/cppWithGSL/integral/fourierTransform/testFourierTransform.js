var factory = require('./example.js');

factory().then((instance) => {
  const lowerBound = -1000.0;
  const upperBound = 1000.0;

  let start = -2;
  let end = 2;
  let step = 0.1;
  
  let ws = [];
  for (let i = start; i <= end; i += step) {
      ws.push(i);
  }

  function functionToIntegrate(x) {
    return Math.cos(x) ;
  }
  instance['jsFunc'] = functionToIntegrate;

  for (let i = 0; i < ws.length; i++) {
    const w = ws[i];
    const res = instance._approxFourierTransform(lowerBound,upperBound,w); // direct calling works
    const outputArr = new Float64Array(instance.HEAPF64.buffer, res, 2);
    const real = outputArr[0];
    const img = outputArr[1];
    const mag = (real**2 + img **2)**0.5
    const phase = 180*Math.atan(img/real) /Math.PI
    console.log('w:',w,'=>',outputArr, 'mag',mag,'phase',phase);
  
  }

});