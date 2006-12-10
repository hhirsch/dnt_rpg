#include "exclamation.h"

#define MIN_HEIGHT 20
#define MAX_HEIGHT 80

/***************************************************************
 *                          Constructor                        *
 ***************************************************************/
exclamation::exclamation()
{
   totalExclamations = 0;
   list.next = &list;
   list.previous = &list;
   exclamationModel = glmReadOBJ("../data/models/objetos/geral/exclamacao.obj",
                                 "",1);
}

/***************************************************************
 *                           Destructor                        *
 ***************************************************************/
exclamation::~exclamation()
{
   glmDelete(exclamationModel);
   while(totalExclamations > 0)
   {
      removeExclamation(list.next);
   }
}

/***************************************************************
 *                         addExclamation                      *
 ***************************************************************/
void exclamation::addExclamation(GLfloat x, GLfloat z)
{
   if(!find(x,z))
   {
      excObject* exc = new(excObject);
      exc->posX = x;
      exc->posY = MIN_HEIGHT;
      exc->posZ = z;
      exc->next = list.next;
      exc->next->previous = exc;
      exc->previous = &list;
      exc->previous->next = exc;
      totalExclamations++;
   }
}

/***************************************************************
 *                             doStep                          *
 ***************************************************************/
void exclamation::doStep()
{
   glDisable(GL_LIGHTING);
   excObject* exc = list.next;
   while(exc != &list)
   {
      if(exc->posY < MAX_HEIGHT)
      {
         glPushMatrix();
            glTranslatef(exc->posX, exc->posY, exc->posZ);
            glmDrawLists(exclamationModel);
         glPopMatrix();
         exc->posY += 0.5;
      }
      else
      {
         exc = exc->previous;
         removeExclamation(exc->next);
      }
      exc = exc->next;
   }
   glEnable(GL_LIGHTING);
}

/***************************************************************
 *                        removeExclamation                    *
 ***************************************************************/
void exclamation::removeExclamation(excObject* exc)
{
   if( (exc) && (exc != &list))
   {
      exc->next->previous = exc->previous;
      exc->previous->next = exc->next;
      delete(exc);
      totalExclamations--;
   }
}

/***************************************************************
 *                              find                           *
 ***************************************************************/
bool exclamation::find(GLfloat x, GLfloat z)
{
   excObject* exc = list.next; 
   int i;
   for(i=0; i< totalExclamations; i++)
   {
      if( (exc->posX == x) && (exc->posZ == z))
      {
         return(true);
      }
      exc = exc->next;
   }
   return(false);
}


