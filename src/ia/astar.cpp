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

#include "astar.h"
#include "../engine/collision.h"
#include <math.h>

#define SEARCH_LIMIT   1000  /**< Max Nodes the aStar will search */
#define SEARCH_INTERVAL   5  /**< Interval of Nodes when aStar will sleep */
#define MIN_CALL        200  /**< Minimun time interval to call search again */


/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
aStar::aStar()
{
   actualMap = NULL;
   patt = new pattAgent(true);
   patt->defineDestiny(0, 0);
   patt->defineStepSize(0);
   patt->defineOrientation(0);
   patt->defineSight(0, 0);
   state = ASTAR_STATE_OTHER;
   lastCallTime = SDL_GetTicks();
   opened = NULL;
   closed = NULL;
   walking = false;
}

/****************************************************************
 *                         clearSearch                          *
 ****************************************************************/
void aStar::clearSearch()
{
   /*  Clear Closed Nodes */
   if(closed)
   {
      delete(closed);
      closed = NULL;
   }

   /* Clear Opened Nodes */
   if(opened)
   {
      delete(opened);
      opened = NULL;
   }

   /* Clear Variables */
   pcs = NULL;
   npcs = NULL;
   curStepSize = 1;
}

/****************************************************************
 *                         defineMap                            *
 ****************************************************************/
void aStar::defineMap(Map* acMap)
{
   if(acMap == actualMap)
   {
      /* No need to change the SAME map */
      return;
   }

   if(state == ASTAR_STATE_RUNNING)
   { 
       /* Stop the search */
       clearSearch();
       state = ASTAR_STATE_OTHER;
   }
   actualMap = acMap;
}

/****************************************************************
 *                         Destructor                           *
 ****************************************************************/
aStar::~aStar()
{
   /* Clear Search Structs */
   clearSearch();

   /* Clear Patter Structs */
   delete(patt);

   /* Empty the map pointer */
   actualMap = NULL;
}

/****************************************************************
 *                         forceNextCall                        *
 ****************************************************************/
void aStar::forceNextCall()
{
   lastCallTime = 0;
}

/****************************************************************
 *                            findPath                          *
 ****************************************************************/
void aStar::findPath(void* actor, GLfloat x, GLfloat z, GLfloat stepSize,
                     void* NPCs, void* PCs, bool fightMode, bool forceCall)
{
   GLuint actualTime = SDL_GetTicks();

   /* Verify time variation */
   if( (actualTime - lastCallTime >= MIN_CALL) || (forceCall))
   {
      lastCallTime = actualTime;

      /* Verify, if at fight mode, if not already walking */
      if((fightMode) && (walking))
      {
         /* Can't change the walk, so keep the current one */
         state = ASTAR_STATE_FOUND;
         return;
      }      

      /* Clear the current search (if one) */
      clearSearch();

      /* Clear the previous path */
      clear();

      /* Creates the new Search */
      opened = new listStar();
      closed = new listStar();
      state = ASTAR_STATE_RUNNING;

      /* Define Actor and Destiny */
      curActor = actor;
      destinyX = x;
      destinyZ = z;
      curStepSize = stepSize;
      pcs = PCs;
      npcs = NPCs;

      /* Verify the distance to the target. If too near, no need to walk. */
      GLfloat actualX = ((character*)actor)->scNode->getPosX();
      GLfloat actualZ = ((character*)actor)->scNode->getPosZ();
      dX = (x - actualX);
      dZ = (z - actualZ);
      GLfloat dist = sqrt( (dX*dX) + (dZ*dZ) );

      /* Verify if the actor is alive (dead things can't walk, right?) */
      if(!((character*)actor)->isAlive())
      {
         state = ASTAR_STATE_NOT_FOUND;
         return;
      }

      if(fightMode)
      {
         /* Verify if can walk */
         if(!((character*)actor)->getCanMove())
         {
             /* Can't move! */
             state = ASTAR_STATE_NOT_FOUND;
             return;
         }
      }

      /* Verify if can occupy the destiny */
      collision collisionDetect;
      collisionDetect.defineMap(actualMap, (characterList*)npcs, 
                                (characterList*)pcs);
      GLfloat varHeight=0, nx=0, nz=0;
      if(!collisionDetect.canWalk((character*)actor, destinyX, 0, destinyZ, 0, 
                                   varHeight, nx, nz, false))
      {
         state = ASTAR_STATE_NOT_FOUND;
         return;
      }

      /* Verify if the destiny is in map and not too far away */
      if( (destinyX < 0) || (destinyZ < 0) || 
          (destinyX >= actualMap->getSizeX()*actualMap->squareSize()) ||
          (destinyZ >= actualMap->getSizeZ()*actualMap->squareSize()) ||
          (dist > 4*((character*)actor)->displacement ) )
      {
         state = ASTAR_STATE_NOT_FOUND;
         return;
      }

      /* Put initial node at search */
      dX = fabs(destinyX - actualX);
      dZ = fabs(destinyZ - actualZ);
      orthogonal = fabs(dX - dZ);
      diagonal = fabs(((dX + dZ) - orthogonal)/2);
      heuristic = diagonal + orthogonal + dX + dZ;
      opened->insert(actualX, actualZ, 0, heuristic, -1, -1);
   }
   else
   {
      /* Note: Verify if isn't running, since sometimes come here
       * when already searching for the same path. */
      if( (state != ASTAR_STATE_RUNNING) && (state != ASTAR_STATE_FOUND) )
      {
         /* Don't search, so don't found =^D */
         state = ASTAR_STATE_NOT_FOUND;
      }
   }
}

