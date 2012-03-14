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
#include <vector>
#include "session.hpp"
#include "Pente.hpp"

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

	string param; // a place holder for reading in the parameters

	vector<string> params; // subsequent POSTS.

	getline(cin, instr); // all requests are by POST.
   
	while (getline(cin, param))
		params.push_back(param);

	cout << "Content-Type: text/plain\n\n";

	// Filter out blank params.
	if (instr == "")
		die("msg: Expected parameter.");

	/*
	 * S E T U P
	 */

	if (instr == "SETUP")
	{
		// check for four characters, noting all options
		// are size() == 4, begin with h and end with 1 or 2.

		bool human_only;
		bool creator_is_white;
		string gameid;
		string sessionkey;
	  
		string game_type = params[0];

		if (game_type.size() != 3 || game_type[0] != 'h')
			die("msg: Invalid parameter: " + param);

		if (game_type[1] == 'h')
			human_only = true;
		else if (game_type[1] == 'c')
			human_only = false;
		else
			die("2nd character must be h or c.");

		if (game_type[2] == '1')
			creator_is_white = true;
		else if (game_type[2] == '2')
			creator_is_white = false;
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
			die("Unable to create new game file at '" + 
				string(gameid_file_path(gameid)) + "'.");

		Pente new_game;
		new_game.turn = 1;

		if (human_only) 
		{

			if (creator_is_white) 
				new_game.players[0] = sessionkey;
		  
			else 
				new_game.players[1] = sessionkey;

		} 
		else 
		{
		
			if (creator_is_white) 
			{
				new_game.players[0] = sessionkey;
				new_game.players[1] = "COMPUTER";
			} 
			else 
			{
				new_game.players[0] = "COMPUTER";
				new_game.players[1] = sessionkey;
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

	/*
	 * J O I N
	 */

	else if (instr == "JOIN")
	{

		// TODO: Assign a valid session ID otherwise.
		// TODO: Or report that the game is full.

		/*
		  JOIN\n<gameid>	JOIN\n<gameid>\n<sessionid>	
		  JOIN\n<gameid>	JOIN\n<gameid>\nGAME_UNDERWAY	
		*/

		if (!gameid_exists(param))
			die("No game identified by '" + param + "'.");

		Pente game;
		
		// TODO: Check for invalid # of params.
		string gameid = params[0];

		ifstream game_file( gameid_file_path(gameid) );
		game.deserialize(game_file);
		game_file.close();

		cout << "JOIN" << endl
			 << gameid << endl;
		// TODO: figure out session ID.

		if (game.players[0] != "WAITING" && game.players[1] != "WAITING")
			cout << "GAME_UNDERWAY" << endl;

		if (game.players[0] == "WAITING") {
			game.players[0] = generate_sessionid();
			cout << "hh2" << endl
				 << "9" << endl
				 << "9" << endl
				 << game.players[0] << endl
				 << "MOVE" << endl;
		}

		else if (game.players[1] == "WAITING") {
			game.players[1] = generate_sessionid();
			cout << "hh1" << endl 
				 << "9" << endl
				 << "9" << endl
				 << game.players[1] << endl
				 << "WAITING" << endl;
		}

		// Save our game.
		ofstream ogame_file( gameid_file_path(gameid) );
		ogame_file << game.serialize() << endl;

		ogame_file.close();

	}

	/* 
	 * M O V E 
	 */
   
	else if (instr == "MOVE")
	{

		// TODO: Validate gameid, sessionid, and feed the computer a move.

	}

	/*
	 * C H E C K
	 */

	else if (instr == "CHECK")
	{

		// TODO: Return computer (or other player) move.

	}
   

	return 0;

}
