#include <iostream>
#include <bitset>
#include <math.h>
#include <cstdlib>
#include <ctime>

unsigned short int numof_1bits(unsigned int var){
	short int bits = 8*sizeof(var);
	unsigned int div;
	unsigned short int counter = 0;
	for (int i = bits; i > 0; --i) {
		div = var/pow(2,i-1);
		counter += div;
		var -= div*pow(2,i-1);
	}
	return counter;
}

int main(){
	unsigned int var;
	unsigned short int result;
	unsigned short int RUNS =10;
	unsigned int max_num = 4294967295;
		
	// reseed RNG
	std::srand(std::time(NULL));
	
	std::cout << "The maximum number is: " << max_num << std::endl;
	for (int i=0; i<RUNS; i++) {
	  	var = rand() * rand() * (rand() % 3 + 1); // Any number between 0-max_num because rand() range is [0,32767]  (max_num = 32767*32767*4)
	  	result = numof_1bits(var);
		std::bitset <32> x(var);
		std::cout << "The number of 1s in the bit sequence of the number " << var << " or in binary form " << x << " is: " << result << "\n\n";
	}

	return 0;
}
