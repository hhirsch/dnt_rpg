/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

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
