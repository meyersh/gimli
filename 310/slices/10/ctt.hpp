#ifndef CTT_HPP
#define CTT_HPP

#include <string>
#include <iostream>

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
   cttNode (char cValue)
   {
      this->cValue = cValue;
      hasIndex = 0;
      par = lc = cc = rc = NULL;
   }
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
	 cur=deleteNode(cur); //cur gets the parent of the node that we deleted
      } //end while
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
   if(p && p->lc == nd)
      p->lc=NULL;
   else if (p && p->cc == nd)
      p->cc=NULL;
   else if (p && p->rc == nd)
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
int ctt<B>::getIndex(const string &key, B &index)
/* DESCR: Verifies the existance of a key.
   PARAM: const string "key" and an `B` reference to hold the index.
   RETUR: NZ on fail. */
{
   /* hasIndex; cValue */
   cttNode<B> *cur_node = matchChar(root, key[0]);

   /* try to find each character of the key */
   for (int i = 1; i < key.size(); i++)
      {
      if (cur_node == NULL)
	 return -3; // null.

      if (cur_node->cc)
	 cur_node = matchChar(cur_node->cc, key[i]);

      if (cur_node == NULL || cur_node->cValue != key[i])
	 {
	 return -1; // search failed.
	 }
      }

   /* now cur_node->cValue should == the last char in the key */
   if (cur_node->cValue == key[key.size()-1] 
       && cur_node->hasIndex)
      {
      index = cur_node->index;
      return 0;
      }
   return -2;


}

template<class B>
void ctt<B>::insert(const string &key, const B &index)
/* DESCR: Insert an index (even if there is already a value there. 
   PARAM: `key' to insert and index.
   RETUR: void */
{
   cttNode<B> *cur_node = matchChar(root, key[0]);
   
   /* Empty tree. */
   if (cur_node == NULL)
      {
      root = cur_node = new cttNode<B>(key[0]);
      }

   /* First character is unique. */
   else if (cur_node->cValue != key[0])
      {
      if (key[0] < cur_node->cValue)
	 {
	 cur_node->lc = new cttNode<B>(key[0]);
	 cur_node = cur_node->lc;
	 }
      if (key[0] > cur_node->cValue)
	 {
	 cur_node->rc = new cttNode<B>(key[0]);
	 cur_node = cur_node->rc;
	 }
      }

   /* Special case: When the key is only 1 character long, 
      we're done now. */
   if (key.size() == 1)
      {
      cur_node->hasIndex = true;
      cur_node->index = index;
      }

   /* Now, add the rest of the key */
   for (int i = 1; i < key.size(); i++)
      {
      /* cur_node == PARENT of whatever we're trying to insert. */
      if (cur_node->cc == NULL)
	 {
	 cur_node->cc = new cttNode<B>(key[i]);
	 cur_node = cur_node->cc;
	 }
      else 
	 {
	 cur_node = cur_node->cc;
	 /* Everytime we move down, make sure we've matchChar'd */
	 if (cur_node->cValue != key[i])
	    cur_node = matchChar(cur_node, key[i]);

	 if (key[i] < cur_node->cValue)
	    {
	    cur_node->lc = new cttNode<B>(key[i]);
	    cur_node = cur_node->lc;
	    }
	 else if (key[i] > cur_node->cValue)
	    {
	    cur_node->rc = new cttNode<B>(key[i]);
	    cur_node = cur_node->rc;
	    }
	 else if (cur_node->cValue == key[i])
	    {
	    /* Don't think we have to do anything here. */
	    }
	 }

      if (i == key.size()-1)
	 {
	 //cout << "We're at the last one. '" << key[i] << cur_node->cValue << "'\n";
	 cur_node->hasIndex = true;
	 cur_node->index=index;
	 }
      }
   
}

template<class B>
int ctt<B>::insertIfNew(const string &key, const B &index)
/* DESCR: Insert a value only if the key does not exist. 
   PARAM: `key' and value
   RETUR: NZ if we did not insert anything */
{
   B oldindex;
   int ret = getIndex(key, oldindex);
   if (ret)
      insert(key, index);
   
}

template<class B>
void ctt<B>:: deleteKey(const string &key)
/* DESCR: Delete a key and its index from the tree. (Do nothing if the 
   key doesn't exist)
   PARAM: key to delete
   RETUR: void */
{
   /* hasIndex; cValue */
   cttNode<B> *cur_node = matchChar(root, key[0]);

   /* try to find each character of the key */
   for (int i = 1; i < key.size(); i++)
      {
      if (cur_node == NULL)
	 return; // null.

      if (cur_node->cc)
	 cur_node = matchChar(cur_node->cc, key[i]);

      if (cur_node == NULL || cur_node->cValue != key[i])
	 {
	 return; // search failed.
	 }
      }

   /* now cur_node->cValue should == the last char in the key */
   if (cur_node->cValue == key[key.size()-1] 
       && cur_node->hasIndex)
      {
      cur_node->hasIndex = false; // we found it!
      }
   else
      return; // didn't find it...
    

   /* now cur_node->cValue should match the last character in the key, 
      and it should have the hasIndex flag set. */

   cttNode<B> *trail_ptr = NULL;
  
   // delete as far as we can.
   for (cur_node = cur_node->par; 
	cur_node != NULL; 
	trail_ptr = cur_node, 
	   cur_node = cur_node->par)
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
	 /* we're a leaf..? We must be the LAST node. */
	 if (cur_node->par == NULL)
	    {
	    delete cur_node;
	    root = cur_node = NULL;
	    }
	 }
      
      /* There is only one child; delete it unless it's an index. */
      if (children == 1) 
	 {
	 
	 if (cur_node->lc && !cur_node->lc->hasIndex)
	    {
	    delete cur_node->lc;
	    cur_node->lc = NULL;
	    }
	 else if (cur_node->cc && !cur_node->cc->hasIndex)
	    {
	    delete cur_node->cc;
	    cur_node->cc = NULL;
	    }
	 else if (cur_node->rc && !cur_node->rc->hasIndex)
	    {
	    delete cur_node->rc;
	    cur_node->rc = NULL;
	    }
	 
	 }
      
      else if (children == 2 || children == 3) /* delete the child we 
						  just CAME from. then stop. */
	 {
	 if (cur_node->lc == trail_ptr)
	    cur_node->lc = NULL;
	 
	 else if (cur_node->lc == trail_ptr)
	    cur_node->lc = NULL;
	 
	 else if (cur_node->rc == trail_ptr)
	    cur_node->rc = NULL;
	 
	 delete trail_ptr;
	 break;
	 }
      
      }

return;

}

#endif

