#include <stack>

template<class K, class V> class ctt
{
public:
  struct cttNode {

    K key; /* value of this nodes' key  */
    V value; /* value of thise nodes' data */

    bool end_of_key; /* this is the end of a key */

    cttNode *parent,
      *left_child,
      *center_child,
      *right_child;

    cttNode()
    {
      key          = NULL;
      value        = NULL;
      parent       = NULL; 
      left_child   = NULL;
      center_child = NULL; 
      right_child  = NULL;
      end_of_key   = false;
    }
  }; /* end of cttNode type */

private:
  /* DATA MEMBERS */
  cttNode *root;

public:
  void delete_all()
  {
    std::stack<cttNode*> nodes;
    nodes.push(root);
    cttNode *curr = nodes.top();

    while (!nodes.empty())
      {
	curr = nodes.top();
	nodes.pop();

	if (curr->left_child != NULL)
	  nodes.push(curr->left_child);

	if (curr->right_child != NULL)
	  nodes.push(curr->right_child);

	if (curr->center_child != NULL)
	  nodes.push(curr->center_child);

	delete curr;
      }
  }

  void insert(K key, V value)
  {
  }

  void delete_node(K key)
  {
  }
}; /* end of ctt class */

int main()
{
  ctt<char, int> foo;
  return 0;
}
