#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "adjMatrix.hpp"

#define MAX_CITIES 1024

enum {
  OK,
  INVALID_CITY, 
  INVALID_DISTANCE,
  INVALID_FILE
};

using namespace std;

bool isalpha(string str)
{
  for (int i = 0; i < str.size(); i++)
    if (!isalpha(str[i]))
      return false;

  return true;
}

bool isdigit(string str)
{
  for (int i = 0; i < str.size(); i++)
    if (!isdigit(str[i]))
      return false;
  return true;
}

/*
 * Read the appropriate file and populate the vector & adjmatrix
 */

int read_cities_file(ifstream &file, vector<string> &cities, adjMatrix &city_matrix)
{
  string snum_cities;
  int num_cities = 0;
  file >> snum_cities;
  if (isdigit(snum_cities))
    num_cities = atoi(snum_cities.c_str());
  else
    return INVALID_FILE;

  if (num_cities > MAX_CITIES)
    return INVALID_FILE;

  /* read city names */

  string cur_city;
  for (int i = 0; i < num_cities; i++)
    {
      if (!file.good())
	return INVALID_FILE;
      file >> cur_city;
      /* ERROR CHECK cur_city */
      if (!isalpha(cur_city))
	{
	  cout << "Invalid city: " << cur_city << endl;
	  return INVALID_CITY;
	}
      cities.push_back(cur_city);
    }

  /* read city distances as a string for error checking */
  string distance;
  city_matrix.resize_matrix(num_cities);
  for (int y = 1; y < num_cities; y++)
    {
      for (int x = 0; x < y; x++)
	{
	  if (!file.good())
	    return INVALID_FILE;
	  file >> distance;
	  if (!isdigit(distance))
	    {
	      cout << "Invalid distance at " << distance << endl;
	      return INVALID_DISTANCE;
	    }
	  city_matrix.edge(x, y) = city_matrix.edge(y, x) = atoi(distance.c_str());
	}
    }
  
return 0;
}

string str_to_upper(const string str)
{
  string ret = "";
  for (int i = 0; i < str.size(); i++)
    ret += std::toupper(str[i]);
  return ret;
}

string str_to_proper(const string str)
{
  string ret;
  ret  += std::toupper(str[0]);
  for (int i = 1; i < str.size(); i++)
    ret += std::tolower(str[i]);
  return ret;
}

int get_city_index(string cityname, const vector<string> &cities)
{
  string needle = str_to_upper(cityname.c_str());
  string haystack;
  for (int i = 0; i < cities.size(); i++)
    {
      haystack = str_to_upper(cities[i].c_str());
      if (needle == haystack)
	return i;
    }
  return -1;
}

void print_usage(bool about_file=false)
{
  cout << "Shaun's dist.x program, initial version." << endl
       << "USAGE: dest.x <cities.txt>" << endl;
  
  cout << endl 
       << "About cities file." << endl
       << "This program expects all cities files to begin with an" << endl
       << "integer detailing the length of the file. If this integer" << endl
       << "is not present or is overly large the file is declared invalid." << endl
       << endl 
       << "If there are fewer cities than indicated, or an invalid city name" << endl
       << "is encountered, the file is flagged as such and reading is aborted." << endl
       << "If there are fewer distances than expected, or of an invalid format" << endl
       << "reading is also aborted." << endl
       << endl 
       << "Example format:" << endl
       << "3" << endl
       << "Appleton" << endl
       << "Oshkosh" << endl
       << "Menasha" << endl
       << "30" << endl
       << "4 38" << endl
       << "<EOF>" << endl
       << endl;

}

int main(int argc, char **argv)
{
  vector<string> cities;
  ifstream cities_file;
  //  cities_file.open("cities.txt");
  adjMatrix a(2);

  if (argc != 2)
    {
      print_usage();
      return 0;
    }
  
  else
    {
      cities_file.open(argv[1]);
      if (!cities_file.good())
	{
	  cout << "Can't seem to open that file at '" << argv[1] << "'.\n";
	  return 1;
	}
    }

  if (read_cities_file(cities_file, cities, a))
    {
      cout << "An error occured loading the file.\n";
      return 0;
    }

  cout << "Loaded " << a.size() << " cities." << endl;
  string citya, cityb;

  while (1)
    {
      cout << "CITY A> ";
      cin >> citya;
      if (citya == "$")
	{
	  cout << "Goodbye!\n"; 
	  return 0;
	}
      
      cout << "CITY B> ";
      cin >> cityb;
      if (cityb == "$")
	{
	  cout << "Goodbye!\n";
	  return 0;
	}
      
      if (get_city_index(cityb, cities) < 0)
	{
	  cout << "City '" << citya << "' not found!\n";
	  continue;
	}
      else if (get_city_index(cityb, cities) < 0) 
	{
	  cout << "City '" << cityb << "' not found!\n";
	  continue;
	}
      else
	{
	  cout << "Distance between " << str_to_proper(citya) << " and " << str_to_proper(cityb) << ":\n";
	  cout << a.edge(
			 get_city_index(citya, cities),
			 get_city_index(cityb, cities)) << endl;
	}
    }
  return 0;
}
