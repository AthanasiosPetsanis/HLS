#include <iostream>
#include <cstdlib>
#include <ctime>
#include "include/ac_channel.h"
#include "include/ac_int.h"

typedef ac_int<4,false> Card;

class BlackJack {
private:
 // internal state
 Card card1;
 Card card2;
 short cnt;
 short sum;
public:
 // constructor - init internal state
 BlackJack(){};
 // top-level interface
 void run (ac_channel<Card> &in_card, bool &end_round, bool &win) {
 	cnt = 2;
 	sum = 0;
 	card1 = in_card.read();
 	card2 = in_card.read();
 	std::cout << "First 2 cards drawn: " << card1 << " and " << card2 << ". Current sum = " << card1+card2 << std::endl; //Here the displayed sum counts the aces as 1.
 	if ((card1 == card2) && (card1 == 1)) {
 		win = true; end_round = true;
 		std::cout << "2 ACES! LUCKY WINNER!\n\n"; 		
	 }
	 else {
	 	end_round = false;
	 	// Ace will always mean 11
		if (card1 == 1){
			card1 = 11;
		}
		else if (card2 == 1) {
			card2 = 11;
		}
		sum = card1 + card2;

	 	while (end_round == false) {

	 			if (sum==21) {
	 				win = true; end_round = true;
	 				std::cout << "21! WE HAVE A WINNER!\n\n";
				 }
				 else if (sum<21) {
				 	if (!in_card.available(1)) {
				 		std::cout << "No more cards in the deck!\n\n";
				 		end_round = true;
					 }
					 else if (cnt==5) {
			 			win = true; end_round = true;
			 			std::cout << "YOU DREW 5 CARDS WITHOUT GETTING BURNED! YOU WIN!\n\n";
			 		 }
					 else {
				 	end_round = false;
				 	card2 = in_card.read();
				 	std::cout << "Drew 1! Daring! (The card drawn was:  "  << card2;
				 	if (card2 == 1) {
						card2 = 11;
				   	 }
				 	sum += card2;
				 	cnt++;
				 	std::cout << ", current sum=" << sum <<")\n";
				     }
				 }
				 else {
				 	win = false; end_round = true;
				 	std::cout << "YOU GOT BURNED!\n\n";
				 }
		 }
	 }
 }
};


int main(){
	static const short ROUNDS = 9;  // Rounds played
	static const int TIMES = 10000; // Shuffle times
	BlackJack jack;
	ac_channel<Card> in_card;
	bool end_round, win;
	Card deck[52], temp;
	short spot1, spot2;
	
	
	// reseed RNG
	std::srand(std::time(NULL));
	
	// Create deck
	for (int num=0; num<13; num++) {
		if (num<9) {
			for (int i=0; i<4; i++) {
				deck[i+num*4] = num+1;
			}
		}
		else {
			for (int i=0; i<4; i++) {
				deck[i+num*4] = 10;
			}
		}
	}
	
	// Shuffle deck randomly
	for (int i=0; i<TIMES; i++) {
		spot1 = rand() % 52;
		spot2 = rand() % 52;
		temp = deck[spot1];
		deck[spot1] = deck[spot2];
		deck[spot2] = temp;
	}
	
	
	std::cout << "The following cards were drawn with order from left to right:\n";
	// Show deck
	for (int i=0; i<52; i++) {
		std::cout << deck[i] << " ";
		in_card.write(deck[i]);
	}
	std::cout << std::endl;
	
	// Play rounds (call fucntion)
	for (int i=0; i<ROUNDS; i++){
		std::cout << "Round " << i << ". ";
		jack.run(in_card, end_round, win);
	}

	return 0;
}
