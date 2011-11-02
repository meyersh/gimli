#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <cstdlib>
#include <time.h> // for time(NULL)

#include "shaunscgi.h" // for urlEncode(string)

using namespace std;

#define INDEX_FILE     "/home/staff/meyersh/public_html/S12.P1.html"
#define GUESTBOOK_FILE "/home/staff/meyersh/private/guestbook.txt"
#define PASSWORD_FILE  "/home/staff/meyersh/private/password.txt"
#define SEP ':' /* Guestbook field separator character */

/*
  Prototypes
*/

struct kv {
  string key;
  string value;
};

string replace(string str, char search, string replacement); // replace this with that in str.
string friendlyTime(string datetime);
string sanitize(string str);
int getLastId(string file=GUESTBOOK_FILE);
int writeGuestbook(string body, string user="Anonymous Coward",
				   int parent=0, time_t datetime=time(NULL));
void readGuestbook(string file=GUESTBOOK_FILE);
bool checkPassword(string pw, string file=PASSWORD_FILE);
string fgetenv(string env);
kv getKeyValue(string str, char separator='=');

string
friendlyTime(string datetime)
{
  struct tm* timeinfo;
  time_t rawtime = atoi( datetime.c_str());
  timeinfo = localtime( &rawtime );
  string time = asctime(timeinfo);

  /* asctime ends with a newline whether we want one or not.
	 here we'll remove it: */
  for (int i = 0; i < time.size(); i++)
	{
	  if (time[i] == '\n')
		time[i] = ' ';
	}

  return time;
}

string
replace(string str, char search, string replacement)
{
  stringstream ss;
  string::iterator it;
  for (it = str.begin(); it != str.end(); it++)
	{
	  if (*it != search)
		ss << *it;
	  else if (*it == search)
		ss << replacement;
	}
  return ss.str();
	  
}

/*
 * Rudimentary html sanitizing function.
 */

string
sanitize(string str)
{
  stringstream output;
  string::iterator it;
  for (it = str.begin(); it != str.end(); it++)
	{
	  if (isalnum(*it) || isspace(*it))
		output << *it;
	  else
		output << "&#x" << std::hex << static_cast<int>(*it) << ';';
	}

  return output.str();
}  

/*
 * split something=somethingelse on the delimiter and return
 * 'somethingelse'
 */

kv
getKeyValue(string str, char separator)
{
  string key = "";
  string value = "";
  stringstream ss(str);
  std::getline(ss, key, separator);
  std::getline(ss, value, separator);
  struct kv t;
  t.key = key;
  t.value = value;
  return t;
  
}
string
fgetenv(string env)
{
  if (getenv(env.c_str()))
	return getenv(env.c_str());
  else
	return "";
}

bool
checkPassword(string pw, string file)
{
  ifstream pwf;
  pwf.open(file.c_str());
  
  string password;
  std::getline(pwf, password);

  pwf.close();

  if (password == pw)
	return true;
  else
	return false;
}  

/*
 * writeGuestbook(body, [user, [parent, [datetime]]])
 * all you really need here is the <body> attribute. =)
 * time defaults to now, in reply to "nothing", and user is
 * "anonymous coward"
 */

int 
writeGuestbook(string body, string user, int parent, time_t datetime)
{
  if (user == "Username")
	user = "Anonymous Coward";

  /* I want my fields to be ":" delimited so lets replace the colons in
     the body of the message strings with '&#58;' */
  
  //body = replace(body, ':', "&#58;");
  //user = replace(user, ':', "&#58;");
  body = sanitize(body);
  user = sanitize(user);

  ofstream gbf;
  gbf.open(GUESTBOOK_FILE, ios::out | ios::app);

  stringstream line;
  
  int id = getLastId() + 1;

  line << id << SEP 
       << parent << SEP
       << datetime << SEP 
       << user << SEP
       << body;

  gbf << line.str() << endl;
  gbf.close();
  return 0; /* or something else when we catch errors. */

}

