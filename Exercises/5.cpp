#include <iostream>
#include <cstdlib>
#include <ctime>
#include "include/ac_int.h"

static const short N = 8;

static const short RUNS = 10;

static const short size = 5;

typedef ac_int<N, false> matrix;

void graph_color(matrix Adj_G[N], short chroma[N], short &color_num) {
	
	bool same_c;
	short c_cnt = 1;
	for (int v=0; v<size; v++) {
	 chroma[v] = 1;
	 same_c = true;
	 while (same_c == true) {
		 for (int i=0; i<size; i++) {
			 if (Adj_G[v][i]==1 && chroma[v]==chroma[i]){
				 same_c = true;
				 chroma[v]++;
				 break;
			 }
			 else {
				 same_c = false;
			 }
			}
		}
	if (chroma[v] > c_cnt) {
		c_cnt = chroma[v];
	}
	}
	color_num = c_cnt;
}

int main () {
  matrix adj_g[N];
  short chroma[N];
  short c_num;
 
  // reseed RNG
  std::srand(std::time(NULL));
 
  // run RUNS tests
  for (int k=0; k < RUNS; k++) {
    std::cout << "Trial " << k << "\n";

    // Initialize matrices. Adj_G will have random 0 or 1 elements besides. Chroma will have elements equal to 0.
    for (int i=0; i < N; i++) {
      for (int j=i; j < N; j++) {
		  if (i<size && j<size && j!=i) {
			  
			  adj_g[i][j] = std::rand() % 2;
			  adj_g[j][i] = adj_g[i][j];
			}
		  else {
			  adj_g[i][j] = 0;
			  adj_g[j][i] = 0;
			}
		}
	  chroma[i] = 0;
    }
	
	// Show matrix
    for (int i=0; i < N; i++) {
      for (int j=0; j < N; j++) {
		  std::cout << adj_g[i][j] << "\t";
		}
      std::cout << "\n";
    }
	
    // call graph_color
    graph_color(adj_g, chroma, c_num);

    // print out colors of vertices and color number
    std::cout << "The colors of the vertices are: ";
    for (int i=0; i < N; i++) {
      std::cout << chroma[i] << " ";
    }
    std::cout << "\n";
	std::cout << "The color number is: " << c_num << "\n\n";
  }
  
  return 0;
} 

