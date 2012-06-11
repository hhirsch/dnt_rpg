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

#ifndef _dnt_door_h
#define _dnt_door_h

#define DOOR_STATUS_CLOSED      0      /**< If door is closed */
#define DOOR_STATUS_OPENED      1      /**< If door is opened */

#define DOOR_STATE_UNLOCKED     0
#define DOOR_STATE_LOCKED       1

#include "../etc/list.h"
#include "../classes/object.h"

/*! A door is an object that, obviously, opens and close passages */
class door: public dntListElement
{
   public:
      /*! Constructor */
      door();
      /*! Destructor */
      ~door();

      object* obj;             /**< pointer to door object */
      
      /*! open/close the door */
      void flip();
      /*! update door positions */
      void update();

      /*! Get the door status (opened or closed) */
      int getStatus(){return(status);};

   protected:
      int status;            /**< actual status (opened, closed) */
      float delta;           /**< actual delta */
};

/*! List of doors */
class doorList: public dntList
{
   public:
      /*! Constructor */
      doorList();
      /*! Destructor */
      ~doorList();

      /*! update all doors */
      void update();

   protected:
      /*! Free the door memory
       * \param obj -> door to delete */
      void freeElement(dntListElement* obj);
};


#endif

