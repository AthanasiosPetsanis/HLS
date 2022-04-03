#include <iostream>
#include <cstdlib>
#include <ctime>

unsigned short int checksum(unsigned int num){
	
	short cnt = 1;
	unsigned int ten_pow = 10;
	unsigned short int sum = 0;
	short stop = 0;

	unsigned int tenth_digit;
	
	while (stop < 2) {
		if (num%ten_pow == num){
			stop++;
		}
		
		tenth_digit = num%ten_pow / (ten_pow/10);
		if (cnt%2 == 0) {
			short tenth_digit2 = 2*tenth_digit;
			if (tenth_digit > 4) {
				sum += tenth_digit2 % 10 + tenth_digit2 / 10;
			}
			else {
				sum += tenth_digit2;
			}
		}
		else {
			sum += tenth_digit;
		}
		ten_pow *= 10;
		cnt++;
	}
	return sum;
}

int main(){
	
	unsigned int num;
	unsigned short int result;
	static const int RUNS = 10;
	
	// reseed RNG
	std::srand(std::time(NULL));
	
	for (int i=0; i<RUNS; i++) {
	  	num = rand(); // rand of rand is [0,32767]
	  	result = checksum(num);
		std::cout << "The checksum of the number " << num << " is: " << result << "\n\n";
	}

	return 0;
}
