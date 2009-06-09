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

#ifndef _dnt_btree_h
#define _dnt_btree_h

#include "btreecell.h"

/*! A binary tree implementation  */
class bTree
{
   public:
      /*! Constructor
       * \param canMerge -> true if merge cells with same key,
       *                    false if put it at right
       * \note -> the merge functionality is done every time where is 
       *          inserted a cell that already is at the tree */
      bTree(bool canMerge=true);

      /*! Destructor
       * \note -> the children class must call clearSubTree for 
       *          its root at its destructor  */
      virtual ~bTree();

      /*! Get the bTree root cell
       * \return root cell */
      bTreeCell* getRoot();

      /*! Dupplicate a cell (aka: alloc a new one with same values)
       * \param cell -> pointer to the cell to dupplicate */
      virtual bTreeCell* dupplicateCell(bTreeCell* cell) = 0;

      /*! Delete the actual cell 
       * \param cell -> the cell to delete */
      virtual void freeCell(bTreeCell* cell) = 0;

      /*! Insert a cell (or merge it to an existed one)
       * \param cell -> pointer to the cell to insert
       * \return -> real pointer to the inserted cell inner the tree
       * \note -> if a cell exists with compare == 0, 
       *          this function will merge the cells, deleting the
       *          one passed as parameter */
      bTreeCell* insert(bTreeCell* cell);

      /*! Remove the cell from the tree
       * \param cell -> cell to remove from tree 
       * \note -> the cell is freed after this procedure. */
      void remove(bTreeCell* cell);

      /*! Merge the current tree with another one
       * \param treeRoot -> root of the tree to merge to this one
       * \note -> the tree received as parameter remains unchanged 
       *          after the function execution */
      void mergeTree(bTreeCell* treeRoot);

      /*! Search for a cell
       * \param cell -> cell to search
       * \return -> pointer to the found cell, or NULL */
      bTreeCell* search(bTreeCell* cell);

   protected:
      /*! Clear a sub tree from memory
       * \param subRoot -> pointer to the subtree root */
      void clearSubTree(bTreeCell* subRoot);

   private:
      bool canMerge;   /**< If will merge with cells equal keys */

      bTreeCell* root; /**< The tree's base cell */
};

#endif
