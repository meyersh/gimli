#include <iostream>
#include "clauses.hpp"

using namespace std;

int main()
{
   int r = 2;
   pretty_print_vector(G1a(r), "G1_a");
   pretty_print_vector(G1b(r), "G1_b");
   pretty_print_vector(G2a(r), "G2_a");
   return 0;
}
