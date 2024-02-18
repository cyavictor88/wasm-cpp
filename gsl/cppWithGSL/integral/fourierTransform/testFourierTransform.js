var factory = require('./example.js');

factory().then((instance) => {
  const lowerBound = -1000.0;
  const upperBound = 1000.0;

  // let start = -2;
  // let end = 2;
  // let step = 0.1;
  
  let start = -8;
  let end = 8;
  let step = 1;

  let ws = [];
  for (let i = start; i <= end; i += step) {
      // ws.push(i);
      ws.push(( Math.PI  ) * i);

  }

  function functionToIntegrate(x) {
    // https://en.wikipedia.org/wiki/Fourier_transform#Example
    return Math.cos(2*Math.PI*x*3)*Math.exp(-1*Math.PI*x*x);
  }
  instance['jsFunc'] = functionToIntegrate;

  for (let i = 0; i < ws.length; i++) {
    const w = ws[i];
    const resPtr = instance._approxFourierTransform(lowerBound,upperBound,w); // direct calling works
    // const outputArr = new Float64Array(instance.HEAPF64.buffer, res, 2);
    // const real = outputArr[0];
    // const img = outputArr[1];
    // const mag = (real**2 + img **2)**0.5
    // const phase = 180*Math.atan(img/real) /Math.PI
    // console.log('w:',w,'=>',outputArr, 'mag',mag,'phase',phase);
    const outputArr = new Float64Array(instance.HEAPF64.buffer, resPtr, 2);
    const fail = (new Int8Array(instance.HEAP32.buffer, resPtr+16, 1))[0]; // 16 because each double is 8 bytes in length
    instance._free(resPtr);
   
    const real = outputArr[0];
    const img = outputArr[1];
    const mag = (real**2 + img **2)**0.5
    const phase = 180*Math.atan(img/real) /Math.PI
    if(!fail) {
      console.log('w:',w,'=>','mag',parseFloat(mag.toFixed(5)),'phase',parseFloat(phase.toFixed(5)));
    }
  }

});