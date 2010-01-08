/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2010 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_split_window
#define _dnt_split_window

#include <libintl.h>
#include "../classes/inventory.h"
#include "../gui/farso.h"
#include "character.h"
#include "modstate.h"
#include "util.h"

/*! The splitWindow class defines a window used to split an object by its 
 * quantity (for example: removing money from a money quantity to give it 
 * away to someone, spliting an ammo, to throw it away, etc.) */
class splitWindow
{
   public:
      /*! Constructor to thrown the desired object quantity away.
       * \param inter -> pointer to the used guiInterface 
       * \param o -> pointer to the object to split
       * \param usedEngine -> pointer to the current in-use engine
       * \param pX -> X coordinate to put the object
       * \param pZ -> Z coordinate to put the object */
      void open(guiInterface* inter, object* o, void* usedEngine, 
            GLfloat pX, GLfloat pZ);
      /*! Constructor to give the desired object quantity to a character. 
       * \param inter -> pointer to the used guiInterface 
       * \param o -> pointer to the object to split
       * \param t -> pointer to the character to receive the object */
      void open(guiInterface* inter, object* o, character* t);

      /*! Close the window, if is opened */
      void close();

      /*! Verify if window is still opened
       * \return true if window is openned, false otherwise */
      bool isOpen();

      /*! Treat Events on Window. 
       * \param guiObj -> last GUI object
       * \param eventInfo -> last GUI Event 
       * \param X -> character that own the object X position
       * \param Z ->     "       "   "    "      "     Z    "  
       * \return true if event got, false otherwise. */
      bool treat(guiObject* guiObj, int eventInfo, GLfloat X, GLfloat Z);

   protected:

      /*! Open the window */
      void open();

      /*! Update the quantity text */
      void updateText();

      static object* obj;            /**< The object to split */
      static GLfloat posX;           /**< X position to throw object */
      static GLfloat posZ;           /**< Z position to throw object */
      static character* target;      /**< Character to receive the object */

      static int curQty;             /**< Current quantity */
      static int delta;              /**< Delta variation to the quantity */

      static void* curEngine;        /**< Current used engine */

      static guiInterface* interf;   /**< Current interface used */
      static window* intWindow;      /**< Internal Window */
      static textBox* qtyText;       /**< The quantity text */
      static button* moreButton;     /**< The more button */
      static button* lessButton;     /**< The less button */
      static button* okButton;       /**< The ok button */
      static button* cancelButton;   /**< The cancel button */
};


#endif

