/*******************************************************************************
 * Shaun Meyer
 * CSCI-270 : S1.E1.2 : Paper - Rock - Scissors
 * Sept, 9, 2010
 ******************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib> // for getenv
#include <cstring> // for strcmp
#include <vector>
#include <istream>
#include <fstream>
#include <dirent.h> // for directory parsing (global scores)


#define LONGEST_COMMAND 255 /* limit input to 255 bytes */
#define HISTORY 1000 /* number of games per session to remember */
#define SESSION_PATH "/home/meyersh/private/prs/"

/* Definitions and et cetera */

using namespace std;
char what_beats(char);
char choose(const vector<char>&, vector<char>&);
int prscmp(char, char);
int readlog(string, vector<char>&, vector<char>&);
int writelog(string, vector<char>&, vector<char>&);
string prspretty(char);
void print_score(const vector<char>&, const vector<char>&);
void print_globalscore();

int main() {

  char *remote_addr, *request_method;

  remote_addr = getenv("REMOTE_ADDR");
  request_method = getenv("REQUEST_METHOD");

  // No return address? probably need to die.
  if (remote_addr == NULL) {
    return 0;
  }

  /* Remember our history for this IP-address */
  vector<char> machine_history, human_history;
  readlog(remote_addr, human_history, machine_history);


  cout << "Content-Type: text/plain\n\n";

  if (strcmp(request_method, "GET") == 0) 
    cout << "Hello, " << remote_addr << endl;

  /*
   * Get POST input. Do not accept more characters than 
   * LONGEST_COMMAND since we don't want any web-abuse.
   */
  
  string postinput;

  cin >> setw(LONGEST_COMMAND) >> postinput;

  /* 
   * Input will be one of:
   * p, r, or s (human makes a move)
   * 
   * later: "score", "reset", "history";
   * I intend to use source IP for session tracking.
   */
  
  char humanchoice, machinechoice;

  if (postinput.length() == 1)
    {
      if (postinput == "p" || postinput == "r" || postinput == "s")
	{
	  humanchoice = postinput[0];
	  machinechoice = choose(human_history, machine_history);
	
	  human_history.push_back(humanchoice);
	  machine_history.push_back(machinechoice);

	  cout << machinechoice << endl;
	  cout << prscmp(humanchoice, machinechoice) << endl;
	  print_score(human_history, machine_history);
	  print_globalscore();
	  // A move was made - log it.
	  writelog(remote_addr, human_history, machine_history);
	}
    }

  return 0;
}

/*
 * char what_beats( char foo )
 */

char what_beats(char foo)
{
  if (foo == 'p') 
    return 's';
  else if (foo == 'r')
    return 'p';
  else if (foo == 's')
    return 'r';
  else return 'B';
}


/*
 * char choose()
 * Look at the history of the game and make a (hopefully) 
 * intelligent decision.
 *
 * Algorithm is basically:
 ** 1.0.0 - Start on paper
 ** 2.0.0 - Play what would beat what would beat you.
 ** 2.0.5 - If the same move is played twice in a row,
 **         play what beats it for the third round.
 ** 2.1.0 - UNLESS it's a stalemate, then take the high-road
 **         so if they play the same thing a second time, we win.
 ** 2.2.0 - UNLESS its the second stalemate (they're on to you!)
 **         if so, play another round ahead.
 ** 2.3.0 - UNLESS we're losing by 2 or more...
 */

char choose(const vector<char>& human_history, vector<char>& machine_history)
{ 


  int numrounds = 0;
  
  if (human_history.size() == machine_history.size())
    numrounds = human_history.size();
  else if (human_history.size() < machine_history.size())
    numrounds = human_history.size();
  else if (human_history.size() > machine_history.size())
    numrounds = machine_history.size();

  // calculate how we're doing aginst the player:
  int score = 0;
  for (int i = 0; i < numrounds; i++)
    score += prscmp(machine_history[i], human_history[i]); 


  int prev = numrounds-1;

  char r='!'; // if you get ! ever, an oopsey happened!

  /* 1.0.0 - Start with paper. */ 
  if ( human_history.size() == 0 )
    return 'p';

  /* First stalemate... */
  if ( *human_history.end()-1 == *machine_history.end()-1 
       && *human_history.end()-2 != *machine_history.end()-2)
    return what_beats(human_history.back());

  /* Second stalemate... */
  if (human_history.end()-1 == human_history.end()-2)
    {
      r =  what_beats(*human_history.end()-1);
      return r;
    } 
  
  if (score < -2)
    {
      /* We're in a losing streak. Try a third look ahead. */
      r = what_beats( what_beats( what_beats( human_history.back())));
      return r;
    }
  
  else if ( *human_history.end()-1 == *machine_history.end()-1 )
    {
      r = what_beats( human_history.back() );
      return r;
    }
  
  else return 'X';
}

