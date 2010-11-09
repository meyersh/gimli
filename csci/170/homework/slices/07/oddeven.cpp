#include <iostream>

using namespace std;

int
main()
{
    int i;

    cout << "Content-Type: text/plain\n\n";

    cin >> i;

    if (i%2)
        cout << "Odd";
    else
        cout << "Even";

    return 0;
}
