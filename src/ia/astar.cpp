#include "astar.h"
#include "../engine/collision.h"
#include <math.h>

#define SEARCH_LIMIT  10000  /**< Max Nodes the aStar will search */
#define SEARCH_INTERVAL 200  /**< Interval of Nodes when aStar will sleep */
#define MIN_CALL        200  /**< Minimun time interval to call search again */


/*! Struct to pass data to the paralel thread */
typedef struct 
{
   aStar* as;
   GLfloat actualX;
   GLfloat actualZ;
   GLfloat x;
   GLfloat z;
   GLfloat stepSize;
   GLfloat orientation;
   GLfloat perX1;
   GLfloat perY1;
   GLfloat perZ1; 
   GLfloat perX2; 
   GLfloat perY2;
   GLfloat perZ2;
}dataThread;

/*************************************************************************
 *                           runParalelSearch                            *
 *************************************************************************/
int runParalelSearch(void* data)
{
   dataThread* dt = (dataThread*)data;
   dt->as->findPathInternal(dt->actualX, dt->actualZ, dt->x, dt->z, 
                            dt->stepSize, dt->orientation, 
                            dt->perX1, dt->perY1, dt->perZ1,
                            dt->perX2, dt->perY2, dt->perZ2);
   /* Delete the dt */
   delete(dt);
   return(1);
}

/*************************************************************************
 *                                 lock                                  *
 *************************************************************************/
void aStar::lock()
{
   if(SDL_mutexP(searchMutex) != 0)
   {
      printf("Error while A* Search Mutex Lock\n");
   }
}

/*************************************************************************
 *                                unLock                                 *
 *************************************************************************/
void aStar::unLock()
{
   if(SDL_mutexV(searchMutex) != 0)
   {
      printf("Error while A* Search Mutex UnLock\n");
   }
}

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
   searchThread = NULL;
   searchMutex = SDL_CreateMutex();
   lastCallTime = SDL_GetTicks();
   abort = false;
}

/****************************************************************
 *                         defineMap                            *
 ****************************************************************/
void aStar::defineMap(Map* acMap)
{
   lock();
      actualMap = acMap;
   unLock();
}

/****************************************************************
 *                         Destructor                           *
 ****************************************************************/
aStar::~aStar()
{
   delete(patt);
   actualMap = NULL;
   if(state == ASTAR_STATE_RUNNING)
   {
      /* Stops the running thread */
      lock();
         abort = true;
      unLock();
   }
   SDL_DestroyMutex(searchMutex);
}

/****************************************************************
 *                            findPath                          *
 ****************************************************************/
void aStar::findPath(GLfloat actualX, GLfloat actualZ, GLfloat x, GLfloat z,
                     GLfloat stepSize, GLfloat orientation,
                     GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                     GLfloat perX2, GLfloat perY2, GLfloat perZ2)
{
   GLuint actualTime = SDL_GetTicks();

   if(actualTime-lastCallTime >= MIN_CALL)
   {
      lastCallTime = actualTime;
      dataThread* dt = new(dataThread);
      dt->as = this;
      dt->actualX = actualX;
      dt->actualZ = actualZ;
      dt->x = x;
      dt->z = z;
      dt->stepSize = stepSize;
      dt->orientation = orientation;
      dt->perX1 = perX1;
      dt->perY1 = perY1;
      dt->perZ1 = perZ1;
      dt->perX2 = perX2;
      dt->perY2 = perY2;
      dt->perZ2 = perZ2;

      if(state == ASTAR_STATE_RUNNING)
      {
         /* End the running thread */
         lock();
            abort = true;
         unLock();
         SDL_Delay(50);
      }
      /* Creates the Search Thread */
      abort = false;
      state = ASTAR_STATE_RUNNING;
      searchThread  = SDL_CreateThread((&runParalelSearch), dt);
   }
}

/****************************************************************
 *                              clear                           *
 ****************************************************************/
void aStar::clear()
{
   lock();
   delete(patt);
   patt = new pattAgent(true);
   patt->defineDestiny(0, 0);
   patt->defineStepSize(0);
   patt->defineOrientation(0);
   patt->defineSight(0, 0);
   unLock();
}

/****************************************************************
 *                        findPathInternal                      *
 ****************************************************************/
