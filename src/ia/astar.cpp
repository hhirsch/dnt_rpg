#include "astar.h"

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
aStar::aStar(Map* map)
{
   actualMap = map;
   patt = new pattAgent(true);
   patt->defineDestiny(0, 0);
   patt->defineStepSize(0);
   patt->defineOrientation(0);
   patt->defineSight(0, 0);
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
   GLfloat newg;
   pointStar* node, *node2;
   listStar opened;
   listStar closed;
   destinyX = x;
   destinyZ = z;

   opened.insert(actualX, actualZ, 0, 
                 (actualX-destinyX)*(actualX-destinyX) + 
                 (actualZ-destinyZ)*(actualZ-destinyZ),
                 NULL);

   while(!opened.isEmpty())
   {
      node = opened.findLowest();
      if( (node->x >= destinyX-1) && (node->x <= destinyX+1) && 
          (node->z >= destinyZ-1) && (node->z <= destinyZ+1) )
      {
         delete(patt);
         patt = new pattAgent(true);
         patt->defineDestiny(destinyX, destinyZ);
         patt->defineStepSize(stepSize);
         patt->defineOrientation(orientation);
         patt->defineSight(stepSize*2, 360);

         /* Make the Founded path */
         patt->addWayPointFirst(destinyX, destinyZ);
         
         while(node->parent != NULL)
         {
            patt->addWayPointFirst(node->parent->x, node->parent->z);
            node = node->parent;
         }
         return(true);
      }

      //Visit all Adjacents Positions
      for(aux = -1; aux <= 1; aux++)
      {
         for(auz = -1; auz <= 1; auz++)
         {
            if( (aux != 0) || (auz != 0))
            {
               newg = node->gone + (aux*aux) + (auz*auz);
               node2 = closed.find(node->x+aux, node->z+auz);
               
               if( (opened.find(node->x+aux, node->z+auz)) ||
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
                  opened.insert(node->x+aux, node->z+auz, newg, 
                                (node->x+aux-destinyX)*(node->x+aux-destinyX) + 
                                (node->z+auz-destinyZ)*(node->z+auz-destinyZ),
                                node);
               }
                   
            }
         }
      }

      closed.insert(node->x, node->z, node->gone, node->heuristic, node->parent);
      opened.remove(node);
   }
   
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
void listStar::insert(GLfloat x, GLfloat z, GLfloat gone, GLfloat heuristic,
                      pointStar* parent)
{
   if(find(x, z) != NULL)
   {
      //To no insert duplicate points
      return;
   }
   pointStar* newWay = new(pointStar);
   newWay->x = x;
   newWay->z = z;
   newWay->gone = gone;
   newWay->heuristic = heuristic;
   newWay->parent = parent;
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


