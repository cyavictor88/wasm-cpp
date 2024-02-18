var factory = require('./example.js');

factory().then((instance) => {
  const lowerBound = 0;


  let start = -4;
  let end = 4;
  let step = .1;
  // 
  // let ws = [Math.PI / 2.0];
  let ws = [];
  for (let i = start; i <= end; i += step) {
      ws.push(( 2*Math.PI  ) * i);
      // ws.push(i);
  }

  function functionToIntegrate(x) {
    // https://en.wikipedia.org/wiki/Fourier_transform#Example
    return Math.cos(2*Math.PI*x*3)*Math.exp(-1*Math.PI*x*x);
  }

  instance['jsFunc'] = functionToIntegrate;

  for (let i = 0; i < ws.length; i++) {
    const w = ws[i];
    try{
        const resPtr = instance._approxFourierTransform(lowerBound,w); // direct calling works
        const outputArr = new Float64Array(instance.HEAPF64.buffer, resPtr, 2);
        const fail = (new Int8Array(instance.HEAP32.buffer, resPtr+16, 1))[0]; // 16 because each double is 8 bytes in length
        instance._free(resPtr);
      
        const real = outputArr[0];
        const img = outputArr[1];
        const mag = (real**2 + img **2)**0.5
        const phase = 180*Math.atan(img/real) /Math.PI
        if(!fail) {
          console.log('w:',w,'=>','mag',parseFloat(mag.toFixed(5)),'phase',parseFloat(phase.toFixed(5)));
          // console.log('w:',w,'=>','real',real,'img',img);

        } else {
          console.log('w',w,'fail');
        }
      } catch (err){
        console.log('sdfds',err)
      }
    
  }

});