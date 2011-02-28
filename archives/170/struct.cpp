#include <string>
#include <iostream>

using namespace std;

struct person {
  string name;
  double height;
  void init ()
  {
    height = 2;
  }
  void hi ()
  {
    cout << "Hello, " << name << endl;
  }
  void height(double &height=height)
  { 
   cout << "Height is: " << height << endl;
  }

};

int 
main()
{
  person Shaun;
  Shaun.name = "Shaun Meyer";
  Shaun.hi();

  return 0;
}
