#include <iostream>
#include <string>
#include <strings.h> // for bzero which I'll remove later.
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h> //inet_aton()
#include <sys/poll.h>

using namespace std;

#define PORT 9000
#define SERVER_ADDR "127.0.0.1"
#define MAXBUF 1024
#define BUFFER_SIZE 1024

/*
 * getSocketInput(&client)
 * Return the recv() buffer as a string object
 */

string
getSocketInput(int* client)
{
  char buffer[BUFFER_SIZE];
  string inpt;
  
  recv(*client, (void*)buffer, BUFFER_SIZE, 0);
  inpt = buffer;
  if (inpt.find_first_of('\r') != string::npos)
	inpt.erase(inpt.find_first_of('\r'));
  return inpt;
}

/*
 * Simple wrapper around send() that accepts
 * a string-type in argument.
 */

int
giveSocketReply(int* client, string reply)
{
  send(*client, (void*)reply.c_str(), reply.length()+1, 0);
}

int
main(int argc, char* argv[])
{
  int sock;
  struct sockaddr_in dest;
  char buffer[MAXBUF];
  string line;

  if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
	  perror("Socket");
	  exit(errno);
	}

  /* Initialize server address/port struct */
  bzero(&dest, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORT);
  if ( inet_pton(AF_INET, SERVER_ADDR, &dest.sin_addr) == 0 )
	{
	  perror(SERVER_ADDR);
	  exit(errno);
	}

  /* Connect to Server */
  if ( connect(sock, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
	{
	  perror("Connect ");
	  exit(errno);
	}

  //bzero(buffer, MAXBUF);
  //recv(sock, buffer, sizeof(buffer), 0);
  //printf("%s", buffer);

  std::getline(cin, line, '\n');

  giveSocketReply(&sock, line);
  cout << getSocketInput(&sock) << endl;
  
  struct pollfd pfd;
  pfd.fd = sock;
  pfd.events = POLLHUP | POLLIN;

  int ret = poll(&pfd, 2, 265);
 
  if (ret > 0)
	{
	  if (pfd.revents & POLLHUP)
		{
		  cout << "Connection closed. (POLLHUP)" << endl;
		}
	  else if (pfd.revents & POLLIN)
		cout << "Connection Closed\n";
	  else
		{
		  // More allowed, evidently...
		  std::getline(cin, line, '\n');
		  giveSocketReply(&sock, line);
		}
	  close(sock);
	}



return 0;

  
}
