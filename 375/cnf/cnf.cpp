#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <set>
#include <map>
#include <iterator>
#include <assert.h>

using namespace std;

typedef unsigned long long int ull;


string tobinary(ull mask) {
    stringstream ss;
    
    for (int i = 0; i < 8*sizeof(mask); i++)
        ss << ((mask & (ull)1 << 8*sizeof(mask)-i-1) ? 1 : 0);
    
    return ss.str();
    
}

string masked_string (string s, unsigned long long int mask) {
    stringstream ss;
    
    for (int i = 0; i < s.length(); i++)
        if (mask & ((ull)1 << i))
            ss << s[i];
    
    return ss.str();
}

int main(int argc, char *argv[])
{
    

    map <string, set<string> > productions;
    productions["S"].insert("A");
    productions["S"].insert("B");
    productions["S"].insert("AS");

    productions["A"].insert("^");
    productions["A"].insert("B");

    productions["B"].insert("b");
    
    ull mask = 0b10000; // this is reversed. In this case, only the
                        // 5th letter is nullable...
    
    string s = "SHAUN";
    
    set<string> elp; // Eliminated lambda productions
    
    int c = pow(2, s.size()) -1;

    for (int i = 0; i < c; i++) 
        elp.insert(masked_string(s,i|mask));

    
    
    // print out the string

    cout << tobinary(mask) << endl;
    
    copy(elp.begin(), elp.end(), 
         ostream_iterator<string>(cout, " "));   
    cout << endl;

    /*
    for (int i = 0; i < 32; i++)
        cout << sizeof(ull)*8 << " " << tobinary(i) << endl;
    */
    return 0;
}
