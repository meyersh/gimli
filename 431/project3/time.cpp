#include <iostream>
#include <sys/timeb.h>
#include <cmath>
//#include <rand>

using namespace std;

int main() {
    struct timeb t_start;
    ftime(&t_start);

    cout << t_start.millitm << endl;

    srand(t_start.millitm);

    for (int i = 0; i<25; i++)
        cout << log(rand())-20 << endl;
}
