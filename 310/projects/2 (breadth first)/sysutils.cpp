#include "sysutils.hpp"

std::vector<std::string> lsdir(const char *path)
{
   DIR *dp;
   struct dirent *dirp;
   std::vector<std::string> files;

   if((dp = opendir(path)) == NULL)
      throw "Error opening directory.";
   
   while ((dirp = readdir(dp)) != NULL)
      files.push_back(std::string(dirp->d_name));

   closedir(dp);
   return files;
	 
}

int timestamp(const char *filename)
{
   struct stat file_stat;
   stat(filename, &file_stat);
   return file_stat.st_mtime;
}
int timestamp(const std::string &filename)
{
   return timestamp(filename.c_str());
}
