
#include "btree.h"
#include <iostream>
using namespace std;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
bTree::bTree(bool canMerge)
{
   this->root = NULL; 
   this->canMerge = canMerge;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
bTree::~bTree()
{
   this->root = NULL;
}

/***********************************************************************
 *                              getRoot                                *
 ***********************************************************************/
bTreeCell* bTree::getRoot()
{
   return(this->root);
}

/***********************************************************************
 *                              insert                                 *
 ***********************************************************************/
bTreeCell* bTree::insert(bTreeCell* cell)
{
   bTreeCell* pos = this->root; /* Position to insert */
   int cmpValue;                /* Comparasion Value */
   bool inserted = false;       /* Insertion flux controll */

   if(this->root == NULL)
   {
      /* No actual root, so this is the new one */
      this->root = cell;
      return(cell);
   }

   while(!inserted)
   {
      /* Compare the two cells  */
      cmpValue = cell->compare(pos);

      if( (cmpValue == 0) && (this->canMerge) )
      {
         /* Is the same, must merge the cell */
         pos->merge(cell);

         /* Free the created one, cause it's no more needed  */ 
         freeCell(cell);

         inserted = true;
         return(pos);
      }
      else if(cmpValue < 0)
      {
         /* Must be at left */
         if(!pos->getLeft())
         {
            /* No one at left, insert here */
            pos->setLeft(cell);
            inserted = true;
         }

         /* Go to the left son  */
         pos = pos->getLeft();
      }
      else 
      {
         /* Must be at right */
         if(!pos->getRight())
         {
            /* No one at right, insert here  */
            pos->setRight(cell);
            inserted = true;
         }

         /* Go to the right son */
         pos = pos->getRight();
      }
   }

   return(cell);
}

/***********************************************************************
 *                              remove                                 *
 ***********************************************************************/
void bTree::remove(bTreeCell* cell)
{
   int cmpValue;
   bTreeCell* father = NULL; 
   bTreeCell* pos = NULL; 

   if(cell)
   {
      /* Treat special root cell case  */
      if(cell == this->root)
      {
         if(cell->getLeft())
         {
            /* Make new root the left child  */
            this->root = cell->getLeft();
         }
         else
         {
            /* Make the root the right child or NULL  */
            this->root = cell->getRight();
         }

         /* Root Cell has no father  */
         father = NULL;
      }
      else
      {
         /* Search for the father  */
         pos = this->root;
         father = NULL;
         while(pos != cell)
         {
            father = pos;
            cmpValue = cell->compare(pos);

            /* Verify where to search  */ 
            if(cmpValue <= 0)
            {
               pos = pos->getLeft();
            }
            else
            {
               pos = pos->getRight();
            }

            /* Verify cell not found  */
            if(!pos)
            {
               cerr << "Error: No cell " << cell 
                    << " at tree to remove!" << endl;
               /* Only free it!  */
               freeCell(cell);
               return;
            }
         }
      }

      /* Select the child to up  */
      if(cell->getLeft())
      {
         /* Up the child  */
         if(father)
         {
            cmpValue = cell->compare(father);
            if(cmpValue <= 0)
            {
               father->setLeft(cell->getLeft());
            }
            else
            {
               father->setRight(cell->getLeft());
            }
         }

         /* Get the right position to insert the right subtree  */
         if(cell->getRight())
         {
            pos = cell->getLeft();
            while(pos->getRight() != NULL)
            {
               pos = pos->getRight();
            }

            /* Insert the subtree  */
            pos->setRight(cell->getRight());
         }
      }
      else if(cell->getRight())
      {
         /* Up the child  */
         if(father)
         {
            cmpValue = cell->compare(father);
            if(cmpValue <= 0)
            {
               father->setLeft(cell->getRight());
            }
            else
            {
               father->setRight(cell->getRight());
            }
         }

         /* No more to do, case the cell hasn't left childs  */
      }
  
      /* Finally, free the cell memory  */
      freeCell(cell);
   }
}

/***********************************************************************
 *                              search                                 *
 ***********************************************************************/
bTreeCell* bTree::search(bTreeCell* cell)
{
   bTreeCell* pos = this->root;
   int res;

   while(pos != NULL) 
   {
      res = cell->compare(pos);

      if(res == 0)
      {
         /* Here it is! */
         return(pos);
      }
      else if(res < 0)
      {
         /* Must be at left  */
         pos = pos->getLeft();
      }
      else
      {
         /* Must be at right  */
         pos = pos->getRight();
      }
   }

   return(NULL);
}

/***********************************************************************
 *                           clearSubTree                              *
 ***********************************************************************/
void bTree::clearSubTree(bTreeCell* subRoot)
{
   if(subRoot)
   {
      /* Clear Left Subtree  */
      this->clearSubTree(subRoot->getLeft());

      /* Clear Right Subtree  */
      this->clearSubTree(subRoot->getRight());

      /* Free the sub-root cell */
      freeCell(subRoot);
   }
}

/***********************************************************************
 *                             mergeTree                               *
 ***********************************************************************/
void bTree::mergeTree(bTreeCell* treeRoot)
{
   /* Walk trought all the received tree, inserting its values at
    * the current one */
   if(treeRoot)
   {
      /* Dupplicate the cell (to avoid double frees =^P )  */
      bTreeCell* cell = dupplicateCell(treeRoot);

      /* Insert (or merge) the value at the tree  */
      this->insert(cell);

      /* Call to the left  */
      this->mergeTree(treeRoot->getLeft());

      /* Call to the right */
      this->mergeTree(treeRoot->getRight());
   }
}

