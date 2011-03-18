/*
  Initial question (when tree is empty): What is it?
  Question at an internal node: Does it have a/an ____? (Item in blank comes from node.)
  Question at a leaf node: Is it a/an ____? (Item in blank comes from node.)
  Questions if not found at leaf:
  What is it?
  What property does it have that makes it different from a/an ____? 
  (Item in blank comes from item in leaf node being expanded.)

  There will be 5 bonus points for any project that meets the basic requirements
  and also allows trees to be save to files and read in from files.
*/

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cctype> // for toupper()

using namespace std;

class Node {
public:
   string data;
   Node *parent, *right, *left;
   
   Node(string str="")
   /* PARAMS     : String for `data` member. Defaults to "".
      RETURNS    : void
      DESCRIPTION: Initializes all pointers to null and sets data string */
   {
      parent = right = left = NULL;
      data = str;
   }
   
   void insert_right_child(Node *child)
   /* PARAMS     : Pointer to a Node.
      RETURNS    : void
      DESCRIPTION: Takes a pointer to a node and inserts it here. If this
      position already has a child, the child is shifted down and right. */
   {
      child->parent = this;
      child->right = this->right;
      this->right = child;
   }

   void insert_right_child(string data)
   /* PARAMS     : data-member string.
      RETURNS    : void
      DESCRIPTION: Takes a data-member string for a node, creates the node 
      and inserts it at this position. If a child already exists, it is shifted
      down and right. */
   {
      Node *newnode = new Node;
      newnode->data = data;
      insert_right_child(newnode);
   }
   
   void insert_left_child(Node *child)
   /* PARAMS     : Pointer to a Node.
      RETURNS    : void
      DESCRIPTION: Takes a pointer to a node and inserts it here. If this
      position already has a child, the child is shifted down and left. */
   {
      child->parent = this;
      child->left = this->left;
      this->left = child;
   }

   void insert_left_child(string data)
   /* PARAMS     : data-member string.
      RETURNS    : void
      DESCRIPTION: Takes a data-member string for a node, creates the node 
      and inserts it at this position. If a child already exists, it is shifted
      down and right. */
   {
      Node *newnode = new Node;
      newnode->data = data;
      insert_left_child(newnode);
   }

   bool isleaf()
   /* PARAMS     : none
      RETURNS    : boolean
      DESCRIPTION: Returns true if this node is a leaf node, false otherwise. */
   {
      return !(left || right);
   }

}; // end Node definition

/***********************
 * HELPER FUNCTIONS
 **********************/

std::vector<std::string> split(const std::string line, 
			       const std::string split_pattern=",")
/* PARAMS     : string thing, string separator
   RETURNS    : vector of strings
   DESCRIPTION: Splits a string into pieces on `split_pattern` and returns a
   vector of these results. */
{
   std::vector<std::string> ret;
   int start,
      end,
      len = 0;

   for (start = 0; start < line.size(); start += len + 1)
      {
      len = 0;
      end = line.find(split_pattern, start);
      if (end == std::string::npos)
         {
         ret.push_back(line.substr(start, line.size()-start));
         break;
         }

      /* If we made it this far, we've got a match. */
      len = end - start;

      if (len < split_pattern.size())
         continue;

      ret.push_back(line.substr(start, len));

      }
   return ret;
}

string toupper(string str)
/* PARAMS     : string
   RETURNS    : string
   DESCRIPTION: Return a string with all characters uppercase. */
{
   string ret = "";

   for (int i = 0; i < str.size(); i++)
      ret += toupper(str[i]); // this is the toupper(char) version!

   return ret;
}

bool isvowel(char inpt)
/* PARAMS     : character
   RETURNS    : boolean
   DESCRIPTION: Return true if the character is a vowel. */
{
   char vowels[] = {'A', 'E', 'I', 'O', 'U', NULL};
   for (char v = vowels[0]; v != NULL; v++)
      if (v == toupper(inpt))
	 return true;
   return false;

}

