#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

/* String comparison */

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

    map< string, vector<string> > age;

    age["shaun"].insert("twenty three");
    age["mike"].insert("thirty");

    cout << "Shaun! " << age["shaun"].begin() << endl;
    /* map<string, int>::iterator cur = age.find("shaun");
    --cur;
    cout << "Next: " << cur->first << endl; */

    return 0;

}

