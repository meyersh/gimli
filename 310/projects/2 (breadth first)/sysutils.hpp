#ifndef __SYSUTILS__HPP__
#define __SYSUTILS__HPP__

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>

std::vector<std::string> lsdir(const char *path);
int timestamp(const char *filename);
int timestamp(const std::string &filename);

#endif
