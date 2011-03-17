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

using namespace std;

class Node {
public:
   string data;
   Node *parent, *right, *left;
   
   Node(string str="")
   {
      parent = right = left = NULL;
      data = str;
   }
   
   void insert_right_child(Node *child)
   {
      child->parent = this;
      child->right = this->right;
      this->right = child;
   }

   void insert_right_child(string data)
   {
      Node *newnode = new Node;
      newnode->data = data;
      insert_right_child(newnode);
   }
   
   void insert_left_child(Node *child)
   {
      child->parent = this;
      child->left = this->left;
      this->left = child;
   }

   void insert_left_child(string data)
   {
      Node *newnode = new Node;
      newnode->data = data;
      insert_left_child(newnode);
   }

}; // end Node definition

/***********************
 * HELPER FUNCTIONS
 **********************/

std::vector<std::string> split(const std::string line, 
			       const std::string split_pattern=",")
/* Split a string on `split_pattern` and return a vector containing the result. */
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


/***********************
 * FILE SAVING
 **********************/

string bt_node(Node *root)
{
   stringstream ret;
   ret << root << "|" << root->parent << "|" << root->left
       << "|" << root->right << "|" << root->data << endl;
   
   return ret.str();
}

string binary_tree_save_file(Node *root)
{
   if (root == NULL)
      return "";

   stringstream ret;
   ret << bt_node(root);
   ret << binary_tree_save_file(root->left);
   ret << binary_tree_save_file(root->right);
   return ret.str();
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
{

   ifstream ifile(filename.c_str());
   
   if (!ifile)
      return NULL;

   string line;
   map<string, string> nodes; // address->node index

   Node *root;

   while (std::getline(ifile, line))
      {
      vector<string>split_line = split(line, "|");
      nodes.insert(std::pair<string, string>(split_line[0],split_line[4]));
      }

   ifile.seekg(ios::beg);

   while (std::getline(ifile,line))
      {
      vector<string>split_line = split(line, "|");
      if (split_line[1] == "0")
	 {
	 
	 }
      }

}

int main()
{
   Node *root = new Node;
   root->data="ROOT!";
   root->insert_left_child("Hi, I'm the new left child!");
   root->insert_right_child("And I'm the right!");
   
   cout << root->right->data << endl;
   cout << root->left->data << endl;

   cout << binary_tree_save_file(root);

   load_file("data.txt");
}
