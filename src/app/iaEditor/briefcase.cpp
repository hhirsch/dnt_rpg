#include "briefcase.h"
#include "../../engine/util.h"

#include <stdlib.h>


/*********************************************************************
 *                         Constructor                               *
 *********************************************************************/
briefCases::briefCases()
{
   totalCases = 0;
   briefModel = glmReadOBJ("../data/models/objetos/geral/mala.obj","",1);
}

/*********************************************************************
 *                          Destructor                               *
 *********************************************************************/
briefCases::~briefCases()
{
   totalCases = -1;
   glmDelete(briefModel);
}

/*********************************************************************
 *                         insertBriefCase                           *
 *********************************************************************/
void briefCases::insertBriefCase(GLfloat x, GLfloat z, GLfloat angle, int value)
{
   if(totalCases < MAX_BRIEFCASES)
   {
      cases[totalCases].x = x;
      cases[totalCases].z = z;
      cases[totalCases].angle = angle;
      cases[totalCases].value = value;
      cases[totalCases].owned = false;
      cases[totalCases].delivered = false;
      totalCases++;
   }
   else
   {
      printf("Max Number of briefCases overflow!\n");
   }
}

/*********************************************************************
 *                            Draw                                   *
 *********************************************************************/
void briefCases::draw()
{
   int i;
   glEnable(GL_COLOR_MATERIAL);
   
   for(i=0; i<totalCases; i++)
   {
      if(cases[i].delivered)
      {
         //if delivered, put the case random in the map
         srand(SDL_GetTicks());
         cases[i].x = drand48() *  384 + 74;
         cases[i].z = drand48() *  448;
         cases[i].value = (rand() % 100) +1;
         cases[i].owned = false;
         cases[i].delivered = false;
      }
      glPushMatrix();
      glTranslatef(cases[i].x ,0.0, cases[i].z);
      glRotatef(cases[i].angle,0,1,0);
      glmDrawLists(briefModel);
      glPopMatrix();
   }
   
   glDisable(GL_COLOR_MATERIAL);
}

/*********************************************************************
 *                            DrawAt                                 *
 *********************************************************************/
void briefCases::drawAt(GLfloat x, GLfloat z, GLfloat orientation)
{
   glEnable(GL_COLOR_MATERIAL);
   
   glPushMatrix();
      glTranslatef(x ,0.0, z);
      glRotatef(orientation,0,1,0);
      glmDrawLists(briefModel);
   glPopMatrix();
   
   glDisable(GL_COLOR_MATERIAL);
}

/*********************************************************************
 *                         briefCaseInArea                           *
 *********************************************************************/
briefCase* briefCases::briefCaseInArea(GLfloat x1, GLfloat z1,
                                       GLfloat x2, GLfloat z2)
{
   int i;
   GLfloat min1[3];
   GLfloat max1[3];
   GLfloat min2[3];
   GLfloat max2[3];

   GLfloat dx = (briefModel->x1+briefModel->x2) / 2.0;
   GLfloat dz = (briefModel->z1+briefModel->z2) / 2.0;

   min1[0] = x1; 
   min1[1] = 0; 
   min1[2] = z1;
   max1[0] = x2; 
   max1[1] = 0; 
   max1[2] = z2;

   for(i=0; i< totalCases; i++)
   {
      if(!cases[i].owned)
      {
         min2[0] = cases[i].x-dx; 
         min2[1] = 0; 
         min2[2] = cases[i].z-dz;
         max2[0] = cases[i].x+dx; 
         max2[1] = 0; 
         max2[2] = cases[i].z+dz;
         if(estaDentro(min1,max1,min2,max2,1))
         {
            return(&cases[i]);
         }
      }
   }
   return(NULL);
}