/* 
 * compare a & b.
 * returns -1 for a < b
 *          0 for a = b
 *          1 for a > b
 */

int prscmp(char a, char b)
{
  if (a == 'p')
    {
      if (b == 'p')
	return 0;
      else if (b == 'r')
	return 1;
      else if (b == 's') 
	return -1;
    }
  else if (a == 'r')
    {
      if (b == 'p')
	return -1;
      else if (b == 'r')
	return 0;
      else if (b == 's')
	return 1;
    }
  else if (a == 's')
    {
      if (b == 'p')
	return 1;
      else if (b == 'r')
	return -1;
      else if (b == 's')
	return 0;
    }
}

/*
 * read the session log and push in the results to the 
 * provided vector<char> references for later computation.
 */
	  
int readlog(string remote_address, vector<char>& human_history, vector<char>& machine_history)
{
  ifstream logfile;
  string logfile_path;
  logfile_path = SESSION_PATH;
  logfile_path += remote_address;
  logfile_path += ".log";
  string line; 
  logfile.open( logfile_path.c_str() );
  while ( getline(logfile, line) )
    {
      if (line.length() == 2)
	{
	  human_history.push_back(line[0]);
	  machine_history.push_back(line[1]);
	}
    }
   logfile.close();
   
   
  return 0;
}

/* 
 * Write out the contents of human_history & machine_history
 * to our session logfile.
 *
 * Do not record more than HISTORY rows.
 */

int writelog(string remote_address, vector<char>&human_history, vector<char>&machine_history)
{
  ofstream logfile;
  string logfile_path;
  logfile_path = SESSION_PATH;
  logfile_path += remote_address;
  logfile_path += ".log";

  logfile.open(logfile_path.c_str());

  /* I was feeling a little paranoid here, like maybe
   * they could get out of sync somehow. stupid interwebs.
   */

  int s = 0;
  if (human_history.size() == machine_history.size())
    s = human_history.size();
  else if (human_history.size() < machine_history.size())
    s = human_history.size();
  else if (human_history.size() > machine_history.size())
    s = machine_history.size();

  int start = 0;
  if (s > HISTORY)
    start = s - HISTORY;

  for (int i = start; i < s; i++)
    logfile << human_history[i] << machine_history[i] << endl;

  logfile.close();
  return 0;
}

/*
 * Return a string show "Paper" for 'p' for wherever we need
 * to display results.
 */
string prspretty(char foo)
{
  string ret;
  if (foo == 'p')
    ret = "Paper";
  else if (foo == 'r')
    ret = "Rock";
  else if (foo == 's')
    ret = "Scissors";
  return ret;
}

void print_score(const vector<char>& human_history, const vector<char>& machine_history)
{
  int i, result, h_score, m_score;

  h_score = m_score = 0;

  for (i = 0; i < human_history.size(); i++)
    {
      result = prscmp(human_history[i], machine_history[i]);
      if (result == -1)
	m_score++;
      else if (result == 1)
	h_score++;
    }

  cout << h_score << ":" << m_score << endl;

}

/* 
 * We abuse readlog() a little bit to get a global score since readlog always uses the
 * push_back() method...
 */

void print_globalscore()
{
  vector<char> h_history;
  vector<char> m_history;
  string t;

  DIR *dp;
  struct dirent *dirp;

  dp = opendir(SESSION_PATH);

  while ((dirp = readdir(dp)) != NULL) {
    t = dirp->d_name;
    if (t == "." || t == "..")
      continue;
    
    t.erase(t.end()-4, t.end());
    readlog(t, h_history, m_history);

  }
  closedir(dp);
  
  print_score(h_history,m_history);
    
}

  
      
