#include <iostream>
#include <map>
#include <cstring>

using namespace std;

/* Why do I have to do this? */
struct ltstr
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) < 0;
    }
};

int 
main (int argc, char* argv[]) 
{

    map<string, int> age;

    age["shaun"] = 23;
    age["mike"] = 30;

    cout << "Shaun! " << age["shaun"] << endl;
    map<string, int>::iterator cur = age.find("shaun");
    --cur;
    cout << "Next: " << cur->first << endl;

    return 0;

}

