#include <iostream>
#include "include/ac_int.h"
#include "include/ac_fixed.h"




int main(){
	ac_int<8,false> a, b; // 8-bit unsigned integer
	ac_int<9,true> c, d; // 9-bit signed integer
	
	ac_int<16,false> ab;
	ac_int<17,true> cd;
	
	ac_int<18,true> e; 


	
	// We need enough bits to store the biggest possible number
	a = 255; b = 255; ab = a*b; // Biggest number for a*b is 65.025 therefore at least 16 bits
	
	c = 255; d = 255; cd = c*d; // Biggest number for c*d is 65.025 therefore at least 16+1(for sign)=17 bits
	e = a*b + c*d;	// How many bits
	std::cout << "A: " << a << " , bitwise: " << a.to_string(AC_BIN, false) << std::endl;
	std::cout << "B: " << b << " , bitwise: " << b.to_string(AC_BIN, false) << std::endl;
	std::cout << "A*B: " << ab << " , bitwise: " << ab.to_string(AC_BIN, false) << "\n\n";
	std::cout << "C: " << c << " , bitwise: " << c.to_string(AC_BIN, true) << std::endl;
	std::cout << "D: " << d << " , bitwise: " << d.to_string(AC_BIN, true) << std::endl;
	std::cout << "C*D: " << cd << " , bitwise: " << cd.to_string(AC_BIN, true) << "\n\n";
	std::cout << "E: " << e << " , bitwise: " << e.to_string(AC_BIN, true) << "\n\n";
	
	// Any less than 18 bits would mean we lose information
	// (MSB being the sign-bit gets cut. It's place takes the next being 1 so the number is translated to negative, by 1's complement. )
	ac_int<17,true> k;
	k = a*b + c*d;
	std::cout << "K (17-bit e): " << k << " , bitwise: " << k.to_string(AC_BIN, true) << "\n\n";

	

	ac_fixed<5,2,false> f; //5-bit unsigned fixed-point type with 2 integer bits
	ac_fixed<5,4,true> g; // 5-bit signed fixed-point type with 4 integer bits
	ac_fixed<10,6,true> h; 
	
	f = 3.875; // Biggest number. 0.875=0.5+0.25+0.125 for decimal, 3=1+2 for non-decimal
	g = 7.5;   // Biggest number. 0.5 for 1-bit decimal. 7=4+2+1 for 3-bits non-decimal (1 bit for sign)
	h = f*g;   // How many bits. Larget number 3.875*7.5=29.0625 therefore 5+1(for sign) bits non-decimal, 4 bits decimal (2^-4 = 0.0625)
	std::cout << "F: " << f << " , bitwise: " << f.to_string(AC_BIN, false) << std::endl;
	std::cout << "G: " << g << " , bitwise: " << g.to_string(AC_BIN, true) << std::endl;
	std::cout << "H: " << h << " , bitwise: " << h.to_string(AC_BIN, true) << std::endl;
	
	// Any less that that in decimal or non-decimal would mean we would lose information
	ac_fixed<9,6,true> p; // 3 decimals
	p = f*g;
	std::cout << "\nP: " << p << " , bitwise: " << p.to_string(AC_BIN, true) << std::endl; // (MSB for decimals cut)

	ac_fixed<9,5,true> i; // 5 non-decimals
	i = f*g;
	std::cout << "\nI: " << i << " , bitwise: " << i.to_string(AC_BIN, true) << std::endl; // (MSB being the sign-bit gets cut. It's place takes the next being 1 so the number is translated to negative, by 1's complement. )
	return 0;
}
