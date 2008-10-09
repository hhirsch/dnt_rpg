#include "portal.h"
#include "../../gui/farso.h"
#include "../../gui/messages.h"

#define PORTAL_STATE_OTHER     0
#define PORTAL_STATE_ADD_INIT  1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
portal::portal(Map* acMap)
{
   actualMap = acMap;
   state = PORTAL_STATE_OTHER;
   fileDoor = "";
   portalList = new(areaList);
   /* Start the portal List */
   Square* s;
   int x, z;
   for(x=0; x<actualMap->getSizeX(); x++)
   {
      for(z=0; z<actualMap->getSizeZ();z++)
      {
         s = actualMap->relativeSquare(x,z);
         if(s->mapConection.active)
         {
            portalList->addArea(s->mapConection.x1, s->mapConection.z1,
                                s->mapConection.x2, s->mapConection.z2,
                                s->mapConection.mapName);
         }
      }
   }
   actualDoor = NULL;
   doorWall = NULL;
   doorMode = 0;
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
 *                        inner                       *
 ******************************************************/
bool portal::inner(GLfloat ax, GLfloat az, GLfloat bx1, GLfloat bz1, 
                                          GLfloat bx2, GLfloat bz2)
{
   return( (ax >= bx1 ) && (ax <= bx2) && (az >= bz1) && (az <=bz2) );
}

/******************************************************
 *                     defineDoor                     *
 ******************************************************/
void portal::defineDoor(mapObject* newDoor, string fileName)
{
   if(newDoor != NULL)
   {
      actualDoor = newDoor;
      fileDoor = fileName;
   }
}

/******************************************************
 *                       getDoor                      *
 ******************************************************/
mapObject* portal::getDoor()
{
   return(actualDoor);
}

/******************************************************
 *                       getDoor                      *
 ******************************************************/
string portal::getDoorFileName()
{
   return(fileDoor);
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
   else if( (tool == TOOL_PORTAL_DOOR) && (actualDoor))
   {
      /* Take nearest wall */
      wall* m = actualMap->getFirstWall();
      int wNum,  wTotal;
      wTotal = actualMap->getTotalWalls();
      doorWall = m;

      if(!doorWall)
      {
         return;
      }
      
      for( wNum = 0; wNum < wTotal; wNum++ )
      {
           if( inner(mX,mZ,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX-2,mZ,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX+2,mZ,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX,mZ-2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX-2,mZ-2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX+2,mZ-2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX,mZ+2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX-2,mZ+2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX+2,mZ+2,m->x1,m->z1,m->x2,m->z2))
           {
              doorWall = m;
           }

           m = m->next;
      }

      //Coloca X ou Z da porta fixo nele
      if( (doorWall->x2 - doorWall->x1) == 10)
      {
          if(doorMode)
          {
              doorX = doorWall->x2;
          }
          else
          {
              doorX = doorWall->x1;
          }
          doorOrientation = 90;  
          if(mZ > doorWall->z2)
          {
              doorZ = doorWall->z2; 
          }
          else if(mZ < doorWall->z1)
          {
              doorZ = doorWall->z1;
          } 
          else
          {
              doorZ = mZ; 
          }
      }
      else
      {
          if(doorMode)
          {
              doorZ = doorWall->z2;
          }
          else
          {
              doorZ = doorWall->z1;
          }
          doorOrientation = 0;
          if(mX > doorWall->x2)
          {
              doorX = doorWall->x2; 
          }
          else if(mX < doorWall->x1)
          {
              doorX = doorWall->x1;
          } 
          else
          {
              doorX = mX; 
          }
      }
   
      if(mButton & SDL_BUTTON(1))
      {
         GLfloat mx1,mx2,mz1,mz2;
         mx1 = doorWall->x1;
         mx2 = doorWall->x2;
         mz1 = doorWall->z1;
         mz2 = doorWall->z2;
         wall* novoMuro;
         novoMuro = actualMap->addWall(0,0,0,0);
         novoMuro->dX = doorWall->dX;
         novoMuro->dY = doorWall->dY;
         novoMuro->dZ = doorWall->dZ;
         boundingBox bounds = actualDoor->getBoundingBox();
         if( doorOrientation == 0 )
         {
            doorWall->x2 = doorX;
            novoMuro->x1 = doorX + (bounds.x2 - bounds.x1);
            novoMuro->x2 = mx2;
            novoMuro->z1 = mz1;
            novoMuro->z2 = mz2;
         }
         else
         {
            doorWall->z2 = doorZ - (bounds.x2 - bounds.x1);
            novoMuro->z1 = doorZ;
            novoMuro->x2 = mx2;
            novoMuro->x1 = mx1;
            novoMuro->z2 = mz2;
         }
         novoMuro->frontTexture = doorWall->frontTexture;
         novoMuro->backTexture = doorWall->backTexture;
         //Coloca a Porta no Mapa
         /*inserirObjetoMapa(doorX, doorZ, doorOrientation, porta, 
                            (int)(doorX / actualMap->squareSize()), 
                            (int)(doorZ / actualMap->squareSize()) );*/
         door* novaPorta = new(door);
         novaPorta->x = doorX;
         novaPorta->z = doorZ;
         novaPorta->orientation = doorOrientation;
         novaPorta->obj = actualDoor;
         door* paux = actualMap->getFirstDoor();
         actualMap->doors = novaPorta;
         novaPorta->next = paux;
         printf("Added Door: %.3f %.3f\n",doorX,doorZ);
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
      
      if(mButton & SDL_BUTTON(3))
      {
         doorMode = !doorMode;
         while(mButton & SDL_BUTTON(3))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
   }
}

/******************************************************
 *                       drawTemporary                *
 ******************************************************/
void portal::drawTemporary()
{
   glPushMatrix();
   int delta = -2;
   if( (actualTool == TOOL_PORTAL_DOOR) && (actualDoor))
   {
      if(doorMode) delta = 2;
      if(doorOrientation)
      {
        actualDoor->draw(doorX+delta, doorZ, 0, doorOrientation, false);
      }
      else
      {
        actualDoor->draw(doorX, doorZ+delta, 0, doorOrientation, false);
      }
      glBegin(GL_QUADS);
         glVertex3f(mX-2,1,mZ-2);
         glVertex3f(mX-2,1,mZ+2);
         glVertex3f(mX+2,1,mZ+2);
         glVertex3f(mX+2,1,mZ-2);
      glEnd();
   }
   glPopMatrix();

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
   Square* s = actualMap->relativeSquare(qx,qz);
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
       minqx = (int)(initmX) / actualMap->squareSize();
       minqz = (int)(initmZ) / actualMap->squareSize();
       maxqx = (int)(mX) / actualMap->squareSize();
       maxqz = (int)(mZ) / actualMap->squareSize(); 
       int X1, Z1;
       Square* q;
       for(X1 = minqx; X1<=maxqx; X1++)
       {
          for(Z1 = minqz; Z1 <=maxqz; Z1++) 
          {
             q = actualMap->relativeSquare(X1,Z1);
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
   int qx = (int) (mX / actualMap->squareSize());
   int qz = (int) (mZ / actualMap->squareSize());

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
      
      int qx = (int) (mX / actualMap->squareSize());
      int qz = (int) (mZ / actualMap->squareSize());
      
      addPortal(qx, qz, getStringFromUser("Input Destiny", ar->whereToGo,
                proj, modl, viewPort));
   }
}


