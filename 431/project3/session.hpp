/*
 * session.hpp
 * Store sessions and gameids
 *
 * Shaun Meyer, Mar 2012
 */

#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include <fstream>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <ctime>

using namespace std;

#define GAMEID_LIST_FILE "games.txt"
#define GAMEID_DIRECTORY "games/"

boost::filesystem::path gameid_file_path(string gameid) {
   // Render the file path for a gameid. 
   
   string path = string(GAMEID_DIRECTORY) + gameid;
   return boost::filesystem::path( path.c_str() ) ;
}

bool gameid_exists(string gameid) {
   // Check if a game exists.

   return boost::filesystem::exists( gameid_file_path(gameid) );

}

int gameid_age(string gameid) {
   // how old is a file in seconds?

   std::time_t t = boost::filesystem::last_write_time( gameid_file_path(gameid) );
   std::time_t now = std::time(NULL);
   
   return now-t;
}

int save_game(string gameid, template<T> game) {
   // Serialize and save a game.

   ofstream gameid_file(gameid_file_path(gameid));
   
   if (!gameid_file.good())
	  return 1;

   gameid_file << game.serialize() << endl;
   gameid_file.close();

   return 0;
}




#endif
