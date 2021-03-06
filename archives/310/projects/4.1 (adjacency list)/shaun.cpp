/******************************************************************************
 * shaun.cpp - 
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * A few utility functions I was getting sick of re-writing / 
 * cutting + pasting.
 *****************************************************************************/
#include "shaun.hpp"

std::vector<std::string> split(const std::string line, 
			       const std::string split_pattern)
/* DESCR: Split a string `line` on `split_pattern` 
 * RETURNS: a vector containing the result. */
{
   std::vector<std::string> ret;
   int start,
      end,
      len = 0;

   for (start = 0; start < line.size(); start += len + 1)
      {
      len = 0;
      end = line.find(split_pattern, start);
      if (end == std::string::npos)
	 {
	 ret.push_back(line.substr(start, line.size()-start));
	 break;
	 }
      
      /* If we made it this far, we've got a match. */
      len = end - start;

      if (len < split_pattern.size())
	 continue; 
      
      ret.push_back(line.substr(start, len));
            
      }
   return ret;
}

std::string uppercase(const std::string line)
/* DESCR: Return a given string in UPPERCASE */
{
   std::string ret;
   for (int i = 0; i < line.size(); i++)
      ret += toupper(line[i]);
  
   return ret;
}

