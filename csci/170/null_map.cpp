#include <iostream>
#include <map>
#include <string>

using namespace std;

int main()
{
map<string, string> foo;
foo["DOESNOTEXIST"] = "FALSE";
if (foo.find("DOESNOTEXIST") != foo.end())
cout << "FOUND.\n";
else
cout << "NOT FOUND.\n";
}
