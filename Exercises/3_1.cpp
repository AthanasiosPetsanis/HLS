#include <iostream>
#include <cstdlib>
#include <ctime>
#include "include/ac_channel.h"
#include "include/ac_int.h"


static const int W = 4;
typedef ac_int<W, false> dtype;

class RunLengthEncoder {
private:
 // internal state
 short cnt;
 dtype curr_val, prev_val;
public:
 // constructor - init internal state
 RunLengthEncoder();
 // top-level interface
 void run (ac_channel<dtype> &in, ac_channel<dtype> &out) {
 	if (in.available(1)) {
 		prev_val = in.read();
	 }
 	
 	// Only the 4 LSBs are written from the counter to the out (if cnt>15 we lose information)
 	while(1) {
 		if(in.available(1)){
	 		curr_val = in.read();
	 		if (curr_val == prev_val) {
	 			cnt++;
			 }
			 else {
			 	out.write(cnt);
			 	out.write(prev_val);
			 	prev_val = curr_val;
			 	cnt = 1;
			 }
	 	}
	 	else {
	 		out.write(cnt);
			out.write(prev_val);
			cnt = 1;
	 		break;
		 }
	 }
 }
};

RunLengthEncoder::RunLengthEncoder(){
	cnt = 1;
}


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
	ac_channel<dtype> in_stream;
	ac_channel<dtype> out_stream;
	dtype data_out[10];
	short val;
	
	// reseed RNG
	std::srand(std::time(NULL));
	
	for (int i=0; i<RUNS; i++) {
		
		RunLengthEncoder runlen;
		
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
		runlen.run(out_stream,in_stream);
		std::cout << " produces:\n";	
	
		while (in_stream.available(1)){
			std::cout << in_stream.read() << " ";
		}
	
		std::cout << "\n\n";
	}
	return 0;
}
