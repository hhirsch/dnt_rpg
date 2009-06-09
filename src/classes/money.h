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

#ifndef _dnt_money_h
#define _dnt_money_h

#include "thing.h"
#include "object.h"

#define DNT_MONEY_OBJECT "models/objetos/itens/money/money.dcc"

/*! money class is the money representation on DNT  */
class money: public object
{
   public:
      /*! Constructor  */
      money();

      /*! Destructor */
      ~money();

      /*! Get the current money quantity (value)
       * \return -> current money value */
      int quantity();

      /*! Remove quantity from current money
       * \param qty -> quantity to remove 
       * \return -> true if can remove, false if not enought money */
      bool removeQuantity(int qty);

      /*! Add a money quantity to the current money
       * \param qty -> quantity to add to current */
      void addQuantity(int qty);

      /*! Set a money quantity to the current money
       * \param qty -> quantity to set as current */
      void setQuantity(int qty);

      /*! Virtual method to call dead animation */
      void callDeadAnimation(){};

      /*! Virtual method to call attack animation */
      void callAttackAnimation(){};

      /*! Virtual method to call attack animation */
      void callIdleAnimation(){};

   protected:

      /*! Calculate the weight */
      void calculateWeight();

};


#endif

