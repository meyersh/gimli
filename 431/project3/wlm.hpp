#ifndef __WLM_HPP__
#define __WLM_HPP__

#define WEIGHTS_FILE "weights.txt"

/*
 * Weighted Learning Machine . hpp
 * Shaun Meyer, Mar 2012
 *
 *
 * Where in RL, V is our PERFECT function, and \hat{v} is an approximation
 * we shall consider V to be the best we can do.
 * 
 * It is made up of weights, such as
 *  w0 + x1w1 + xiwi + x(i+1)w(i+1) + ... + xnwn.
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
#include <fstream>
#include <sstream>
#include <stdexcept>


using namespace std;

/* 
 * State class
 *
 * A thin wrapper around a vector of ints.
 *
 * Defaults to size 0, allows insert(int).
 *
 * Overrides subscript operator for direct access
 * and default constructor may take a length.
 */

struct State 
{

   vector<int> x;

   State (unsigned int);

   /* Prototypes */
   int size() {return x.size();};
   int& operator[] (const int);
   void insert(int);
   string toString();

   
};

/* State Object functions */

State::State(unsigned int len=0) {
   if (len)
	  x.resize(len);
}

void State::insert(int i) {
   x.push_back(i);
}

int& State::operator[] (const int i) {
   return x[i];
}

string State::toString() {
   stringstream ss;

   for (int i = 0; i < x.size(); i++)
	  {
	  ss << x[i];
	  if (i+1 != x.size())
		 ss << ", ";
	  }

   return ss.str();
}

/*
 * Weight class 
 */

struct Weight
{

   vector<int> w;
   string filename;

   Weight(string);
   ~Weight();
   int size() {return w.size();};
   int& operator[] (const int);
   int save();
   int load(); 
   void insert(int i);
   string toString();
};

/* Weight Object functions */

Weight::Weight(string filename) {
   this->filename = filename;
   load();
}

Weight::~Weight() {
   save();
}

int& Weight::operator[] (const int i) {
   return w[i];
}

int Weight::save() {
   ofstream weight_file(filename.c_str());

   if (!weight_file.good()) 
	  return 1; // can't save the file!

   for (int i = 0; i < w.size(); i++)
	  weight_file << w[i] << endl;
   
   weight_file.close();
   return 0;
}

int Weight::load() {
   ifstream weight_file(filename.c_str());

   string line;

   if (!weight_file.good())
	  {
	  weight_file.close();
	  return 1; // can't open the file! 
	  }

   w.clear();

   while (getline(weight_file, line))
	  w.push_back( atoi(line.c_str()) );

   weight_file.close();
   return 0;
}

void Weight::insert(int i) {
   w.push_back(i);
}

string Weight::toString() {
   stringstream ss;

   for (int i = 0; i < w.size(); i++)
	  {
	  ss << w[i];
	  if (i+1 != w.size())
		 ss << ", ";
	  }

   return ss.str();
}
	  

int Vhat(State b) {
   Weight w(WEIGHTS_FILE);

   // empty w or b is exceptional.
   if (!b.size() || !w.size())
	  throw logic_error("Weight or B is empty.");

   // We have dynamic-length state & weight,
   // calculate for the shorter.
   int len = 0;

   if (b.size() < w.size())
	  len = b.size();
   else 
	  len = w.size() - 1;


   // Implicit w0 var.
   int result = w[0];
   
   // Multiply up the rest of the xi*wi vars.
   for (int i = 0; i < len; i++)
	  result += b[i] * w[i+1];

   return result;

}



#endif
