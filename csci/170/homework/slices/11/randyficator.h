/* Prototypes */
string parseInput(string);

/*
 * parseInput(string inpt) - convert a given string to HTML-formatted
 * Randy-Styled output. This means All alnum characters are passed
 * as-is, all punctuation is HTML-escaped ('&#xFF;'-style), and all
 * non-printable ascii is output in bolded '[FF]' notation where FF
 * is the Hex representation of the ASCII code.
 *
 * RETURNS: A string containing the formatted output.
 */

#define MAX_LINE_LENGTH 75

string
parseInput(string inpt)
{
  int linelen = 0;
  char me;
  stringstream output;

  for (int i = 0; i < inpt.length(); i++)
        {
          me = inpt[i];
          if ( (!isprint(me)) || (isspace(me)) ) 
                {
                  output << std::hex << "<b>[" << setfill('0') << setw(2) << static_cast<int>(me) << "]</b>";
                  linelen += 4;
                }
          else if ( isalnum(me) )
                {
                  output << me << "";
                  linelen += 1;
                }
          else 
                {
                  output << std::hex << "&#x" << static_cast<int>(me) << ";";
                  linelen += 1;
                }
	  if (linelen > MAX_LINE_LENGTH) 
	    {
	      linelen = 0;
	      output << "<br />\n";
	    }

        }
  return output.str();
}