/****************************************************************
 *                              clear                           *
 ****************************************************************/
void aStar::clear()
{
   delete(patt);
   patt = new pattAgent(true);
   state = ASTAR_STATE_OTHER;
   walking = false;
}

/****************************************************************
 *                             doCycle                          *
 ****************************************************************/
void aStar::doCycle(bool fightMode)
{
   GLfloat newg = 0;                /* new gone value */
   pointStar* node, *node2, *node3; /* auxiliar nodes */
   GLfloat posX=0, posZ=0;          /* positions */
   int i, j;                        /* counters */
   bool directionChange;            /* direction verification */

   GLfloat varHeight=0, nx=0, nz=0; /* collision returns (ignored) */

   /* Verify if is searching, if not, no need to run */
   if(state != ASTAR_STATE_RUNNING)
   {
      return;
   }

   /* Verify Lists */
   if( (opened == NULL) || (closed == NULL) )
   {
      state = ASTAR_STATE_OTHER;
      return;
   }

   /* Define Pass sizes */
   GLfloat pass = curStepSize*10;
   GLfloat passMid = ((curStepSize*10) * sqrt(2.0));

   /* Set the colision variables */
   collision collisionDetect;
   collisionDetect.defineMap(actualMap, (characterList*)npcs, 
                             (characterList*)pcs);

   for(j = 0; j < SEARCH_INTERVAL; j++)
   {
       /* Verify if the search still exists or if the limit was reached */
      if( (opened->isEmpty()) || 
          ((closed->size() + opened->size()) >= SEARCH_LIMIT) )
      {
         state = ASTAR_STATE_NOT_FOUND;
         return;
      }

      /* Get better node from queue */
      node = opened->findLowest();
      if(!node)
      {
         state = ASTAR_STATE_NOT_FOUND;
         return;
      }

       /* Verify if arrived at destiny */
      if( (node->x >= destinyX - (curStepSize * 10)) &&
          (node->x <= destinyX + (curStepSize * 10)) && 
          (node->z >= destinyZ - (curStepSize * 10)) &&
          (node->z <= destinyZ + (curStepSize * 10)) )
      {
         /* create new path */
         delete(patt);
         patt = new pattAgent(true);
         patt->defineDestiny(node->x, node->z);
         patt->defineStepSize(curStepSize);
         patt->defineOrientation( ((character*)curActor)->scNode->getAngleY() );
         patt->defineSight(curStepSize*10, 360);
         destinyX = node->x;
         destinyZ = node->z;

         /* Make the Founded path */
         while( (node != NULL) )
         {
            patt->addWayPointFirst(node->x, node->z);
            node = closed->find(node->parentX, node->parentZ);
         }
         patt->removeLinearWayPoints();
         patt->setOrigin( ((character*)curActor)->scNode->getPosX(), 
                          ((character*)curActor)->scNode->getPosZ() );
         patt->definePosition( ((character*)curActor)->scNode->getPosX(), 
                               ((character*)curActor)->scNode->getPosZ() );

         /* We're done */
         clearSearch();
         state = ASTAR_STATE_FOUND;
         walking = true;

         return;
      }

      /* Visit all Adjacents Positions */
      for(i=0; i<9; i++)
      {
         /* Get current direction  */
         directionChange = false;
         dX = node->x - node->parentX;
         dZ = node->z - node->parentZ;

         /* Get next potential node  */
         switch(i) 
         {
           case 1:
           {
              posX = node->x;
              posZ = node->z - pass;
              directionChange = !( (dX == 0) && (dZ < 0) ); 
           }
           break;
           case 2:
           {
              posX = node->x + passMid;
              posZ = node->z - passMid;
              directionChange = !( (dX > 0) && (dZ < 0) );
           }
           break;
           case 3:
           {
              posX = node->x + pass;
              posZ = node->z;
              directionChange = !( (dX > 0) && (dZ == 0) );
           }
           break;
           case 4:
           {
              posX = node->x + passMid;
              posZ = node->z + passMid;
              directionChange = !( (dX > 0) && (dZ > 0) );
           }
           break;
           case 5:
           {
              posX = node->x;
              posZ = node->z + pass;
              directionChange = !( (dX == 0) && (dZ > 0) );
           }
           break;
           case 6:
           {
              posX = node->x - passMid;
              posZ = node->z + passMid;
              directionChange = !( (dX <  0) && (dZ > 0) );
           }
           break;
           case 7:
           {
              posX = node->x - pass;
              posZ = node->z;
              directionChange = !( (dX <  0) && (dZ == 0) );
           }
           break;
           case 8:
           {
              posX = node->x - passMid;
              posZ = node->z - passMid;
              directionChange = !( (dX <  0) && (dZ < 0) );
           }
           break;
        }

        /* Only look at valid nodes */
        if(collisionDetect.canWalk((character*)curActor, posX, 0, posZ, 0, 
                                   varHeight, nx, nz, false) )
        {
           /* New Gone is the current gone + distance to this one */
           newg = node->gone + sqrt((posX - node->x) * (posX - node->x) + 
                 (posZ - node->z) * (posZ - node->z));

           /* Apply direction change penalty, if needed  */
           if(directionChange)
           {
              newg += 2;
           }

           /* search it at closed */
           node2 = closed->find(posX, posZ);

           /* search it at opened */
           node3 = opened->find(posX, posZ);

           /* Heuristic calculation with diagonal.  */
           dX = fabs(destinyX - posX);
           dZ = fabs(destinyZ - posZ);
           diagonal = (dX < dZ) ? dX : dZ;
           orthogonal = dX + dZ;
           heuristic = 1.41 * diagonal + (orthogonal - 2*diagonal);

           /* If is in open or closed and n.g <= new g */
           if( ((node2 != NULL) && (node2->gone <= newg)) || 
               ((node3 != NULL) && (node3->gone <= newg)) )
           {
              //skip
           }
           else
           {
              /* Remove it from closed */
              if(node2 != NULL)
              {
                 closed->remove(node2);
              }

              /* If is at open list */
              if(node3)
              {
                 /* Just update it */
                 node3->parentX = node->x;
                 node3->parentZ = node->z;
                 node3->gone = newg;
                 node3->heuristic = heuristic;
              }
              else
              {
                 /* Insert it */
                 opened->insert(posX, posZ, newg, heuristic, node->x, node->z);
              }
           }
        }
      }

      /* Push node onto closed */
      closed->insert(node->x, node->z, node->gone, node->heuristic, 
                     node->parentX, node->parentZ);
      opened->remove(node);
   }

}

