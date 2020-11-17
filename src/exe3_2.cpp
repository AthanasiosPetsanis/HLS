#include <iostream>
#include "ac_channel.h"
#include "ac_int.h"


static const int W = 4;
typedef ac_int<W, false> dtype;

dtype data_in[10];
dtype prev_data_in = 0;
short int counter = 1;

// Encoder
void runlength_encode(ac_channel<dtype> &in, ac_channel<dtype> &out){	
	if (in.available(10)) {
		data_in[0] = in.read();
		for (int i = 1; i < 10; ++i){
			data_in[i] = in.read();
			if (data_in[i] == data_in[i-1]){
				++counter;
			}
			else {
				out.write(counter);
				out.write(data_in[i-1]);
				counter = 1;
			}
		}	
		out.write(counter);
		out.write(data_in[9]);
	}
}

// Decoder
void runlength_decode(ac_channel<dtype> &in, ac_channel<dtype> &out){
	dtype in_num, in_cnt;

	if (in.available(2)){
		in_cnt = in.read();
		in_num = in.read();
	}

	while (in_cnt > 0) {
		out.write(in_num);
		std::cout << in_num << " ";
		--in_cnt;
	}
}
int main(){
	
	static const int NUM_SAMPLES=10;
	ac_channel<dtype> in_stream;
	ac_channel<dtype> out_stream;
	dtype data_out[10];

	// Give in stream the values 0123456789
	for (int i = 0; i < NUM_SAMPLES; ++i){
		in_stream.write(i);
	}	

	while (in_stream.available(1)){
		runlength_decode(in_stream, out_stream);
	}
	
	std::cout << std::endl;

	runlength_encode(out_stream,in_stream);
	runlength_encode(out_stream,in_stream);

	while (in_stream.available(1)){
		std::cout << in_stream.read() << " ";
	}

	std::cout << std::endl;
	return 0;
}
