#include <time.h>
#include <iostream>

using namespace std;

int main()
{
time_t rawtime; 
struct tm * timeinfo;

time(&rawtime);
timeinfo = localtime( &rawtime );
cout << asctime(timeinfo) ;
return 0;
}

