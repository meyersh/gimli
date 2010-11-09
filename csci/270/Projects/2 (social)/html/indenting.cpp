#include <iostream>
#include <string>
#include <sstream>

using namespace std;

string indent(string, int);

int indentlevel = 0;

int
main()
{
  /* Desired output:
     <ul>
       <li>blah</li>
       <ul>
           <li>blah</li>
       </ul>
     </ul> 
  */
  int children = 3;

  for (int i = 0; i < children; i++)
    cout << indent("blah", i);

  cout << endl;

  return 0;

}

string indent(string message, int i)
{
  stringstream ss;
  if ( i == 0 )
    ss << "<ul>\n";
  if ( i < indentlevel )
    ss << "</ul>\n";
  if ( i > indentlevel )
    ss << "<ul>\n";
  if ( i == indentlevel )
    ss << "<li>blah!</li>\n";
  
  indentlevel = i;
  
  return ss.str();
  
}
