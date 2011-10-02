/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "money.h"
#include "../engine/util.h"

/***********************************************************************
 *                               Constructor                           *
 ***********************************************************************/
money::money(): object(DNT_MONEY_OBJECT, "")
{
   cost = 0;
}

/***********************************************************************
 *                                Destructor                           *
 ***********************************************************************/
money::~money()
{
}

/***********************************************************************
 *                                 quantity                            *
 ***********************************************************************/
int money::quantity()
{
   return(cost);
}

/***********************************************************************
 *                              removeQuantity                         *
 ***********************************************************************/
bool money::removeQuantity(int qty)
{
   /* Only remove if have the quantity */
   if(cost >= qty)
   {
      cost -= qty;
      calculateWeight();
      setState(cost);
      return(true);
   }
   return(false);
}

/***********************************************************************
 *                              addQuantity                         *
 ***********************************************************************/
void money::addQuantity(int qty)
{
   cost += qty;
   calculateWeight();
   setState(cost);
}

/***********************************************************************
 *                               setQuantity                           *
 ***********************************************************************/
void money::setQuantity(int qty)
{
   cost = qty;
   calculateWeight();
   setState(cost);
}

/***********************************************************************
 *                             calculateWeight                         *
 ***********************************************************************/
void money::calculateWeight()
{
   weight = (cost / 100.0f);
}

