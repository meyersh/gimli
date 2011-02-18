#ifndef __SYSUTILS__HPP__
#define __SYSUTILS__HPP__

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>

std::vector<std::string> lsdir(const char *path);
int timestamp(const char *filename);
int timestamp(const std::string &filename);

int delete_oldest_file_in_directory(const std::string &path, time_t min_age=time(NULL));

#endif
