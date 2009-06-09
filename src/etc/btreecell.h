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

#ifndef _dnt_btreecell_h
#define _dnt_btreecell_h

/*! A binary tree cell definition  */
class bTreeCell
{
   public:
      /*! Constructor */
      bTreeCell();
      /*! Desctuctor */
      virtual ~bTreeCell();

      /*! Gets the left son 
       * \return the cell's left son */
      bTreeCell* getLeft();
      /*! Sets the left son 
       * \param cell -> reference to the son to be */
      void setLeft(bTreeCell* cell);
      /*! Gets the right son 
       * \return the cell's right son */
      bTreeCell* getRight();
      /*! Sets the right son 
       * \param right -> reference to the son to be */
      void setRight(bTreeCell* right);

      /*! Compare with another cell
       * \param cell -> pointer to cell to compare this one to
       * \return == 0 if equal, < 0  if lesser or > 0 if greater */
      virtual int compare(bTreeCell* cell) = 0;

      /*! Merge the current bTreeCell with another
       * \param cell -> cell to merge the current one with */
      virtual void merge(bTreeCell* cell) = 0;

   private:
      bTreeCell* left;   /**< The cell's left son */
      bTreeCell* right;  /**< The cell's right son */ 
};

#endif

