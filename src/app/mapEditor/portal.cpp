#include "portal.h"
#include "../../gui/farso.h"


#define PORTAL_STATE_OTHER     0
#define PORTAL_STATE_ADD_INIT  1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
portal::portal(Map* map)
{
   actualMap = map;
   state = PORTAL_STATE_OTHER;
   portalList = new(areaList);
   /* Start the portal List */
   Square* s;
   int x, z;
   for(x=0; x<actualMap->x; x++)
   {
      for(z=0; z<actualMap->z;z++)
      {
         s = actualMap->quadradoRelativo(x,z);
         if(s->mapConection.active)
         {
            portalList->addArea(s->mapConection.x1, s->mapConection.z1,
                                s->mapConection.x2, s->mapConection.z2,
                                s->mapConection.mapName);
         }
      }
   }
}

/******************************************************
 *                       Destructor                   *
 ******************************************************/
portal::~portal()
{
   actualMap = NULL;
   delete(portalList);
}

/******************************************************
 *                       VerifyAction                 *
 ******************************************************/
void portal::verifyAction(GLfloat mouseX, GLfloat mouseY, 
                          GLfloat mouseZ, Uint8 mButton, int tool,
                          GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   actualTool = tool;
   mX = mouseX;
   mY = mouseY;
   mZ = mouseZ;
   mB = mButton;

   if(tool == TOOL_PORTAL_ADD)
   {
      doAddPortal();
   }
   else if (tool == TOOL_PORTAL_TAG)
   {
      doTagPortal(proj,modl,viewPort);
   }
      
}

/******************************************************
 *                       drawTemporary                *
 ******************************************************/
void portal::drawTemporary()
{
   glDisable(GL_LIGHTING);
   /* Draw In course Map Portal Add */
   if(state == PORTAL_STATE_ADD_INIT)
   {
      glColor3f(0.8,0.1,0.8);
      glBegin(GL_QUADS);
        glVertex3f(initmX, 0.5, initmZ);
        glVertex3f(initmX, 0.5, mZ);
        glVertex3f(mX, 0.5, mZ);
        glVertex3f(mX, 0.5, initmZ);
      glEnd();
   }

   /* Draw All Map Portals Destinations */
   portalList->drawAll();
   
   glColor3f(1.0,1.0,1.0);
   glEnable(GL_LIGHTING);
}

/******************************************************
 *                       addPortal                    *
 ******************************************************/
void portal::addPortal(int qx, int qz, string where)
{
   Square* s = actualMap->quadradoRelativo(qx,qz);
   if(s)
   {
       if(initmX > mX)
       {
           GLfloat tmp = initmX;
           initmX = mX;
           mX = tmp;
       }    
       if(initmZ > mZ)
       {
           GLfloat tmp = initmZ;
           initmZ = mZ;
           mZ = tmp;
       } 
       s->mapConection.x1 = initmX;
       s->mapConection.x2 = mX;
       s->mapConection.z1 = initmZ;
       s->mapConection.z2 = mZ;
       s->mapConection.mapName = where; 
       s->mapConection.active = true;
       portalList->addArea(initmX, initmZ, mX, mZ, where);
      
       int minqx, minqz, maxqx, maxqz;
       minqx = (int)(initmX) / SQUARESIZE;
       minqz = (int)(initmZ) / SQUARESIZE;
       maxqx = (int)(mX) / SQUARESIZE;
       maxqz = (int)(mZ) / SQUARESIZE; 
       int X1, Z1;
       Square* q;
       for(X1 = minqx; X1<=maxqx; X1++)
       {
          for(Z1 = minqz; Z1 <=maxqz; Z1++) 
          {
             q = actualMap->quadradoRelativo(X1,Z1);
             if((q) && (q != s))
             {
                q->mapConection.x1 = initmX;
                q->mapConection.x2 = mX;
                q->mapConection.z1 = initmZ;
                q->mapConection.z2 = mZ;
                q->mapConection.mapName = where; 
                q->mapConection.active = true;
             }
          }
      }
   }
}

/******************************************************
 *                      doAddPortal                   *
 ******************************************************/
void portal::doAddPortal()
{
   int qx = (int) (mX / SQUARESIZE);
   int qz = (int) (mZ / SQUARESIZE);

   if( (state == PORTAL_STATE_OTHER) && (mB & SDL_BUTTON(1)) )
   {
      //Init portal Select Area
      state = PORTAL_STATE_ADD_INIT;
      initmX = mX;
      initmZ = mZ;
   }
   else if( (state == PORTAL_STATE_ADD_INIT) && !(mB & SDL_BUTTON(1)) )
   {
      //Add Portal
      state = PORTAL_STATE_OTHER;
      addPortal(qx,qz, "TODO");
   }
}

/******************************************************
 *                       doTagPortal                  *
 ******************************************************/
void portal::doTagPortal(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   area* ar = portalList->getArea(mX, mZ);

   if( (ar != NULL) && (mB & SDL_BUTTON(1)) )
   {
      initmX = ar->x1;
      mX = ar->x2;
      initmZ = ar->y1;
      mZ = ar->y2;
      
      int qx = (int) (mX / SQUARESIZE);
      int qz = (int) (mZ / SQUARESIZE);
      
      addPortal(qx, qz, getStringFromUser("Input Destiny", ar->whereToGo,
                proj, modl, viewPort));
   }
}


