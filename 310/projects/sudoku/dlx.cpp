#include <iostream>
#include <string>
#include <vector>

#define MAX_COL 1000
#define MAX_ROW 1000

using namespace std;

class dlx {
   struct Node {
      Node *header, *up, *down, *left, *right;
      string id;
      Node() 
      {
	 header = up = down = left = right = NULL;
	 id = "...";
      }

      void cover() 
      {
	 Node *column = header;
	 column->right->left = column->left;
	 column->left->right = column->right;

	 for (Node *row = column->down; row != column; row = row->down)
	    {
	    for (Node *right_node = row->right; 
		 right_node != row; 
		 right_node = right_node->right)
	       {
	       right_node->right->left = right_node->left;
	       right_node->left->right = right_node->right;
	       }
	    }
      }
      
      void uncover()
      {
	 Node *column = header;
	 
	 for (Node *row = column->down; row != column; row = row->down)
	    {
	    
	    for (Node *right_node = row->right; 
		 right_node != row;
		 right_node = right_node->right)
	       {
	       right_node->right->left = right_node;
	       right_node->left->right = right_node;
	       }
	    }
      }
   };
   
   Node *root;
   vector<Node*> columns;

public:
   dlx(int row_width)
   {
      columns.resize(row_width);

      for (int i = 0; i < row_width; i++)
	 {
	 cout << i << endl;
	 columns[i] = new Node;
	 columns[i]->right = columns[0];
	    
	 if (i)
	    {
	    columns[i]->left = columns[i-1];
	    columns[i]->left->right = columns[i];
	    }
	 else 
	    columns[i]->left = columns[i];

	 columns[i]->up = columns[i]->down = columns[i];

	 columns[i]->id = i+'0';
	 }

      root = columns[0];
   }

   void print_matrix()
   {

      for (int i = 0;
	   i < columns.size();
	   i++)
	 {
	    Node *column = columns[i];
	 cout << column << " '" << column->id << "'" <<  endl;
	 /*
	 for (Node *row = column->down; row != column; row = row->down)
	    {
	    for (Node *right_node = row->right; 
		 right_node != row; 
		 right_node = right_node->right)
	       {
	       cout << right_node << endl;
	       }
	    }
	 */
	 }
   }

   void insert_row(const string &row)
   /* row is a string of 1's and 0's. size() should be less 
      than or equal to the row_width. 
      if it is less than, we assume it is fixed by the least
      signifiant digits. */
   {
      Node *row_root = NULL;
      for (int i = row.size() - 1;
	   i >= 0;
	   i--)
	 {
	 if (row[i] == '0')
	    continue;
	 if (row[i] == '1')
	    {
	    if (row_root != NULL)
	       {
	       Node *old_right = row_root->right;
	       Node *n = new Node;

	       // -- set left + right links
	       row_root->right = n;
	       n->header = row_root->header->right;
	       old_right->left = n;

	       // -- set the down links
	       n->down = n->header;

	       // -- set the up link
	       n->up = n->header->up;
	       n->down->up = n;
	       n->header->up = n;
	       }
	    else
	       {
	       row_root = new Node;
	       row_root->header = columns[i];
	       row_root->right = row_root;
	       row_root->left = row_root;

	       // -- set the down link
	       row_root->down = columns[i];

	       // -- set the up link.
	       row_root->up = columns[i]->up;
	       row_root->down->up = row_root;
	       }
	    }
	 }
   }
};

int main()
{
   dlx foo(8);
   foo.insert_row("10110001");
   foo.print_matrix();
   return 0;
}
