/******************************************************************************
 * sysutils.hpp - 
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * Some functions to return directory contents and get the timestamp of a file.
 *****************************************************************************/

#ifndef __SYSUTILS__HPP__
#define __SYSUTILS__HPP__

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> lsdir(const char *path);
int timestamp(const char *filename);
int timestamp(const std::string &filename);

int delete_oldest_file_in_directory(const std::string &path, 
				    time_t min_age=time(NULL));

#endif
