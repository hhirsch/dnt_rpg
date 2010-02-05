/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#ifndef _dnt_astar_h
#define _dnt_astar_h

#include "../map/map.h"
#include "../etc/list.h"
#include "pattAgent.h"

#include <SDL/SDL_opengl.h>

/*! The A* Point Class */
class pointStar: public dntListElement
{
   public:
      GLfloat x,           /**< Way Point X Coordinate */
              z;           /**< Way Point Z Coordinate */
      GLfloat heuristic;   /**< Calue of the heuristic to this point */
      GLfloat gone;        /**< How much percorred to this point */
      GLfloat parentX,     /**< Node parent X position */
              parentZ;     /**< Node parent Z position */
};

/*! The A* Point List Class */
class listStar: public dntList
{
   public:
      /*! Constructor */
      listStar();
      /*! Destructor */
      ~listStar();

      /*! Insert Node on List 
       * \param x -> x map position
       * \param z -> z map position
       * \param gone -> distance travelled
       * \param heuristic -> heuristic to the goal 
       * \param parentX -> X position of the parent
       * \param parentZ -> Z position of the parent */
      pointStar* insert(GLfloat x, GLfloat z, GLfloat gone, GLfloat heuristic,
                         GLfloat parentX, GLfloat parentZ);
      /*! Remove Node from list 
       * \param node -> node to be removed */
      void remove(pointStar* node);
      /*! Find a specific node on list
       * \param x -> node X position
       * \param z -> node Z position
       * \return pointer to the founded node. NULL if not found */
      pointStar* find(GLfloat x, GLfloat z);
      /*! Find the node with lowest value
       * \return pointer to the founded node. NULL if not found */
      pointStar* findLowest();
      /*! Verify if the list is empty
       * \return true if empty, false otherwise */
      bool isEmpty(){return(total <= 0);};
      /*! return the size of the List */
      int size(){return(total);};

   protected:
      /*! Free an element (deleting it)
       * \param obj -> object to free */
      void freeElement(dntListElement* obj);

};

#define ASTAR_STATE_OTHER     0  /**< A* Other State */
#define ASTAR_STATE_RUNNING   1  /**< A* Running State (searching for path) */
#define ASTAR_STATE_FOUND     2  /**< A* Found Path State */   
#define ASTAR_STATE_NOT_FOUND 3  /**< A* Not Found Path State */

/*! A* implementation. The A* will be searching few nodes per cycle. 
 *  When running, state is RUNNING. When end running, state is
 *  FOUND or NOT_FOUND, based on the result of the search. */
class aStar
{
   public:
      /*! Constructor */
      aStar();
      /*! Destructor */
      ~aStar();

      /*! Define the actual opened map 
       *  \param acMap -> current opened map. */
      void defineMap(Map* acMap);

      /*! Calls A* thread to find path
       * \param actor -> the character that will search for path
       * \param x -> destiny x position
       * \param z -> desired z position
       * \param stepSize -> size of the Step 
       * \param NPCs -> list of NPCs 
       * \param PCs -> list of PCs
       * \param fightMode -> true if is in the fight mode
       * \param forceCall -> true to force the call (there's a counter inner
       *                      the function that don't allow too often calls)
       * */
      void findPath(void* actor, GLfloat x, GLfloat z, GLfloat stepSize,
                    void* NPCs, void* PCs, bool fightMode, 
                    bool forceCall=false);

      /* Do the a* cycle (if is current searching for something)
       * \param fightMode -> true if is in the fight mode */
      void doCycle(bool fightMode);

      /*! Get the New Character Position, based on Path Found previously 
       * \param posX -> new X position
       * \param posZ -> new Z position
       * \param ori -> new orientation
       * \param fightMode -> true if is in the fight mode
       * \param run -> true if character is running or false if walking
       * \return true if can actualize, false otherwise */
      bool getNewPosition(GLfloat& posX, GLfloat& posZ, GLfloat& ori,
                          bool fightMode, bool run);

      /*! Gets the Destiny of The Character
       * \param destX -> X destiny Position
       * \param destZ -> Z destiny Position */
      void getDestiny(GLfloat& destX, GLfloat& destZ);

      /*! Draws the founded path */
      void drawPath();

      /*! Get Actual State of the aStar and put it on OTHER mode
       * \return state value if the state is nor RUNNING. */
      int getState();

      /*! Set character orientation value
       * \param ori -> new orientation value */
      void setOrientation(GLfloat ori);

      /*! Clear the a* state */
      void clear();

      /*! Toggle the a* to force the next call */
      void forceNextCall();

   private:

      /*! Clear search structures */
      void clearSearch();

      Map* actualMap;           /**< Pointer to opened Map */
      GLfloat destinyX,         /**< Destiny X position */
              destinyZ;         /**< Destiny Z position */
      pattAgent* patt;          /**< The Pattern Agent to Follow created path */
      int state;                /**< Internal State of the aStar */
      void* curActor;           /**< Current Actor */
      void* pcs;                /**< PCs list */
      void* npcs;               /**< NPCs list */
      GLfloat curStepSize;      /**< Current Step Size */
      bool walking;             /**< True if the character is walking with
                                     at a* at the momment. */

      listStar* opened;         /**< Opened nodes list */
      listStar* closed;         /**< Closed nodes list */

      GLfloat dX, 
              dZ, 
              diagonal, 
              orthogonal, 
              heuristic;

      GLuint lastCallTime;      /**< Last Time when Called Search */
};


#endif

