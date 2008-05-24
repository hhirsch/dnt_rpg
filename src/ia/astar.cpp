#include "astar.h"
#include "../engine/collision.h"
#include <math.h>

#define SEARCH_LIMIT    200  /**< Max Nodes the aStar will search */
#define SEARCH_INTERVAL  10  /**< Interval of Nodes when aStar will sleep */
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
   curActor = NULL;
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
                     void* NPCs, void* PCs, bool forceCall)
{
   GLuint actualTime = SDL_GetTicks();

   /* Verify time variation */
   if( (actualTime - lastCallTime >= MIN_CALL) || (forceCall))
   {
      lastCallTime = actualTime;

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
      GLfloat actualX = ((character*)actor)->xPosition;
      GLfloat actualZ = ((character*)actor)->zPosition;
      dX = (x - actualX);
      dZ = (z - actualZ);
      GLfloat dist = sqrt( (dX*dX) + (dZ*dZ) );

      if( (destinyX < 0) || (destinyZ < 0) || 
          (destinyX >= actualMap->getSizeX()*actualMap->squareSize()) ||
          (destinyZ >= actualMap->getSizeZ()*actualMap->squareSize()) ||
          (dist < stepSize*10 ) )
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
}

/****************************************************************
 *                             doCycle                          *
 ****************************************************************/
void aStar::doCycle()
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
         patt->defineOrientation( ((character*)curActor)->orientation );
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
         patt->definePosition( ((character*)curActor)->xPosition, 
                               ((character*)curActor)->zPosition );

         clearSearch();
         state = ASTAR_STATE_FOUND;
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
bool aStar::getNewPosition(GLfloat& posX, GLfloat& posZ, GLfloat& ori)
{
   if(patt->defineNextPosition())
   {
      patt->getPosition(posX, posZ);
      ori = patt->orientationValue();
      return((posX != destinyX) || (posZ != destinyZ));
   }
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
   first = NULL;
   totalNodes = 0;
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
listStar::~listStar()
{
   while(totalNodes > 0)
   {
      remove(first);
   }
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
   newWay = new(pointStar);
   newWay->x = x;
   newWay->z = z;
   newWay->gone = gone;
   newWay->heuristic = heuristic;
   newWay->parentX = parentX;
   newWay->parentZ = parentZ;
   if(first == NULL)
   {
      newWay->next = newWay;
      newWay->previous = newWay;
   }
   else
   {
      newWay->previous = first->previous;
      newWay->next = first;
      newWay->previous->next = newWay;
      newWay->next->previous = newWay;
   }
   first = newWay;

   totalNodes++;
   return(newWay);
}

/****************************************************************
 *                           remove                             *
 ****************************************************************/
void listStar::remove(pointStar* node)
{
   if(node)
   {
      node->previous->next = node->next;
      node->next->previous = node->previous;
      if(node == first)
      {
         first = node->next;
      }
      delete(node);
      totalNodes--;
      if(totalNodes <= 0)
      {
         first = NULL;
      }
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
   pointStar* tmp = first;
   for(aux = 0; aux < totalNodes; aux++)
   {
      if( (tmp->x == x) && (tmp->z == z))
      {
         return(tmp);
      }
      tmp = tmp->next;
   }
   return(NULL);
}

/****************************************************************
 *                            findLowest                        *
 ****************************************************************/
pointStar* listStar::findLowest()
{
   int aux;
   pointStar* tmp = first;
   pointStar* lowest = first;

   if(totalNodes <= 0)
   {
      return(NULL);
   }
   
   for(aux = 0; aux < totalNodes; aux++)
   {
      if( (tmp->gone + tmp->heuristic) <= (lowest->gone+lowest->heuristic) )
      {
         lowest = tmp;
      }
      tmp = tmp->next;
   }
   return(lowest);
}

