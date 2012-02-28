#ifndef __WLM_HXX__
#define __WLM_HXX__

/*
 * Weighted Learning Machine . hxx
 * Shaun Meyer, Mar 2012
 *
 *
 * Where in RL, V is our PERFECT function, and \hat{v} is an approximation
 * we shall consider V to be the best we can do.
 * 
 * It is made up of weights, such as w0 + x1w1 + xiwi + x(i+1)w(i+1) + ... + xnwn.
 * which are determined by the least mean squares method:
 * 
 * (Vtrain(b) - V(b))^2; 
 * E(rror) => wi <- wi + n(Vtrain(b) - V(b))*Xi
 *
 * Successor(b) reveals the n+1 state in the game-trace.
 *
 * Game state: A game has a state, a set of attributes which we care about, 
 * that the V*() functions will use: Careful to distinguish it from the
 * current game configuration (eg, state is the number of black pieces while
 * configuration is their positions on the board).
 * 
 */


#include <vector>

using namespace std;

struct State 
{

   vector<int> x;

   State(initializer_list<int> initial_values) {
	  vector<int> new_x = initial_values;
	  x = new_x;

   }

   
};

int V(State b) {

   return -1;

}

#endif
