#ifndef __CLAUSES_HPP__
#define __CLAUSES_HPP__
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#define WIDTH 5

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

   if (desc != "")
	  std::cout << desc << " = {" << std::endl;

   for (int i = 0; i < v.size(); i++)
	  {
	  if (i && i  % WIDTH == 0)
		 std::cout << std::endl;
	  std::cout << std::setw(20) << std::setfill(' ') << v[i];
	  if (i != v.size() - 1)
		 std::cout << ", ";
		
	  }

   if (desc != "")
	  std::cout << "}";

   std::cout << std::endl;
}

std::vector<std::string> G1a(int r)
/* G1a: {Q[i,0], Q[i,1], ... , Q[i,r]}, 0<=i<=p(n)
   G1b: {!Q{i,j], !Q[i,j'}, 0<= i <= p(n), 0<= j < j' <= r */
{
   std::vector<std::string> g1a;

   for (int i = 0; i <= p(n); i++)
	  for (int s = 0; s <= r; s++)
		 {
		 std::stringstream ss;
		 ss << "Q[" << i << "," << s << "]";
		 g1a.push_back(ss.str());
		 }
   
   return g1a;
}

std::vector<std::string> G1b(int r)
/* G1a: {Q[i,0], Q[i,1], ... , Q[i,r]}, 0<=i<=p(n)
   G1b: {!Q{i,j], !Q[i,j'}, 0<= i <= p(n), 0<= j < j' <= r */
{
   std::vector<std::string> g1b;

   for (int i = 0; i <= p(n); i++)
	  for (int j=0, jp = 1; j < jp && jp <= r; j++, jp++)
		 {
		 std::stringstream ss;
		 ss << "{!Q[" << i << "," << j << "], " << "!Q[" << i << "," << jp << "]}";
		 g1b.push_back(ss.str());
		 }

   return g1b;

}

std::vector<std::string> G2a(int r)
/* G2a: {H[i, -p(n)], H[i, -p(n)+1], ..., H[i,p(n)+1]}, 0 <= i <= p(n)
   G2b: {!H[i,j], !H[i,j']}, 0<= i <= p(n), -p(n) <= j < j' <= p(n) +1
*/
{
   std::vector<std::string> g2_a;
   for (int i = 0; i <= p(n); i++)
	  for (int n = -p(n); n <= p(n) + 1; n++)
		 {
		 std::stringstream ss;
		 ss << "H[" << i << ", " << n << "]";
		 g2_a.push_back(ss.str());

		 }

   return g2_a;
}
#endif
