#include <iostream>
#include <cstdlib>
#include <ctime>
#include "include/ac_channel.h"
#include "include/ac_int.h"


static const int W = 4;
typedef ac_int<W, false> dtype;


// Encoder
void runlength_encode(ac_channel<dtype> &in, ac_channel<dtype> &out){
	dtype data_in[10];
	dtype prev_data_in = 0;
	short int counter = 1;

	if (in.available(10)) {
		data_in[0] = in.read();
		std::cout << data_in[0];
		for (int i = 1; i < 10; ++i){
			data_in[i] = in.read();
			std::cout << " " << data_in[i] ;
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
	// 3 4 <encodes=decodes> 4 4 4  (instead of 3 4 <encodes=decodes> 3 3 3 3) ������ ��� ������ ����������� ����� ����.
	// ������ �� �������� �� �� ���� ���� ���� � ������ ���������� ��� ����� .read ���� decoder ��� ��� ����� .write ���� encoder 
	// ������ �� ��� ��������.
	
	static const int NUM_SAMPLES=10;
	static const int RUNS=10;
	ac_channel<dtype> in_stream, out_stream;
	dtype data_out[10];
	short val;
	
	// reseed RNG
	std::srand(std::time(NULL));
	
	for (int i=0; i<RUNS; i++) {
		int lul;
		std::cout << "Decoding the sequence ";
		// Give in stream the values 0123456789
		for (int j = 0; j < NUM_SAMPLES; ++j){
			val = rand()%15;
			in_stream.write(val);
			std::cout << val << " ";
		}	
		std::cout << " produces:\n";
		
		while (in_stream.available(1)){
			runlength_decode(in_stream, out_stream);
		}
		
		std::cout << std::endl;
		std::cout << "Encoding the sequence: ";
		runlength_encode(out_stream,in_stream);
		std::cout << " produces:\n";	
	
		while (in_stream.available(1)){
			std::cout << in_stream.read() << " ";
		}
	
		std::cout << "\n\n";
	}
	return 0;
}