/****************************************************************
 *                       setOrientation                         *
 ****************************************************************/
void aStar::setOrientation(GLfloat ori)
{
   patt->defineOrientation(ori);
}

/****************************************************************
 *                       getNewPosition                         *
 ****************************************************************/
bool aStar::getNewPosition(GLfloat& posX, GLfloat& posZ, GLfloat& ori,
                           bool fightMode, bool run)
{
   GLfloat pX=0, pZ=0;

   /* Get the pattern next position */
   if(patt->defineNextPosition(run))
   {
      /* If Position not changed (with the same angle), the move ended! */
      patt->getPosition(pX, pZ);
      if( (pX == posX) && (pZ == posZ) && (ori == patt->orientationValue()) )
      {
         walking = false;
         return(false);
      }

      /* Update Position */
      posX = pX;
      posZ = pZ;
      ori = patt->orientationValue();

      if(fightMode)
      {
         /* Verify if overflow the max normal walk */
         if( (patt->getTotalWalked() > ((character*)curActor)->displacement) &&
             ( !((character*)curActor)->getCanAttack() ) )
         {
            /* Can't do a full move, so stop here */
            ((character*)curActor)->setCanMove(false);
            walking = false;
            return(false);
         }

         /* Verify if overflow the max full walk */
         else if( patt->getTotalWalked() >= 
               2*((character*)curActor)->displacement)
         {
            ((character*)curActor)->setCanAttack(false);
            ((character*)curActor)->setCanMove(false);
            walking = false;
            return(false);
         }

         /* Verify if arrived at destiny */
         else if( (posX == destinyX) && (posZ == destinyZ) )
         {
            /* Update the booleans */
            if(patt->getTotalWalked() > ((character*)curActor)->displacement)
            {
               ((character*)curActor)->setCanAttack(false);
            }
            ((character*)curActor)->setCanMove(false);
            
            /* Stop the motion */
            walking = false;
            return(false);
         }

         /* Still walking */
         walking = true;
         return(true);
      }
      else
      {
         /* Only stop when arrive at destiny */
         walking = (posX != destinyX) || (posZ != destinyZ);
         return(walking);
      }
   }
   walking = false;
   return(false);
}