/*********************
 * MEMORY MANAGEMENT
 ********************/

void deallocate_map_nodes(map<string, Node*> &nodes)
/* PARAMS     : map<string, node*> of nodes
   RETURNS    : void
   DESCRIPTION: Deallocates all nodes pointed to by the nodes map*/
{
   for (map<string,Node*>::iterator iter = nodes.begin();
	iter != nodes.end();
	iter++)
      delete iter->second;
}

Node* deallocate_binary_tree(Node* &node)
/* PARAMS     : reference to a Node ptr
   RETURNS    : A Node ptr
   DESCRIPTION: Recursively deletes all nodes beginning at `node` and sets
   all deleted pointers to NULL */
{
   if (node == NULL)
      return NULL;

   // recurse to children (if there are any)
   deallocate_binary_tree(node->left);
   deallocate_binary_tree(node->right);

   // finally, delete the node and reset its pointer.
   delete node;
   node = NULL;

   return node;
}


/***********************
 * FILE SAVING
 **********************/

string bt_node(Node *root)
/* PARAMS     : Node pointer
   RETURNS    : string
   DESCRIPTION: Given a node pointer, returns a pipe-separated representation 
   of the node's data suitable for printing or saving. */
{
   if (!root)
      return "";

   stringstream ret;
   ret << root 
       << "|" << root->parent 
       << "|" << root->left
       << "|" << root->right 
       << "|" << root->data << endl;
   
   return ret.str();
}

string print_binary_tree(Node *root)
/* PARAMS     : Node ptr
   RETURNS    : string
   DESCRIPTION: Recursively descends into a binary tree calling bt_node to 
   print all nodes. */
{
   if (root == NULL)
      return "";

   stringstream ret;
   ret << bt_node(root);
   ret << print_binary_tree(root->left);
   ret << print_binary_tree(root->right);
   return ret.str();
}

int binary_tree_save_file(string filename, Node *root)
/* PARAMS     : string filename, node ptr
   RETURNS    : int (non-zero for failure, 0 for success)
   DESCRIPTION: Opens a file, saving the output of print_binary_tree() to it. */
{
   ofstream save_file(filename.c_str());
   if (!save_file)
      return -1;
   
   save_file << print_binary_tree(root) << endl;
   
   save_file.close();

   return 0;
}

/**********************
 * FILE LOADING
 *********************/

Node *node_from_string(string nodestr)
{
   vector<string> splitted = split(nodestr);
   Node *ret = new Node;
   ret->data = splitted[4];

   return ret;
}


