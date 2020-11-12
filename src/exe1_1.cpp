#include <iostream>
#include <math.h>

unsigned short int numof_1bits(unsigned int var){
	short int bits = 8*sizeof(var);
	std::cout << "The size of the number you entered is: " << bits << " bits." << std::endl;
	unsigned int div;
	unsigned short int counter = 0;
	for (int i = bits; i > 0; --i) {
		div = var/pow(2,i-1);
		counter += div;
		var -= div*pow(2,i-1);
		std::cout << "The div result is: " << div << "." << std::endl;
		std::cout << "The counter value is: " << counter << "." << std::endl;
	}
	return counter;
}

int main(){
	unsigned int var = 19; 
	unsigned short int result = numof_1bits(var);
	std::cout << "The number of 1s in the bit sequence of the number " << var << " is: " << result << std::endl;
	return 0;
}
