/*******************************************************************************
 *
 * dist.cpp 
 * for S1.P1 in CSCI 310
 * CREATED BY: Shaun Meyer, Jan-2011
 *
 *******************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "adjMatrix.hpp"

#define MAX_CITIES 1024       // Number of cities we will accept from the file.

enum {
   OK,
   INVALID_CITY, 
   INVALID_DISTANCE,
   INVALID_FILE
};

using namespace std;

bool isalpha(string str)
/* Returns true if ALL characters are alphabetical only. */
{
   for (int i = 0; i < str.size(); i++)
      if (!isalpha(str[i]))
	 return false;

   return true;
}

bool isdigit(string str)
/* Returns true if ALL characters are numeral only. */
{
   for (int i = 0; i < str.size(); i++)
      if (!isdigit(str[i]))
	 return false;
   return true;
}

int read_cities_file(ifstream &file, vector<string> &cities, 
		     adjMatrix &city_matrix)
/* Read a given ifstream object to populate cities vector and city_matrix 
   adjMatrix. Dies for a number of interesting errors, returning values
   defined in enum{} above. */
{
   string snum_cities;    // string for num_cities line (1'st line in file)
   int num_cities = 0;    // integer we actually use for this after Q/A.

   file >> snum_cities;

   /* Check that the number of cities is, in fact, a numeral. */
   if (isdigit(snum_cities))
      num_cities = atoi(snum_cities.c_str());
   else
      return INVALID_FILE;

   /* Bail out if there are an absurd number of cities specified */
   if (num_cities > MAX_CITIES)
      return INVALID_FILE;

   /* read city names */
   string cur_city;        // current city (this is user input!)

   /* For as many cities as specified, attempt to read them in */
   for (int i = 0; i < num_cities; i++)
      {
      /* check for EOF or other file conditions */
      if (!file.good())
	 return INVALID_FILE;

      /* load in a city */
      file >> cur_city;

      /* ERROR CHECK cur_city */
      if (!isalpha(cur_city))
	 {
	 cout << "Invalid city: " << cur_city << endl;
	 return INVALID_CITY;
	 }
      
      /* It's cool, add it to the list and continue. */
      cities.push_back(cur_city);
      }

   /* We read city distances as a string for error checking */
   string distance;

   /* My adjMatrix class is resizable. We initialize it to 
    * a 2x2 and resize it here given the num_cities input */
   city_matrix.resize_matrix(num_cities);

   /* Attempt to load all distances. There will be num_cities rows
      and each row will have one n*row columns in it. */
   for (int y = 1; y < num_cities; y++)
      {
      for (int x = 0; x < y; x++)
	 {
	 /* Check that the file is still ok (no EOF or whatever) */
	 if (!file.good())
	    return INVALID_FILE;

	 /* load up a distance */
	 file >> distance;

	 /* check the distance for sanity. */
	 if (!isdigit(distance))
	    {
	    cout << "Invalid distance at " << distance << endl;
	    return INVALID_DISTANCE;
	    }

	 /* Distance seems sane. Add it to the appropriate edge(s) */
	 city_matrix.edge(x, y) 
	    = city_matrix.edge(y, x) 
	    = atoi(distance.c_str());
	 }
      }
  
   return 0;
}

string str_to_upper(const string str)
/* Convert a given string to UPPERCASE. */
{
   string ret = "";
   for (int i = 0; i < str.size(); i++)
      ret += std::toupper(str[i]);
   return ret;
}

string str_to_proper(const string str)
/* Convert a given string to be all lowercase except the 1st character. */
{
   string ret;
   ret  += std::toupper(str[0]);
   for (int i = 1; i < str.size(); i++)
      ret += std::tolower(str[i]);
   return ret;
}

int get_city_index(string cityname, const vector<string> &cities)
/* given a vector of strings, return the index of a given search.
 * returns -1 if not found.
 * Additionally, all searches are case-insensitive. */
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

void print_cities(const vector<string> &cities)
/* pretty-print a list of the cities to the screen for reference. */
{
   int width = 0; /* printed characters counter. Keep this under 80. */
   for (int i = 0; i < cities.size(); i++)
      {
      width += cities[i].size() + 1; // city size + space
      if (width > 80) 
	 {
	 cout << endl;
	 width = 0;
	 }
      else
	 cout << cities[i] << " ";
      }
   cout << endl;
}

string get_city_from_user(string city_id, const vector<string> &cities)
/* We basically do all this twice, so it got functionalized. Always
 * returns a valid city name or we die from here. */
{
   string city = ""; // Return variable.

   while (city == "")
      {
      /* Prompt and wait on input */
      cout << "CITY " << city_id << "> ";
      cin >> city;

      /* handle QUIT */
      if (city == "$")
	 {
	 cout << "Goodbye!\n";
	 exit(0);
	 }
      
      /* handle PRINT_CITIES */
      else if (city == "#")
	 {
	 print_cities(cities);
	 city="";
	 continue;
	 }
      
      /* handle BOGUS CITIES */
      else if (get_city_index(city, cities) < 0) 
	 {
	 cout << "City '" << city << "' not found!\n";
	 city = "";
	 continue;
	 }
      }
   // We've made it this far, it must be a legit city!
   return city;
}

int main(int argc, char **argv)
{
   vector<string> cities; // An ordered list of cities.
   ifstream cities_file;  // input file object
   adjMatrix a(2);        // adjacency matrix of all city distances.

   /* get cities list from command-line, if something's weird 
    * just print the usage and die. */
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
  

   /* We were able to open the file, read it in or die loudly. */
   if (read_cities_file(cities_file, cities, a))
      {
      cout << "An error occured loading the file.\n";
      return 0;
      }

   /* Print some stats before main input loop. */
   cout << "\nLoaded " << a.size() << " cities." << endl
	<< "$ to quit, # to list cities." << endl;

   string citya, cityb;

   /* Repeat here until user bails. */
   while (1)
      {
      citya = get_city_from_user("A", cities);
      cityb = get_city_from_user("B", cities);

      cout << "( " << str_to_proper(citya) << " ) ---- " 
	   << a.edge(get_city_index(citya, cities),
		     get_city_index(cityb, cities)) 
	   << " ---- ( " << str_to_proper(cityb) << " )" << endl;
      }
  
   /* All done! Lets go home. */
   return 0;
}
