
#include "btreecell.h"
#include <stdlib.h>

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
bTreeCell::bTreeCell()
{
   this->left = NULL;
   this->right = NULL;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
bTreeCell::~bTreeCell()
{
}

/***********************************************************************
 *                          Get and Set Left                           *
 ***********************************************************************/
bTreeCell* bTreeCell::getLeft()
{
   return(this->left);
}

void bTreeCell::setLeft(bTreeCell* left)
{
   this->left = left;
}

/***********************************************************************
 *                          Get and Set Right                          *
 ***********************************************************************/
bTreeCell* bTreeCell::getRight()
{
   return(this->right);
}

void bTreeCell::setRight(bTreeCell* right)
{
   this->right = right;
}
