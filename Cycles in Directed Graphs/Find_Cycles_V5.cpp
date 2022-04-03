#include <cstdlib>
#include <ctime>
#include "include/ac_int.h"
#include "include/ac_channel.h"
#include "include/mc_scverify.h"
#include "include/data_types.h"

static const short RUNS = 10;

template<typename T>
class Stack {
	private:
		T mem[size];
		T cnt;
	public:
		Stack() {
			ac::init_array<AC_VAL_0>(mem, size);
			mem[0] = 1;
			cnt = 0;
		}
		void push(node_bits x){
			cnt += 1;
			mem[cnt] = x;
		}
		void pop(){
			mem[cnt] = 0;
			cnt -= 1;
		}
		T read(node_bits k) {
			return mem[k];
		}
		T top() {
			return mem[cnt];
		}
		void swap(node_bits x){
			mem[cnt] = x;
		}
};

template<typename T>
class Visited {
	private:
		bool mem[size];
	public:
		Visited(){
			ac::init_array<AC_VAL_0>(mem, size);
			mem[0] = 1;
		}
		void write(T x) {
			mem[x] = 1;
		}
		bool read(T x){
			return mem[x];
		}
};

void go_back(node_bits &n, node_bits &next_c, Stack<node_bits> &stack, Visited<node_bits> &visited){
	
	// We only want to go back to stack if there is a past node 
	// AND if we reach the end of the node/line AND if we didn't jump to another node!
	// Last 2 statements are true because we're outside the inner/row loop.
	if (stack.read(1)!=0) {
		// Pop stack & go back
		next_c = stack.top();
		stack.pop();
	 	n = stack.top()-2;	 	
	}
	// If there is only 1 node left either it had noone to jump to OR we reached the end of the algorithm.
	else {
		SCAN_NODES: for (node_bits k=stack.read(0); k<size; k++) {
			// if we missed visiting a node we jump to it.
			if (visited.read(k)==0) {
				// Visit the unvisited node.
				n = k-1;
				next_c = 0;
				// We leave current node and replace it with unvisited node.
				stack.swap(k+1);
				
				k = size; // break; replacement
			}
			else {
				n = size; // break; replacement of outer loop. End the algorithm.
			}
		}
	}
}

void check_for_cycle (node_bits &n, node_bits &c, Stack<node_bits> &stack,
 Visited<node_bits> &visited, ac_channel<node_bits> &cycles, node_bits size1){
 	
 	 	
	SCAN_STACK: for (node_bits k=0; k<size; k++){
		// If we find a cycle
		if (stack.read(k)==c+1){
			RW_CYCLE: for (k; k<size && stack.read(k)!=0; k++) {
				// Produce an output.
				cycles.write(stack.read(k));
				
			}
			cycles.write(0);
			
			k = size; // break; replacement
			
		}
		// Moved inside loop. Was done right after it.
		// Go to node only if we didn't find a cycle.
		else if (k==size1) {
			
			// Push to stack the node
			stack.push(c+1);
			visited.write(c);
			
			// Jump to node
			n = c;
			c = -1;
		}		
    }
}
#pragma hls_design top
void CCS_BLOCK(find_cycles)(matrix graph[size], ac_channel<node_bits> &cycles) {
	
	// Local Variables
	node_bits k,n,c,next_c = 0;
	node_bits size1 = size-1;
	Visited<node_bits> visited;
	Stack<node_bits> stack;
		
	// Self-cycles/Diagonal values
	DIAG: for (k=0; k<size; k++){
		if (graph[k][k]==1) {
			cycles.write(k+1);
			cycles.write(0);
		}
	}
	
	ROW: for (n=0; n<size; n++) {
		
		COL: for (c=next_c; c<size; c++) {
			// This if protects from re-reading diagonal values
			if (n!=c) {	
				if (graph[n][c]==1) {
					
					// Check if we cycled
					check_for_cycle (n, c, stack, visited, cycles, size1);		
				}
			}
		}
		
		go_back(n, next_c, stack, visited);
	}
}

CCS_MAIN(int argc, char* argv[]) {

  	matrix graph[size];
	ac_channel<node_bits> cycles;
	node_bits temp;
	node_bits size = 4;
	
	ac::init_array<AC_VAL_0>(graph, size);
	 
	// reseed RNG
	std::srand(std::time(NULL));
	 
	// run RUNS tests
	for (int k=1; k < RUNS+1; k++) {
		std::cout << "Trial " << k << "\n";
	
		// Initialize matrices.
		for (int i=0; i < size; i++) {
	        for (int j=0; j < size; j++) {
			  graph[i][j] = std::rand() % 2;
			}
	    }
	    		
		// Show matrix
	    for (int i=0; i < size; i++) {
	        for (int j=0; j < size; j++) {
			  std::cout << graph[i][j] << "\t";
			}
	    	std::cout << "\n";
	    }
		
	    // call graph_color
	    find_cycles (graph, cycles);
	    
	    std::cout << "\nOutput stream: ";
	    while (cycles.available(1)) {
	    	temp = cycles.read();
	    	std::cout << " " << temp;
		}
		std::cout << "\n\n";
	
	 }

 /*
  node_bits temp;
  matrix graph[size];
  ac_channel<node_bits> cycles;
  
  graph[0][0] = 1; graph[0][1] = 1; graph[0][2] = 0; graph[0][3] = 1;
  graph[1][0] = 1; graph[1][1] = 0; graph[1][2] = 1; graph[1][3] = 0;
  graph[2][0] = 0; graph[2][1] = 0; graph[2][2] = 1; graph[2][3] = 1;
  graph[3][0] = 0; graph[3][1] = 1; graph[3][2] = 1; graph[3][3] = 1;

	// Show matrix
	for (int i=0; i < size; i++) {
		
	    for (int j=0; j < size; j++) {
		  std::cout << graph[i][j] << "\t";
		}
		std::cout << "\n";
	}
	    
  find_cycles (graph, cycles);

    std::cout << "\nOutput stream: ";
    while (cycles.available(1)) {
    	temp = cycles.read();
    	std::cout << " " << temp;
	}
	std::cout << "\n\n";
   */
  CCS_RETURN(0);
} 
