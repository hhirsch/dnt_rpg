#include "area.h"

/*********************************************************************
 *                           Constructor                             *
 *********************************************************************/
areaList::areaList()
{
   first = NULL;
   total = 0;
}

/*********************************************************************
 *                           Destructor                              *
 *********************************************************************/
areaList::~areaList()
{
   int aux;
   area* a;
   for(aux = 0 ; aux < total; aux++)
   {
      a = first;
      first = first->next;
      delete(a);
   }
   first = NULL;
   total = 0;
}

/*********************************************************************
 *                              getArea                              *
 *********************************************************************/
area* areaList::getArea(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
   int aux;
   area* a = first;
   for(aux = 0 ; aux < total; aux++)
   {
      if( (a->x1 == x1) && (a->y1 == y1) && (a->x2 == x2) && (a->y2 == y2) )
      {
         return(a);
      }
      a = a->next;
   }
   return(NULL);
}

area* areaList::getArea(GLfloat x, GLfloat y)
{
   int aux;
   area* a = first;
   for(aux = 0 ; aux < total; aux++)
   {
      if( (a->x1 <= x) && (a->y1 <= y) && (a->x2 >= x) && (a->y2 >= y) )
      {
         return(a);
      }
      a = a->next;
   }
   return(NULL);
}


/*********************************************************************
 *                             addArea                               *
 *********************************************************************/
void areaList::addArea(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
                       string whereToGo)
{
   /* Only insert if not inserted already */
   if(getArea(x1, y1, x2, y2) == NULL)
   {
      area* a = new(area);
      a->x1 = x1;
      a->y1 = y1;
      a->x2 = x2;
      a->y2 = y2;
      a->whereToGo = whereToGo;
      a->next = first;
      first = a;
      total++;
   }
   else
   {
      getArea(x1, y1, x2, y2)->whereToGo = whereToGo;
   }
}

/*********************************************************************
 *                             drawAll                               *
 *********************************************************************/
void areaList::drawAll()
{
   int aux;
   area* a = first;
   //Draw
   glDisable(GL_LIGHTING);
   glColor3f(0.5, 0.5, 0.1);
   glBegin(GL_QUADS);
   for(aux = 0 ; aux < total; aux++)
   {
      glVertex3f(a->x1, 0.1, a->y1);
      glVertex3f(a->x1, 0.1, a->y2);
      glVertex3f(a->x2, 0.1, a->y2);
      glVertex3f(a->x2, 0.1, a->y1);
      a = a->next;
   }
   glEnd();
   glEnable(GL_LIGHTING);
   glColor3f(1.0,1.0,1.0);
}

