/******************************************************************************
 * sysutils-driver.cpp - 
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * A testing interface for my sysutils library.
 *****************************************************************************/

#include <iostream>
#include "sysutils.hpp"
#include "shaun.hpp"

using namespace std;

int main()
{
   vector<string> files = lsdir(".");
   print_vector(files);
   for (int i = 0; i < files.size(); i++)
      {
      cout << "mtime: " << files[i] << ": " << timestamp(files[i]) << endl;
      }
}
