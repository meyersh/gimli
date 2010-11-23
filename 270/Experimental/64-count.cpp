#include <iostream>
#include <iomanip>

using namespace std;

typedef unsigned long long int ulli ;

int main()
{
  ulli pos = 1;
  const ulli max = 0xFFFFFFFFFFFFFFFF;
  for (ulli i = 0; i < max; i++)
    if (pos & i) {
      pos <<= 1;
      cout << hex << setw(16) << i << " / " << max << endl;
    }

  cout << "Ding!\n";
}