Node *load_file(string filename)
/* PARAMS     : string filename
   RETURNS    : pointer to root node or NULL indicating a failure.
   DESCRIPTION: Attempts to read a file into a binary tree, 
   creating as it goes. */
{

   ifstream ifile(filename.c_str());
   
   if (!ifile)
      return NULL;

   string line;
   map<string, Node*> nodes; // NODE_ID -> node* map

   Node *root = NULL;

   /* Repeat for every line in ifile */
   while (std::getline(ifile, line))
      {
      // Ignore lines beginning with a # (comments) in the data file.
      if (line.size() && line[0] == '#')
	 continue;

      // this is our data-line.
      vector<string>split_line = split(line, "|");
      
      // for legibility below, these are the values in split_line:
      enum {NODE_ID, PARENT_ID, LEFT_ID, RIGHT_ID, DATA, NUM_FIELDS=5};

      /* filter invalid lines */
      if (split_line.size() != NUM_FIELDS)
	 continue;
      
      /* Create the current node in the nodes map if it's not already there. */
      if (nodes.end() == nodes.find(split_line[NODE_ID]))
       	 nodes.insert(std::pair<string, Node*>(split_line[NODE_ID], 
					       new Node(split_line[DATA])));

      /* Make sure the data field is set (in case this node was
	 created earlier as an UNDEFINED CHILD . */
      if (nodes[split_line[NODE_ID]]->data == "UNDEFINED CHILD")
	 nodes[split_line[NODE_ID]]->data = split_line[DATA];

      /** Figure CHILDREN out **/
      /* LEFT child */
      if (split_line[LEFT_ID] != "0") // if we're not a leaf
	 {
	 if (nodes.end() == nodes.find(split_line[LEFT_ID]))
	    nodes.insert(std::pair<string, Node*>(split_line[LEFT_ID], 
						  new Node("UNDEFINED CHILD")));
	 
	 // Set Parent + Child pointers.
	 nodes[split_line[NODE_ID]]->left = nodes[split_line[LEFT_ID]];
	 nodes[split_line[NODE_ID]]->left->parent = nodes[split_line[NODE_ID]];
	 }

      /* RIGHT child */
      if (split_line[RIGHT_ID] != "0") // if we're not a leaf
	 {
	 if (nodes.end() == nodes.find(split_line[RIGHT_ID]))
	    nodes.insert(std::pair<string, Node*>(split_line[RIGHT_ID], 
						  new Node("UNDEFINED CHILD")));
	 
	 // Set Parent + Child pointers.
	 nodes[split_line[NODE_ID]]->right = nodes[split_line[RIGHT_ID]];
	 nodes[split_line[NODE_ID]]->right->parent = nodes[split_line[NODE_ID]];
	 }


      /** Figure PARENT out **
	  (Can't do a lot, parent doesn't tell us if we're the left or right
	  node so we only check for root status.) */
      if (split_line[PARENT_ID] == "0")
	 root = nodes[split_line[NODE_ID]];
      } // end while()

   // we're done with our file stream at this point.
   ifile.close();

   /*******************
    * SANITY CHECKING
    *******************/

   /* Loop through the map, make sure all nodes:
      1. have valid children (if not null)
      2. if children, that the children->parent ptr is correct.

      if 1 or 2 fail, we deallocate all dynamically allocated memory
      with deallocate_map_nodes() and reset `root` to NULL. */
   for (map<string, Node*>::iterator iter = nodes.begin();
	iter != nodes.end();
	iter++)
      {
      // iter->first is our NODE_ID, iter->second is the Node*.
      Node* curnode = iter->second;

      if (curnode->left)
	 if (curnode->left->parent != curnode)
	    {
	    deallocate_map_nodes(nodes);
	    root = NULL; // blow up.
	    }

      if (curnode->right)
	 if (curnode->right->parent != curnode)
	    {
	    deallocate_map_nodes(nodes);
	    root = NULL; // Blow up.
	    }
      }
     
   return root;
}

/**************************************
 * MAIN - Contains Command-loop logic.
 **************************************/

