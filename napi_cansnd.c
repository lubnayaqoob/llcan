#include <node_api.h>
#include <stdio.h>
#include "../llcan/llcan.h"
#include "../llcan/cansnd.c"
#include "../llcan/llcan.c"



//hello world native function

napi_value MyNapiFunction(napi_env env, napi_callback_info info)

{
	napi_status status;
	size_t argc = 5;
	napi_value argv[5];
	status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);// for call back funtion in index.js

	// argument 1 of index file function
	char my_dev[16];
	size_t devSize;
	status = napi_get_value_string_utf8(env, argv[0], my_dev, 16, &devSize);
	printf("------the dev is : %s\n", my_dev);
	if (status != napi_ok) {
		    napi_throw_error(env, "EINVAL", "Expected a string.");
	}

	// argument 2 of index file function
	int my_canID;
	status = napi_get_value_int32(env, argv[1], &my_canID);
	printf("------the canID is : %d\n", my_canID);

	// argument 3 of index file function
	unsigned int my_len;
	unsigned char my_len_char;
	status = napi_get_value_uint32(env, argv[2], &my_len);
	my_len_char = (unsigned char)my_len;
	printf("------the len is : %d\n", my_len_char);


	// argument 4 of index file function
	char my_arg_data[8];
	size_t arg_dataSize;
	status = napi_get_value_string_utf8(env, argv[3], my_arg_data, 8, &arg_dataSize);
	printf("------the arg_data is : %s\n", my_arg_data);

	//function calling
	int send_res = sendCANFrame(my_dev, my_canID, my_len_char, my_arg_data);

	napi_value val1;
	status = napi_create_int32(env,send_res, &val1);
	printf("------the arg_data after funtion calling is : %s\n", my_arg_data);
	
	/*
	napi_value val2;
	status = napi_create_int32(env, my_canID, &val2);
	
	napi_value val3;
	status = napi_create_string_utf8(env, my_len, NAPI_AUTO_LENGTH, &val3);

	napi_value val4;
	status = napi_create_string_utf8(env, my_arg_data, NAPI_AUTO_LENGTH, &val4);
	*/
	// return val1, val2, val3, val4;
	return val1;
}

napi_value Init(napi_env env, napi_value exports)

{
	napi_status status;
	napi_value result; //representing the JS function object for the newly created function.& represents a variable address
	//The native function 'helloWorld' should be called when this function object is invoked.
	status = napi_create_function(env, NULL, 0, MyNapiFunction, NULL, &result);

	if (status != napi_ok)
		return NULL;
	status = napi_set_named_property(env, exports, "MyNapiFunction", result);

	if (status != napi_ok)
		return NULL;
	return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
