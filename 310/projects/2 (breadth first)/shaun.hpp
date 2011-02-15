#ifndef __SHAUN_HPP__
#define __SHAUN_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <map>

std::vector<std::string> split(const std::string line, const std::string split_pattern=",");

template<class V>
void print_vector(const std::vector<V> &vec);

template<class K, class V>
void print_map(const std::map<K,V> &con);

std::string uppercase(const std::string line);

#endif