int main(int argc, char **argv)
{

   Node *root = NULL;

   // parse command line args into a vector of strings
   vector<string>args(argv, argc+argv);

   // store the name of our file for later
   string filename = "";

   if (args.size() > 1) // try to load the specified file.
      {
      root = load_file(args[1].c_str());
      if (root == NULL)
	 cout << "Error loading file '" << args[1] << "'\n";
      else
	 filename = args[1]; // remember the filename for SAVE and LOAD
      }

   cout << "======================================================\n"
	<< "Primary Commands: (For use outside the Q&A routine)\n"
	<< "HELP, QUIT, LOAD, SAVE, GUESS\n" << endl
	<< "For more specific instructions, Type HELP <COMMAND>\n"
	<< "======================================================\n";

   Node *curnode = root; /* Our current node. May be null. */
   string cmd="";        /* user input */
   string prompt="> ";   /* user prompt */

   cout << prompt;       /* initial prompting */

   enum {NORMAL,         /* NORMAL mode, process system commands */
	 IS_IT_A,        /* Present our guess and wait for Y/N. */
	 WHAT_IS_IT ,    /* What is it mode, processing new thing */
	 WHAT_PROP ,     /* What property mode, process new property */
	 RESET};         /* Reset into NORMAL mode */

   unsigned int mode = NORMAL;

   /**********************************************************************
    * Command loop. 
    * Repeat until end of CIN (eof) or a command causes us to break out. 
    **********************************************************************/
   while (std::getline(cin, cmd))
      {
      vector<string> cmds = split(cmd, " "); /* vector of cmd words */
      
      /* We're reseting, we must have reached an end node. */
      if (mode == RESET)
	 {
	 curnode = root;
	 mode = NORMAL;
	 cout << "Reseting curnode to root.\n";
	 }

      /* What is it? At this point, the user has been prompted and
	 `cmd` contains their answer to "What is it?". Here we just
	 have to insert the new node(s) and set the mode for WHAT_PROP. */
      if (mode == WHAT_IS_IT)
	 {
	 if (curnode == NULL)
	    {
	    root = new Node(cmd);
	    curnode = root;
	    cout << "Initial thing added to database. Perhaps you should GUESS again.\n";
	    mode = NORMAL;
	    }
	 else 
	    {
	    curnode->insert_left_child(new Node(curnode->data));
	    curnode->insert_right_child(new Node(cmd));
	    curnode->data = "Undefined Property";
	    mode = WHAT_PROP;
	    }

	 }
      /* WHAT_PROP distinguishes it from our guess? The node to hold this
	 information is created ahead of time in WHAT_IS_IT mode so we simply
	 set the data member. */
      else if (mode == WHAT_PROP)
	 {
	 curnode->data = cmd;
	 mode = RESET;
	 cout << "I have now learned that property. You may type GUESS again,\n"
	      << "or HELP for more options.\n";
	 }

      /* IS_IT_A We have to guess, either we're at a leaf node and we're guess 
	 thing for the thing, or we're at an internal node and we're describing 
	 a feature to make our next decision. */
      else if (mode == IS_IT_A)
	 {

	 /* We've guessed right */
	 if (toupper(cmd[0]) == 'Y')
	    {
	    /* ... and we're at a leaf */
	    if (curnode->isleaf())
	       {
	       cout << "Good game!\n";
	       curnode = root;
	       mode = NORMAL;
	       }
	    else // we're at an internal node, describe + ask again. 
	       {
	       curnode = curnode->right;
	       if (curnode->isleaf())
		  cout << "Is it a[n]... " << curnode->data << "?" << endl;
	       else
		  cout << "Does it have a[n] " << curnode->data << "?" << endl;
	       }
	    
	    }

	 /* We've guessed wrong. Not a problem if we're at an internal node. */
	 else if (toupper(cmd[0]) == 'N')
	    {
	    cout << "Oh.\n";

	    /* Things are kosher, this is an internal node */
	    if (!curnode->isleaf())
	       {
	       curnode = curnode->left;
	       cout << "Is it a[n] " << curnode->data << "? " << endl;
	       }
	    else // we've no idea it is.
	       mode = WHAT_IS_IT;
	    }

	 /* We wanted Y/N and they gave us neither. */
	 else 
	    cout << "Yes or No? Yae or Nae? Yesum or Nosir?\n";

	 }

      /* All modes have been handled, now provide processing for system 
	 commands. this had to be done to integrate the loading/saving 
	 features. */
      else 
	 {
	 if (cmds.size() == 1)
	    /* one word commands */
	    {
	    if (toupper(cmds[0]) == "QUIT")
	       break;

	    if (toupper(cmds[0]) == "HELP")
	       cout << "Primary Commands: (For use outside the Q&A routine)\n"
		    << "HELP, QUIT, LOAD, SAVE, GUESS\n" << endl
		    << "For more specific instructions, Type HELP <COMMAND>\n";
		    
	    if (toupper(cmds[0]) == "LOAD")
	       {
	       if (filename != "")
		  {
		  deallocate_binary_tree(root);
		  root = load_file(filename);

		  if (root == NULL)
		     cout << "Error loading '" << filename << "'.\n";
		  else
		     cout << "File loaded. (TODO-For real).\n";
		  }
	       else
		  cout << "No filename specified. Try LOAD <filename>.\n";
	       }
	    
	    if (toupper(cmds[0]) == "SAVE")
	       {
	       if (filename != "")
		  if (binary_tree_save_file(filename, root))
		     cout << "Could not save to '" << filename << "'.\n";
		  else
		     cout << "File saved to '" << filename << "'.\n";	       
	       else
		  cout << "No filename specified. Try SAVE <filename>.\n";
	       }
	    
	    /* Start off the guessing process. If this is an empty tree
	       we just ask for an item and return to the system context. */
	    if (toupper(cmds[0]) == "GUESS")
	       {
	       if (curnode == NULL)
		  {
		  prompt = "What is it?> "; 
		  mode = WHAT_IS_IT;
		  }
	       
	       else if (curnode->isleaf())
		  {
		  cout << "Is it a[n]... " << curnode->data << "?" << endl;
		  prompt = "Y/N?> ";
		  mode = IS_IT_A;
		  }
	       else // it is an internal node.
		  {
		  cout << "Does it have a[n] " << curnode->data << "?" << endl;
		  mode = IS_IT_A;
		  }
	       }
	    }
	 else if (cmds.size() == 2)
	    /* two word commands */
	    {
	    if (toupper(cmds[0]) == "SAVE")
	       {
	       if (binary_tree_save_file(cmds[1], root))
		  cout << "Error saving file '" << cmds[1] << "'." << endl;
	       else 
		  cout << "Data saved to '" << cmds[1] << "'." << endl;
	       }

	    if (toupper(cmds[0]) == "LOAD")
	       {
	       deallocate_binary_tree(root);
	       root = load_file(cmds[1]);

	       if (root == NULL)
		  cout << "Failed to load '" << cmds[1] << "'." << endl;
	       else
		  cout << "Successfully loaded '" << cmds[1] << "'." << endl;
	       }

	    if (toupper(cmds[0]) == "HELP")
	       {
	       if (toupper(cmds[1]) == "GUESS")
		  cout << "The GUESS command starts the guessing routine which\n"
		       << "continues until a leaf node is reached and the user\n"
		       << "describes the new object/property.\n";
	       else if (toupper(cmds[1]) == "LOAD")
		  cout << "The LOAD command attempts to load the binary tree from a file.\n"
		       << "If the program was called from the command-line with a file\n"
		       << "specified, this is the default location.\n"
		       << "  In the absence of a default, or at any time, a file may be\n"
		       << "specified by typing the filename after the LOAD command:\n"
		       << "LOAD <FILENAME>\n";
	       else if (toupper(cmds[1]) == "SAVE")
		  cout << "The SAVE command attempts to save the binary tree to a file.\n"
		       << "If the program was called from the command-line with a file\n"
		       << "specified, this is the default location.\n"
		       << "  In the absence of a default, or at any time, a file may be\n"
		       << "specified by typing the filename after the SAVE command:\n"
		       << "SAVE <FILENAME>\n";
	       else
		  cout << "Sorry, help is not available on that topic.\n";
	       }
	    }
	 }

      /* Set prompt based on context */
      switch (mode)
	 {
	 case WHAT_IS_IT:
	    prompt = "What is it?> ";
	    break;
	 case IS_IT_A:
	    prompt = "Y/N?> ";
	    break;
	 case WHAT_PROP:
	    prompt = "What property distinguishes it from a[n] " 
	       + curnode->left->data + "?> ";
	    break;
	 default:
	    prompt = "> ";
	 }
      cout << prompt;
   
      }

   // we're all done, clean up and die.
   deallocate_binary_tree(root);
   cout << "\nGoodbye!\n";

   return 0;
}
