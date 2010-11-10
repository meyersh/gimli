#include <iostream>
#include <pwd.h>

using namespace std;

int
main(int argc, char* argv[])
{
    const char* username;

    if ( argc > 1 )
    {
        username = argv[1];
    }

    if (passwd* user = getpwnam( username ))
    {

        cout << user->pw_name << endl
             << user->pw_gecos << endl
             << user->pw_uid << endl
             << user->pw_dir << endl
             << user->pw_shell << endl;
    } 
    else
    {
        cout << "User does not Exist!\n";
    }

    return 0;
}
