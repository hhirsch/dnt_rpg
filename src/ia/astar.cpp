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
   int i;
   GLfloat posX=0, posZ=0;
   GLfloat newg=0;
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
      
      if( (node->x>=destinyX-(stepSize*5))&&(node->x<=destinyX+(stepSize*5)) && 
          (node->z>=destinyZ-(stepSize*5))&&(node->z<=destinyZ+(stepSize*5)) )
      {
         delete(patt);
         patt = new pattAgent(true);
         patt->defineDestiny(node->x, node->z);
         patt->defineStepSize(stepSize);
         patt->defineOrientation(orientation);
         patt->defineSight(stepSize*5, 360);
         destinyX = node->x;
         destinyZ = node->z;

         /* Make the Founded path */
         printf("Found Path to %.3f %.3f!\n",node->x, node->z);
         //patt->addWayPointFirst(destinyX, destinyZ);
         
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
      for(i=0; i<9; i++)
      {
         switch(i) {
           case 1:
              posX = node->x;
              posZ = node->z - stepSize*5;
              newg = node->gone + 10;
           break;
           case 2:
              posX = node->x + stepSize*5;
              posZ = node->z - stepSize*5;
              newg = node->gone + 14;
           break;
           case 3:
              posX = node->x + stepSize*5;
              posZ = node->z;
              newg = node->gone + 10;
           break;
           case 4:
              posX = node->x + stepSize*5;
              posZ = node->z + stepSize*5;
              newg = node->gone + 14;
           break;
           case 5:
              posX = node->x;
              posZ = node->z + stepSize*5;
              newg = node->gone + 10; 
           break;
           case 6:
              posX = node->x - stepSize*5;
              posZ = node->z + stepSize*5;
              newg = node->gone + 14;
           break;
           case 7:
              posX = node->x - stepSize*5;
              posZ = node->z;
              newg = node->gone + 10;
           break;
           case 8:
              posX = node->x - stepSize*5;
              posZ = node->z - stepSize*5;
              newg = node->gone + 14;
           break;
        }
       
        //newg = node->gone + 1;
        
        node2 = closed.find(posX, posZ);
             
        /*if( (opened.find(posX, posZ)) ||
            (node2 != NULL) &&
            (node->gone <= newg))*/
        if( (node2 != NULL) || (opened.find(posX, posZ)))
        {
           continue;
        }
        else
        {
           opened.insert(posX, posZ, newg, 
                         sqrt((posX - destinyX) * (posX - destinyX) + 
                         (posZ - destinyZ) * (posZ - destinyZ)),
                         node->x, node->z);
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

void aStar::drawPath()
{
   patt->drawWayPoints();
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


