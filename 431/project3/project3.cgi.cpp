/*
 * Handle the webapp junk.
 * Shaun Meyer, Mar 2012
 */

// TODO: Session persistance
// TODO: Game serialization for saving + loading
// TODO: game_id persistance
// TODO: game_id timeouts
// TODO: ?

#include <iostream>
#include <sstream>
#include "session.hpp"
#include "shauns_Pente.hpp"

using namespace std;

void die(string msg) {
   cout << "ERROR_CAUSED_SHUTDOWN" << endl
		<< msg << endl;
   exit(1);
}

int main() {
   string game_id;
   string session_id;

   string instr; // initial POST instruction
   string param; // initial POST parameter.

   getline(cin, instr); // all requests are by POST.
   getline(cin, param); 

   cout << "Content-Type: text/plain\n\n";

   // Filter out blank params.
   if (instr == "")
	  die("msg: Expected parameter.");

   if (instr == "SETUP")
	  {
	  // check for four characters, noting all options
	  // are size() == 4, begin with h and end with 1 or 2.

		bool human_only;
		bool creator_is_white;
		string gameid;
		string sessionkey;
	  
	  if (param.size() != 3 || param[0] != 'h')
		 die("msg: Invalid parameter: " + param);

	  if (param[1] == 'h')
		 bool human_only = true;
	  else if (param[1] == 'c')
		 bool human_only = false;
	  else
		 die("2nd character must be h or c.");

	  if (param[2] == '1')
		 bool creator_is_white = true;
	  else if (param[2] == '2')
		 bool creator_is_white = false;
	  else
		 die("3rd character must be 1 or 2.");

	  // Generate a unique game id and session key.
	  for (int i = 0; i < NUM_WORDS; i++)
		 {
		 gameid = get_word(i);
        	 sessionkey = get_word(++i);
		 if (!gameid_exists(gameid))
			break;
		 }

	  ofstream game_file(gameid_file_path(gameid));

	  if (!game_file.good())
	    die("Unable to create new game file at '" + gameid + "'.");

	  Pente new_game;
	  new_game.turn = BLACK;

	  if (human_only) 
		{

		  if (creator_is_white) 
			new_game.white_player = sessionkey;
		  
		  else 
			new_game.white_player = sessionkey;

		} 
	  else 
		{
		
		  if (creator_is_white) 
			{
			  new_game.white_player = sessionkey;
			  new_game.black_player = "COMPUTER";
			} 
		  else 
			{
			  new_game.white_player = "COMPUTER";
			  new_game.black_player = sessionkey;
			}
		}

	  // Finally, place the white piece on the board and 
	  // save the file.
	  new_game.fillCell(9,9,WHITE);
	  game_file << new_game.serialize();
	  game_file.close();

	  // return the details to the client
	  cout << "SETUP" << endl
		   << gameid << endl
		   << sessionkey << endl;
	  }

   else if (instr == "JOIN")
	  {

	  // TODO: Check that param is a valid gameid.
	  // TODO: Assign a valid session ID otherwise.
	  // TODO: Or report that the game is full.

	  /*
		JOIN\n<gameid>	JOIN\n<gameid>\n<sessionid>	
		JOIN\n<gameid>	JOIN\n<gameid>\nGAME_UNDERWAY	
	  */

	  if (!gameid_exists(param))
		 die("No game identified by '" + param + "'");

	  Pente game;
	  
	  ifstream game_file( gameid_file_path(param) );
	  game.deserialize(game_file);
	  game_file.close();

	  cout << "JOIN" << endl
		   << param << endl;; 
	  // TODO: figure out session ID.


	  }

   else if (instr == "MOVE")
	  {

	  // TODO: Validate gameid, sessionid, and feed the computer a move.

	  }

   else if (instr == "CHECK")
	  {

	  // TODO: Return computer (or other player) move.

	  }
   

   return 0;

}
