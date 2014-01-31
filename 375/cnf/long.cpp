#include <iostream>
#include <sstream>

using namespace std;

string tobinary(unsigned long long int mask) {
    stringstream ss;
    
    for (int i = 0; i < 8*sizeof(mask); i++)
        ss << ((mask & ((unsigned long long int)1 << (8*sizeof(mask))-i-1)) ? 1 : 0);
    
    return ss.str();
    
}

int main(int argc, char *argv[])
{
    
    // This can be `int i`, but I wanted to be sure...
    for (unsigned long long int i = 0; i < 32; i++)
        cout << sizeof(i)*8 << ": " << tobinary(i) << endl;

    return 0;
}
