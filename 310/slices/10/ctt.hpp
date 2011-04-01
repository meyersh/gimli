#ifndef CTT_HPP
#define CTT_HPP
#include <string>

using namespace std;

//*********************************************************

template <class A>
struct cttNode
{
   char cValue,
      hasIndex;
   A index;

   cttNode *par,//parent child 
      *lc, //left child
      *cc, //center child
      *rc; //right child
		  
   cttNode (){cValue=NULL; hasIndex=0; par=lc=cc=rc=NULL;}
};

//*********************************************************

template <class B>
class ctt //(character ternary tree)
{
   cttNode<B> *root;	//the root node

   cttNode<B> *deleteNode(cttNode<B> *nd); // Delete nd and return ptr to parent

   // Starting with node nd, seek node that has char to match cval.  Return ptr
   //     to node if found, NULL if empty tree, and ptr to node that should have
   //     a new child added to it otherwise
   cttNode<B> *matchChar(cttNode<B> *nd, char cval);

public: 
   ctt(){root = NULL;}
   ~ctt(){clear();}
   void clear();

   // Insert regardless of whether or not we have one already there
   void insert(const string &key, const B &index); 

   // Insert only if one does not already exist, Return 0 if insert, NZ if not
   int insertIfNew(const string &key, const B &index);

   // Delete this key and its index from the tree (or do nothing if not found)
   void deleteKey(const string &key); 

   int getIndex(const string &key, B &index); // Return 0 on success, NZ if fail
};

//*********************************************************

template <class C>
void ctt<C>::clear()
{
   cttNode<C> *cur;
   if(root == NULL) // there is nothing in it so just return
      return;
	
   cur=root;
   while(cur != NULL)
      {
      if(cur->lc !=NULL) // it has a left child
	 cur=cur->lc;
      else if(cur->cc !=NULL) // it has a center child
	 cur=cur->cc;
      else if (cur ->rc !=NULL) // it has a right child
	 cur=cur->rc;
      else
	 cur=deleteNode(cur)//cur gets the parent of the node that we deleted
	    }//end while
}//end function

//***************************************************************

template <class D>
cttNode<D> *ctt<D>::deleteNode(cttNode<D> *nd)
{
   cttNode<D> *p; //parent
   if ((nd->lc) || (nd->cc) || (nd->rc)) // Node has children - don't delete
      return NULL;
   p=nd->par;
   //check which child is to be deleted and set the pointer to it to NULL
   if(p->lc==nd)
      p->lc=NULL;
   else if (p->cc ==nd)
      p->cc=NULL;
   else
      p->rc=NULL;
   delete nd;
   return p;
}

//*****************************************************************

// Starting with node nd, seek node that has char to match cval.  Return ptr
//     to node if found, NULL if empty tree, and ptr to node that should have
//     a new child added to it otherwise
template<class B>
cttNode<B> *ctt<B>::matchChar(cttNode<B> *nd, char cval)
{
   if (nd == NULL) // Root of subtree is NULL
      return NULL;

   if (nd->cValue == cval) // Matches the root
      return nd;
   
   while (nd->cValue != cval) // Keep trying to find it
      {
      if (cval < nd->cValue)
	 {
	 if (nd->lc == NULL)
	    break;
         
	 nd = nd->lc;
	 }
      else // cval > nd->cValue
	 {
	 if (nd->rc == NULL)
	    break;
         
	 nd = nd->rc;
	 }
      } // End while
   return nd;
}

template<class B> 
cttNode<B> *ctt<B>::int getIndex(const string &key, B &index)
/* DESCR: Verifies the existance of a key.
   PARAM: const string "key" and an `B` reference to hold the index.
   RETUR: NZ on fail. */
{
   /* hasIndex; cValue */
   cttNode<B> *cur_node = root;

   /* try to find each character of the key */
   for (int i = 0; i < key.size(); i++)
      {
      cur_node = matchChar(cur_node, key[i]); // set current node to first match
      if (cur_node == NULL || cur_node->cValue != key[i])
	 return -1; // search failed.
      }

   /* now cur_node->cValue should == the last char in the key */
   if (cur_node->cValue == key[key.size()-1] 
       && cur_node->hasIndex)
      index = cur_node->index;

   return -2;

}

template<class B>
void ctt<B>::insert(const string &key, const B &index)
/* DESCR: Insert an index (even if there is already a value there. 
   PARAM: `key' to insert and index.
   RETUR: void */
{
   return;
}

template<class B>
int ctt<B>::insertIfNew(const string &key, const B &index)
/* DESCR: Insert a value only if the key does not exist. 
   PARAM: `key' and value
   RETUR: NZ if we did not insert anything */
{
}

template<class B>
void ctt<B>:: deleteKey(const string &key)
/* DESCR: Delete a key and its index from the tree. (Do nothing if the 
   key doesn't exist)
   PARAM: key to delete
   RETUR: void */
{
   /* hasIndex; cValue */
   cttNode<B> *cur_node = root;
   
   /* try to find each character of the key */
   for (int i = 0; i < key.size(); i++)
      {
      cur_node = matchChar(cur_node, key[i]); // set current node to first match
      if (cur_node == NULL || cur_node->cValue != key[i])
	 return; // search failed.
      }

   /* now cur_node->cValue should match the last character in the key, 
    and it should have the hasIndex flag set. */
   if (cur_node->cValue == key[key.size()-1] 
       && cur_node->hasIndex)
      {

      cur_node->hasIndex = 0; /* no longer an index */

      // delete as far as we can.
      for (cur_node = cur_node->par; 
	   cur_node != NULL; 
	   cur_node = cur_node->parent)
	 {
	 /* Count how many children we have. */
	 int children = 
	    cur_node->rc ? 1 : 0
	    +
	    cur_node->cc ? 1 : 0
	    + 
	    cur_node->lc ? 1 : 0;

	 if (children == 0) 
	    {
	    /* we're a leaf..? */
	    if (cur_node->par == NULL)
	       delete cur_node;

	 /* There is only one child; delete it unless it's an index. */
	 if (children == 1) 
	    {
	    cttNode<B> node_to_delete = NULL; 

	    if (cur_node->rc && !cur_node->rc->hasIndex)
	       {
	       node_to_delete = cur_node->rc;
	       cur_node->rc = NULL;
	       }

	    else if (cur_node->cc && !cur_node->cc->hasIndex)
	       {
	       node_to_delete = cur_node->cc;
	       cur_node->cc = NULL;
	       }

	    else if (cur_node->lc && !cur_node->lc->hasIndex)
	       {
	       node_to_delete = cur_node->lc;
	       cur_node->lc = NULL;
	       }

	    if (node_to_delete)
	       delete node_to_delete;

	    }

	 else if (children > 1)
	    break; /* there are other keys from this... */

	   
	 if (cur_node->cc  cur_node->cc->hasIndex)
	    break; /* This is an index, so we can't delete any more. */

	 delete cur_node->cc;
	 cur_node->cc = NULL;
	    
	 }
      }

   return;

}

#endif

