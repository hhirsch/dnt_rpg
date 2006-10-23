#include "briefcase.h"


briefCases::briefCases()
{
   totalCases = 0;
   briefModel = glmReadOBJ("../data/models/objetos/geral/mala.obj","",1);
}

briefCases::~briefCases()
{
   totalCases = -1;
   glmDelete(briefModel);
}

void briefCases::insertBriefCase(GLfloat x, GLfloat z, GLfloat angle, int value)
{
   if(totalCases < MAX_BRIEFCASES)
   {
      cases[totalCases].x = x;
      cases[totalCases].z = z;
      cases[totalCases].angle = angle;
      cases[totalCases].value = value;
      totalCases++;
   }
   else
   {
      printf("Max Number of briefCases overflow!\n");
   }
}

void briefCases::draw()
{
   int i;
   glEnable(GL_COLOR_MATERIAL);
   
   for(i=0; i<totalCases; i++)
   {
      glPushMatrix();
      glTranslatef(cases[i].x ,0.0, cases[i].z);
      glRotatef(cases[i].angle,0,1,0);
      glmDrawLists(briefModel);
      glPopMatrix();
   }
   
   glDisable(GL_COLOR_MATERIAL);
}

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

