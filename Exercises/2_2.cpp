#include <iostream>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include "include/ac_channel.h"
#include "include/ac_int.h"


static const short W = 8;
static const short Wout = 2*W;

//typedef ac_int<W, false> dtype;

template <int W>
struct CSD {
 ac_int<W,false> x_p;
 ac_int<W,false> x_n;
};

template <int W>
void csd_encode (ac_int<W,true> &num, CSD<W> &num_csd){
	bool prev, curr, isneg;
	short cnt = 1;
	ac_int<W,false> temp;
	
	isneg = num[W-1]==1;
	
	// In case num is negative maybe I cheated a bit. Pun intended.
	// Problem is it takes the 2's complement
	if (isneg) {
		num = -num;
	}
	num_csd.x_p = num;
	prev = num_csd.x_p[0];
	for (short i=1; i<W; i++) {
		curr = num_csd.x_p[i];
		if (curr==1 && prev==1){
			cnt++;
		}
		else if (curr==0 && prev==1 && cnt>1) {
			num_csd.x_p[i] = 1;
			for (short j=1; j<=cnt; j++){
				num_csd.x_p[i-j] = 0;
			}
			num_csd.x_n[i-cnt] = 1;
			cnt=1;
		}
		prev = num_csd.x_p[i];
	}
	if (isneg) {
		temp = num_csd.x_n;
		num_csd.x_n = num_csd.x_p;
		num_csd.x_p = temp;
		num = -num;
	}
}

ac_int<Wout,true> csd_mult (ac_int<W,true> &in, const CSD<W> &constant_csd){
	// in is the input value (non constant)
	// constant_csd is the CSD representation of a constant

	ac_int<Wout,true> sum, temp;
	bool isneg = in[W-1]==1;
	sum = 0;
	
	// In case in is negative maybe I cheated a bit. Pun intended.
	// Problem is it takes the 2's complement
	if (isneg) { 
		temp = -in;
	}
	else {
		temp = in;
	}

	for (short i=0; i<W; i++) {
		if (constant_csd.x_p[i] == 1) {
			sum += temp<<i;
		}
		else if (constant_csd.x_n[i] == 1) {
			sum -= temp<<i;
		}
	}
	if (isneg) {
		sum = -sum;
	}
	return sum;
}

int main(){
	static const int RUNS = 10;
	ac_int<W,true> num, in, limit;
	ac_int<Wout,true> mult;
	CSD<W> num_csd;
	
	//reseed RNG
	std::srand(std::time(NULL));
	
	
	limit = pow(2,W-1)-1;
	for (short i=0; i<RUNS; i++) {
		// Test csd_encode
		num_csd.x_n = 0;
		num_csd.x_p = 0;
		num = rand() % (limit*2) - limit;
		csd_encode<W> (num, num_csd);
		std::bitset<W> n_xp(num_csd.x_p);
		std::bitset<W> n_xn(num_csd.x_n);
		std::cout << "Num=" << num << ", bitwise: " << num.to_string(AC_BIN, true) << "\nBitwise: num_csd.x_p=" << n_xp << ", num_csd.x_n=" << n_xn << "\n";	
		
		//Test csd_mult
		in = rand() % (limit*2) - limit;
		mult = csd_mult(in, num_csd);
		std::cout << in << " * " << num << " = " << mult << "\n\n";
	}
	


	return 0;
}
