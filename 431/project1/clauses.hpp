#ifndef __CLAUSES_HPP__
#define __CLAUSES_HPP__
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath> // for floor

#include "dtm.hpp" // for stats structure.

#define WIDTH 5
#define SCREEN_WIDTH 80 // only 80-character wide terminal.

/* Q[i,j] variable:
   0<=i<=p(n), 0<=k<=r
   r is number of states.
   At time i, M is in state Q_k.
*/

int n = 5;

int p(int n)
{
   return 5;
}

void pretty_print_vector(std::vector<std::string> v, std::string desc="") 
{
   int w = 0; // the width of our widest field
   int width = 0; // where to wrap.

   for (int i = 0; i < v.size(); i++)
	  if (v[i].length() > w)
		 w = v[i].length();

   w += 4; // add a little room on either side.
   
   width = 80 / (w+2);

   if (desc != "")
	  std::cout << desc << " = {" << std::endl;

   for (int i = 0; i < v.size(); i++)
	  {
	  if (i && i % width == 0)
		 std::cout << std::endl;
	  std::cout << std::setw(w) << std::setfill(' ') << v[i];
	  if (i != v.size() - 1)
		 std::cout << ",";
		
	  }

   if (desc != "")
	  std::cout << "}";

   std::cout << std::endl;
}

std::vector<std::string> G1a(const Stats &stats)
/* G1a: {Q[i,0], Q[i,1], ... , Q[i,r]}, 0<=i<=p(n)
   G1b: {!Q{i,j], !Q[i,j'}, 0<= i <= p(n), 0<= j < j' <= r */
{
   std::vector<std::string> g1a;

   for (int i = 0; i <= stats.pn; i++)
	  for (int s = 0; s <= stats.r; s++)
		 {
		 std::stringstream ss;
		 ss << "Q[" << i << "," <<  s << "]";
		 g1a.push_back(ss.str());
		 }
   
   return g1a;
}

std::vector<std::string> G1b(const Stats &stats)
/* G1a: {Q[i,0], Q[i,1], ... , Q[i,r]}, 0<=i<=p(n)
   G1b: {!Q{i,j], !Q[i,j'}, 0<= i <= p(n), 0<= j < j' <= r */
{
   std::vector<std::string> g1b;

   for (int i = 0; i <= stats.pn; i++)
	  for (int j=0; j < stats.r; j++)
		 {
		 std::stringstream ss;
		 ss << "{!Q[" << i << "," << j << "], " << "!Q[" << i << "," << (j+1) << "]}";
		 g1b.push_back(ss.str());
		 }

   return g1b;

}

std::vector<std::string> G2a(const Stats &stats)
/* G2a: {H[i, -p(n)], H[i, -p(n)+1], ..., H[i,p(n)+1]}, 0 <= i <= p(n)
   G2b: {!H[i,j], !H[i,j']}, 0<= i <= p(n), -p(n) <= j < j' <= p(n) +1
*/
{
   std::vector<std::string> g2_a;
   for (int i = 0; i <= stats.pn; i++)
	  for (int n = -stats.pn; n <= stats.pn + 1; n++)
		 {
		 std::stringstream ss;
		 ss << "H[" << i << ", " << std::setw(2) << n << "]";
		 g2_a.push_back(ss.str());

		 }

   return g2_a;
}


std::vector<std::string> G2b(const Stats &stats)
/* G2a: {H[i, -p(n)], H[i, -p(n)+1], ..., H[i,p(n)+1]}, 0 <= i <= p(n)
   G2b: {!H[i,j], !H[i,j']}, 0<= i <= p(n), -p(n) <= j < j' <= p(n) +1
*/
{
   std::vector<std::string> g2b;

   for (int i = 0; i <= stats.pn; i++)
	  for (int j=-stats.pn, jp = j+1; j < jp && jp <= stats.pn+1; j++, jp++)
		 {
		 std::stringstream ss;
		 ss << "{!H[" << i << "," << j << "], " << "!H[" << i << "," << jp << "]}";
		 g2b.push_back(ss.str());
		 }

   return g2b;

}


std::vector<std::string> G3a(const Stats &stats)
/* 
   r = states - 1, v = input symbols - 1
   G3a: {S[i,j,n], S[i,j,n+1], ... , S[i,j,v]}, 0 <= i <= p(n), -p(n) <= j <= p(n)+1
   G3b: {!S[i,j,n], !S[i,j',n']}, 0 <= i <= p(n), -p(n) <= j <= p(n)+1, 0 <= n < n' <= v
*/
{

   std::vector<std::string> g3_a;
   for (int i = 0; i <= stats.pn; i++)
	  for (int n = -stats.pn; n <= stats.pn + 1; n++)
		 {
		 std::stringstream ss;
		 ss << "S[" << i << ", " << std::setw(2) << n << "]";
		 g3_a.push_back(ss.str());

		 }

   return g3_a;
}


std::vector<std::string> G3b(const Stats &stats)
/* G3a: {S[i,j,n], S[i,j,n+1], ... , S[i,j,v]}, 0 <= i <= p(n), -p(n) <= j <= p(n)+1
   G3b: {!S[i,j,n], !S[i,j',n']}, 0 <= i <= p(n), -p(n) <= j <= p(n)+1, 0 <= n < n' <= v
*/
{
   std::vector<std::string> g3b;

   for (int i = 0; i <= stats.pn; i++)
	  for (int j=-stats.pn, jp = j+1; j < jp && jp <= stats.pn+1; j++, jp++)
		 {
		 std::stringstream ss;
		 ss << "{!H[" << i << "," << j << "], " << "!H[" << i << "," << jp << "]}";
		 g3b.push_back(ss.str());
		 }

   return g3b;

}









std::vector<std::string> G5a(const Stats &stats)
/* Q[p(n), 1] */
{
   std::vector<std::string> ret;
   std::stringstream ss;

   ss << "Q[" << stats.pn << ", 1]";

   ret.push_back(ss.str());

   return ret;
}


#endif
