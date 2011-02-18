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

int delete_oldest_file_in_directory(const std::string &path, time_t min_age)
/* Delete the oldest file in a directory, comparing the time against NOW.
 * if time_t is set to something like (say) five minutes ago, it will delete the
 * oldest files that are 5 minutes old or more. Returns 1 if a file was deleted,
 * 0 if none were. */
{
   std::vector<std::string> files = lsdir(path.c_str());
   std::string oldest_file_name;
   unsigned int oldest_file_time = min_age;
   for (int i = 0; i < files.size(); i++)
      {
      if (files[i] == "." || files[i] == "..")
	 continue;

      if (timestamp( std::string(path + files[i]) ) < oldest_file_time)
	 {
	 oldest_file_time = timestamp( std::string(path + files[i]) );
	 oldest_file_name = files[i];
	 }
      }
   
   if (oldest_file_name != "")
      {
      unlink( std::string(path + oldest_file_name).c_str() );
      return 1;
      }
   return 0;
}
