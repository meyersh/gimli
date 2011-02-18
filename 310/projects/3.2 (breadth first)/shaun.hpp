#ifndef __SHAUN_HPP__
#define __SHAUN_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <map>

std::vector<std::string> split(const std::string line, const std::string split_pattern=",");
std::string uppercase(const std::string line);

template<class V> 
void print_vector(const std::vector<V> &vec) 
/* iterate through a vector, printing the contents with std::cout */
{
   for (int i = 0; i < vec.size(); i++)
      std::cout << i << ": " << vec[i] << std::endl;
}

template<class K, class V>
void print_map(const std::map<K,V> &con)
/* iterate through a map, printing the contents with std::cout */
{
   typename std::map<K, V>::const_iterator it;

   for (it = con.begin(); it != con.end(); ++it)
      std::cout << it->first 
		<< " => " 
		<< it->second 
		<< std::endl;
}

#endif
