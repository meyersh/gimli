/*****************************************************************************
 * S8.E1 - Characters & Character Strings
 * Shaun Meyer, Feb 19 2010
 ****************************************************************************/

#include <iostream>
#include <iomanip>

using namespace std;

/*************************************
 * int main() - Take cin string and
 * convert / output Hex/Ascii codes
 ************************************/

int
main()
{
    string foo;
    getline(cin, foo);

    // We're on the interwebs now!
    cout << "Content-Type: text/plain\n\n";

    cout << "notice=Length: " << foo.length() << endl;

    string::iterator it;

    cout << "hex=";
    for (it = foo.begin(); it < foo.end(); it++) 
    {
        cout << hex << "[" << static_cast<int>(*it) << "] ";
    }
	
    cout << endl;

    return 0;
    }
