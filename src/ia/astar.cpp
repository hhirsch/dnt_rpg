#include "astar.h"
#include <math.h>

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
}

/****************************************************************
 *                         defineMap                            *
 ****************************************************************/
void aStar::defineMap(Map* map)
{
   actualMap = map;
}

/****************************************************************
 *                         Destructor                           *
 ****************************************************************/
aStar::~aStar()
{
   delete(patt);
   actualMap = NULL;
}

/****************************************************************
 *                       defineDestiny                          *
 ****************************************************************/
bool aStar::findPath(GLfloat actualX, GLfloat actualZ, GLfloat x, GLfloat z,
                     GLfloat stepSize, GLfloat orientation)
{
   int aux, auz;
   GLfloat posX, posZ;
   GLfloat newg;
   pointStar* node, *node2;
   listStar opened;
   listStar closed;
   destinyX = x;
   destinyZ = z;

   opened.insert(actualX, actualZ, 0, 
                 sqrt((actualX-destinyX)*(actualX-destinyX) + 
                 (actualZ-destinyZ)*(actualZ-destinyZ)), -1, -1);

   while(!opened.isEmpty())
   {
      node = opened.findLowest();
      
      if( (node->x >= destinyX-stepSize) && (node->x <= destinyX+stepSize) && 
          (node->z >= destinyZ-stepSize) && (node->z <= destinyZ+stepSize) )
      {
         delete(patt);
         patt = new pattAgent(true);
         patt->defineDestiny(destinyX, destinyZ);
         patt->defineStepSize(stepSize);
         patt->defineOrientation(orientation);
         patt->defineSight(stepSize*2, 360);

         /* Make the Founded path */
         printf("Found Path to %.3f %.3f!\n",destinyX, destinyZ);
         patt->addWayPointFirst(destinyX, destinyZ);
         
         while( (node != NULL) )
         {
            patt->addWayPointFirst(node->x, node->z);
            printf("Add: %.3f %.3f\n", node->x, node->z);
            node = closed.find(node->parentX, node->parentZ);
         }
         printf("definedPosition %.3f %.3f\n", actualX, actualZ);
         patt->definePosition(actualX, actualZ);
         return(true);
      }

      //Visit all Adjacents Positions
      for(aux = -1; aux <= 1; aux++)
      {
         for(auz = -1; auz <= 1; auz++)
         {
            if( (aux != 0) || (auz != 0))
            {
               posX = node->x + (aux*stepSize);
               posZ = node->z + (auz*stepSize);
               newg = node->gone + sqrt(((aux*stepSize)*(aux*stepSize)) + 
                                         (auz*stepSize)*(auz*stepSize));
               node2 = closed.find(posX, posZ);
               
               if( (opened.find(posX, posZ)) ||
                   (node2 != NULL) &&
                   (node->gone <= newg))
               {
                  continue;
               }
               else
               {
                  if(node2)
                  {
                     closed.remove(node2);
                  }
                  if(!opened.find(posX, posZ))
                  {
                     opened.insert(posX, posZ, newg, 
                                sqrt( (posX - destinyX) * (posX - destinyX) + 
                                      (posZ - destinyZ) * (posZ - destinyZ)),
                                   node->x, node->z);
                  }
               }
                   
            }
         }
      }

      closed.insert(node->x, node->z, node->gone, node->heuristic, 
                    node->parentX, node->parentZ);
      opened.remove(node);
   }
   
   printf("Not Found!\n");
   return(false);
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
   if(find(x, z) != NULL)
   {
      //To no insert duplicate points
      return(NULL);
   }
   pointStar* newWay = new(pointStar);
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


