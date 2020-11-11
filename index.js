
//napi addon
const addon = require('./build/Release/module');
//const napi_res = addon.MyNapiFunction(2, 3, 3, 5);
const napi_res = addon.MyNapiFunction("vcan0", 114, 4, "0x12 0x14 0x12 0x12");
console.log("addon.MyNapiFunction(response is)-------", napi_res);
//