/****************************************************************
 *                          drawPath                            *
 ****************************************************************/
void aStar::drawPath()
{
   patt->drawWayPointsLinear();
}

/****************************************************************
 *                            getDestiny                        *
 ****************************************************************/
void aStar::getDestiny(GLfloat& destX, GLfloat& destZ)
{
   destX = destinyX;
   destZ = destinyZ;
}

/****************************************************************
 *                             getState                         *
 ****************************************************************/
int aStar::getState()
{
   int st = -1;
   st = state;
   if(state != ASTAR_STATE_RUNNING)
   {
      state = ASTAR_STATE_OTHER;
   }
   return(st);
}


/****************************************************************************/
/****************************************************************************/
/*                            LIST STAR                                     */
/****************************************************************************/
/****************************************************************************/


/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
listStar::listStar()
{
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
listStar::~listStar()
{
   clearList();
}

/****************************************************************
 *                         freeElement                          *
 ****************************************************************/
void listStar::freeElement(dntListElement* obj)
{
   pointStar* pst = (pointStar*) obj;
   delete(pst);
}

/****************************************************************
 *                            insert                            *
 ****************************************************************/
pointStar* listStar::insert(GLfloat x, GLfloat z, GLfloat gone, 
                            GLfloat heuristic, GLfloat parentX, GLfloat parentZ)
{
   pointStar* newWay = find(x,z);
   if(newWay != NULL)
   {
      /* To not insert duplicate points */
      if(gone < newWay->gone)
      {
         newWay->gone = gone;
         newWay->heuristic = heuristic;
         newWay->parentX = parentX;
         newWay->parentZ = parentZ;
      }
      return(newWay);
   }

   /* Not found, create a new */
   newWay = new(pointStar);
   newWay->x = x;
   newWay->z = z;
   newWay->gone = gone;
   newWay->heuristic = heuristic;
   newWay->parentX = parentX;
   newWay->parentZ = parentZ;

   /* Really insert on list */
   ((dntList*)this)->insert(newWay);

   return(newWay);
}

/****************************************************************
 *                           remove                             *
 ****************************************************************/
void listStar::remove(pointStar* node)
{
   if(node)
   {
      ((dntList*)this)->remove(node);
   }
   else
   {
      cerr << "liststar: Trying to remove a NULL node!" << endl;
   }
}

/****************************************************************
 *                            find                              *
 ****************************************************************/
pointStar* listStar::find(GLfloat x, GLfloat z)
{
   int aux;
   pointStar* tmp = (pointStar*)first;
   for(aux = 0; aux < total; aux++)
   {
      if( (tmp->x == x) && (tmp->z == z))
      {
         return(tmp);
      }
      tmp = (pointStar*)tmp->getNext();
   }
   return(NULL);
}

/****************************************************************
 *                            findLowest                        *
 ****************************************************************/
pointStar* listStar::findLowest()
{
   int aux;
   pointStar* tmp = (pointStar*)first;
   pointStar* lowest = (pointStar*)first;

   if(total <= 0)
   {
      return(NULL);
   }
   
   for(aux = 0; aux < total; aux++)
   {
      if( (tmp->gone + tmp->heuristic) <= (lowest->gone+lowest->heuristic) )
      {
         lowest = tmp;
      }
      tmp = (pointStar*)tmp->getNext();
   }
   return(lowest);
}

