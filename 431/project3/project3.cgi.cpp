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

using namespace std;

void die(string msg) {
   cout << "ERROR_CONDITION_THINGER" << endl
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


   // Filter out blank params.
   if (instr == "")
	  die("msg: Expected parameter.");

   if (instr == "SETUP")
	  {
	  // check for four characters, noting all options
	  // are size() == 4, begin with h and end with 1 or 2.
	  
	  if (param.size() != 3 || param[0] != 'h')
		 die("msg: Invalid parameter: " + param);

	  if (param[1] == 'h')
		 bool human_only = true;
	  else if (param[1] == 'c')
		 bool human_only = false;
	  else
		 die("2nd character must be h or c.");

	  if (param[2] == '1')
		 bool creator_is_black = true;
	  else if (param[2] == '2')
		 bool creator_is_black = false;
	  else
		 die("3rd character must be 1 or 2.");


	  // TODO: Create a game, files, sessions, etc.
	  }

   else if (instr == "JOIN")
	  {

	  // TODO: Check that param is a valid gameid.
	  // TODO: Assign a valid session ID otherwise.
	  // TODO: Or report that the game is full.

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
