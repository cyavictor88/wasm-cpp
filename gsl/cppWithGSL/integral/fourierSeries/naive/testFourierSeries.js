var factory = require('./example.js');


async function getFS(functionToIntegrate, minK, maxK, lowerBound,upperBound){
  return factory().then((instance) => {
    const ck=[];
    const period = upperBound - lowerBound;
  
    let start = minK;
    let end = maxK;
    
    let ks = [];
    for (let i = start; i <= end; i += 1) {
      ks.push(i);
    }
  
    // function functionToIntegrate(x) {
    //   return x**3-12*x+16;
    //   // return x**2+Math.sin(x);
    // }
    instance['jsFunc'] = functionToIntegrate;
  
    for (let i = 0; i < ks.length; i++) {
      const k = ks[i];
      const resPtr = instance._approxFourierSeries(lowerBound, upperBound, 2*Math.PI/period, k);
      const outputArr = new Float64Array(instance.HEAPF64.buffer, resPtr,2);
      const fail = (new Int8Array(instance.HEAP32.buffer, resPtr+16, 1))[0]; // 16 because each double is 8 bytes in length
      instance._free(resPtr);
     
      const real = outputArr[0]/period;
      const img = outputArr[1]/period;
      const mag = (real**2 + img **2)**0.5
      const phase = 180*Math.atan(img/real) /Math.PI
      if(!fail) {
       
        ck.push([real, img ]);
        console.log('ck:',k,'=>',`real: ${real}, img: ${img} , mag: ${mag}, phase: ${phase}`);

        
      }
    }
    return {ck};
  });
}

function func1(x) {
  return x**3-12*x+16;
}
getFS(func1, -10, 10 , -6,6).then(res=>console.log(res))

function func2(x) {
  return 12*Math.cos(2*x)*Math.sin(x)+16;
}
getFS(func2, -4, 4 , 0,2 * Math.PI).then(res=>console.log(res))