/*
 * Read and return html for the guestbook, line-by-line
 */

void 
readGuestbook(string file)
{
  ifstream gbf;
  gbf.open(file.c_str());

  int i;
  stringstream output;

  string token, line;
  string fieldnames[] = {"id", "parentId", "datetime", "username", "body"};
  /* id, parentId, datetime, username, body\n */
  string fields[5]; 
  cout << "guestbook="; /* for the javascript parser */
  while (std::getline(gbf, line, '\n'))
    {
	  if ( line[0] == '#' )
		continue;
	  
	  /* I don't like re-defining this each time,
		 but if I use another stringstream it doesn't
		 take on the new values I try and feed through
		 it... */
	  
	  stringstream linestream(line);

      /* Retrieve the "columns" from this row: */
	  i=0;
      while(std::getline(linestream, fields[i], ':'))
		{
		  //cout << setw(8) << fieldnames[i] << ":  " << fields[i] << endl;
		  i++;
		}
	  output << "<div id='post'><i>" << fields[3] << "</i><small> @ "
			 << friendlyTime(fields[2]) << "</small> says: '"
			 << fields[4] << "'</div>";
    }
  cout << urlEncode(output.str());

  gbf.close();
}

int 
getLastId(string file)
{
  string line, token;
  stringstream ls, ids;
  int id=0;

  ifstream gbf;
  gbf.open(file.c_str());
  
  while (!gbf.eof())
    {
      std::getline(gbf, line, '\n');
      ls.str(line);
      std::getline(ls, token, SEP);
      if (atoi(token.c_str()) > id)
	  id = atoi(token.c_str());
    }
  return id;
}
      

int 
main()
{
  //writeGuestbook(1, "hey, whats up?", "meyersh");
  //  cout << "Hi ID: '" << getLastId() << "'" << endl;
  //readGuestbook();

  string REQUEST_METHOD = fgetenv("REQUEST_METHOD");
  string HTTPS = fgetenv("HTTPS");
  cout << "Content-type: text/html\n\n";

  if (REQUEST_METHOD == "POST")
	{
	  string line;
	  string password="";
	  string username="";
	  string comment="";
	  string action="";
	  
	  while (std::getline(cin,line))
		{
		  struct kv p = getKeyValue(line);
		  if (p.key == "password")
			password=p.value;
		  else if (p.key == "username")
			username=p.value;
		  else if (p.key == "comment")
			comment=p.value;
		  else if (p.key == "action")
			action=p.value;
		}

	  /* I'm looking at a couple of scenarios here -
		 1) a "refresh" (or view) is issued. This only requires the PASSWORD,
		 so the rest can / should-be NULL.

		 2) a "post" - this requires at least the password & a body.

		 the action (refresh or post) is passed as action=...
	  */

	  if (checkPassword(password))
		{
		  
		  if (action == "refresh")
			{
              cout << "state=Guestbook contents has been refreshed.\n";
			  readGuestbook();
			}
		  else if (action == "post")
			{
			  if (!writeGuestbook(comment, username))
                cout << "state=Your message has been successfully recieved.\n";
              else
                cout << "state=Error recording message.\n";
			  readGuestbook(); // this refreshes the guestbook on page
			}
		  else
			exit(1);
		  
		}
	  else
		cout << "state=Invalid password!\n";
	  
	   
	  
	}
  else if (REQUEST_METHOD == "GET")
	{
	 	  if (HTTPS == "on")
			cout << "PREMIUM UNLEADED" << endl;
		  else
			cout << "REGULAR UNLEADED\n";
		  ifstream index;
		  index.open(INDEX_FILE);
		  string line;
		  while (std::getline(index, line))
			{
			  if (line != "{{GUESTBOOK}}")
				cout << line << endl;
			  else
				{
				  //readGuestbook();
				}
			}
		 
	}
  return 0;

}