bool aStar::findPathInternal(GLfloat actualX, GLfloat actualZ, 
                             GLfloat x, GLfloat z,
                             GLfloat stepSize, GLfloat orientation,
                             GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                             GLfloat perX2, GLfloat perY2, GLfloat perZ2)
{
   /* To avoid previous path, delete the existed one! */
   clear();   
  
 lock();
   int i;
   GLfloat varHeight = 0, nx = 0, nz = 0;
   Square* perQuad = NULL;
   GLfloat posX=0, posZ=0;
   GLfloat newg=0, heuristic, dX, dZ, diagonal, orthogonal;
   pointStar* node, *node2, *node3;
   listStar opened;
   listStar closed;
   destinyX = x;
   destinyZ = z;
   collision collisionDetect;

   /* FIXME use the NPCs pointer, instead of NULL */
   collisionDetect.defineMap(actualMap, NULL);

   GLfloat ssize = actualMap->squareSize();

   if( (destinyX < 0) || (destinyZ < 0) || 
       (destinyX >= actualMap->getSizeX()*ssize) ||
       (destinyZ >= actualMap->getSizeZ()*ssize) )
   {
      state = ASTAR_STATE_NOT_FOUND;
      unLock();
      return(false);
   }
   
   opened.insert(actualX, actualZ, 0, 
                 sqrt((actualX-destinyX)*(actualX-destinyX) + 
                 (actualZ-destinyZ)*(actualZ-destinyZ)), -1, -1);

   while((!opened.isEmpty()) && (closed.size() <= SEARCH_LIMIT))
   {
      /* Verify Abort */
      if(abort)
      {
         return(false);
      }

      if((closed.size() != 0) && (closed.size() % SEARCH_INTERVAL == 0))
      {
         /* Sleep */
         unLock();
         SDL_Delay(50);
         lock();
      }
      
      node = opened.findLowest();

      if(!node)
      {
         state = ASTAR_STATE_NOT_FOUND;
         unLock();
         return(false);
      }
     
       /* Verify if arrived at destiny */
      if( (node->x>=destinyX-(stepSize*10)) &&
          (node->x<=destinyX+(stepSize*10)) && 
          (node->z>=destinyZ-(stepSize*10)) &&
          (node->z<=destinyZ+(stepSize*10)) )
      {
         delete(patt);
         patt = new pattAgent(true);
         patt->defineDestiny(node->x, node->z);
         patt->defineStepSize(stepSize);
         patt->defineOrientation(orientation);
         patt->defineSight(stepSize*10, 360);
         destinyX = node->x;
         destinyZ = node->z;

         /* Make the Founded path */
         while( (node != NULL) )
         {
            patt->addWayPointFirst(node->x, node->z);
            node = closed.find(node->parentX, node->parentZ);
         }
         patt->removeLinearWayPoints();
         patt->definePosition(actualX, actualZ);
         state = ASTAR_STATE_FOUND;
         unLock();
         return(true);
      }

      GLfloat pass = stepSize*10;
      GLfloat passMid = ((stepSize*10) / sqrt(2.0));
      /* Visit all Adjacents Positions */
      for(i=0; i<9; i++)
      {
         
         switch(i) {
           case 1:
              posX = node->x;
              posZ = node->z - pass;
           break;
           case 2:
              posX = node->x + passMid;
              posZ = node->z - passMid;
           break;
           case 3:
              posX = node->x + pass;
              posZ = node->z;
           break;
           case 4:
              posX = node->x + passMid;
              posZ = node->z + passMid;
           break;
           case 5:
              posX = node->x;
              posZ = node->z + pass;
           break;
           case 6:
              posX = node->x - passMid;
              posZ = node->z + passMid;
           break;
           case 7:
              posX = node->x - pass;
              posZ = node->z;
           break;
           case 8:
              posX = node->x - passMid;
              posZ = node->z - passMid;
           break;
        }
       
        newg = node->gone + sqrt((posX - node->x) * (posX - node->x) + 
                                 (posZ - node->z) * (posZ - node->z));
        
        node2 = closed.find(posX, posZ);
        node3 = opened.find(posX, posZ);

        dX = fabs(destinyX - posX);
        dZ = fabs(destinyZ - posZ);
        orthogonal = fabs(dX - dZ);
        diagonal = fabs(((dX + dZ) - orthogonal)/2);
        
        heuristic = diagonal + orthogonal + dX + dZ;

        perQuad = actualMap->relativeSquare((int)floor( posX / (ssize)),
                                            (int)floor( posZ / (ssize)));
        if( (node2 != NULL) || (node3 != NULL) || 
            (perQuad == NULL) ||
            (!collisionDetect.canWalk(posX, 0, posZ, 
                                      perX1, perY1, perZ1, 
                                      perX2, perY2, perZ2, 
                                      orientation, perQuad,
                                      varHeight, nx, nz)) )
        {
           if( (node2 != NULL) && (node2->gone > newg))
           {
              closed.remove(node2);
              opened.insert(posX, posZ, newg, heuristic, node->x, node->z);
           }
           
           if( (node3 != NULL) && (node3->gone > newg))
           {
              opened.remove(node3);
              opened.insert(posX, posZ, newg, heuristic, node->x, node->z);
           }
        }
        else
        {
           opened.insert(posX, posZ, newg, heuristic, node->x, node->z);
        }
      }

      closed.insert(node->x, node->z, node->gone, node->heuristic, 
                    node->parentX, node->parentZ);
      opened.remove(node);
   }
   
   state = ASTAR_STATE_NOT_FOUND;
   unLock();
   return(false);
}

/****************************************************************
 *                       setOrientation                         *
 ****************************************************************/
void aStar::setOrientation(GLfloat ori)
{
   lock();
      patt->defineOrientation(ori);
   unLock();
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
      //To no insert duplicate points
      if(gone < newWay->gone)
      {
         newWay->gone = gone;
         newWay->heuristic = heuristic;
         newWay->parentX = parentX;
         newWay->parentZ = parentZ;
      }
      return(NULL);
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
      first = newWay;
   }
   else
   {
      newWay->previous = first->previous;
      newWay->previous->next = newWay;
      newWay->next = first;
      first->previous = newWay;
   }

   totalNodes++;
   return(newWay);
}

/****************************************************************
 *                           remove                             *
 ****************************************************************/
void listStar::remove(pointStar* node)
{
   node->previous->next = node->next;
   node->next->previous = node->previous;
   if(node == first)
   {
      first = node->next;
   }
   delete(node);
   totalNodes--;
   if(totalNodes == 0)
   {
      first = NULL;
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
   int st = state;
   if(state != ASTAR_STATE_RUNNING)
   {
      state = ASTAR_STATE_OTHER;
   }
   return(st);
}

