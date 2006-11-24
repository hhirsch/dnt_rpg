#include "objects.h"

#define OBJECTS_STATE_NONE    0
#define OBJECTS_STATE_ADD     1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
objects::objects(Map* map)
{
   actualMap = map;
   state = OBJECTS_STATE_NONE;
   actualObstacle = (mapObjeto*)actualMap->Objetos->primeiro->proximo;
}

/******************************************************
 *                       Destructor                   *
 ******************************************************/
objects::~objects()
{
   actualMap = NULL;
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void objects::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                           Uint8 mButton, int mouseXw, int mouseYw,
                           int tool, GLdouble proj[16], 
                           GLdouble modl[16], GLint viewPort[4])
{
   if(tool == TOOL_OBSTACLE_ADD)
   {
      state = OBJECTS_STATE_ADD;

      if( (mX != mouseXw) || (mY != mouseYw))
      {
         mX = mouseXw;
         mY = mouseYw;
         obstacleX = mouseX;
         obstacleZ = mouseZ;
      }
      
      if( (mButton & SDL_BUTTON(1)) && (actualObstacle != NULL) )
      {
         insertObject(mouseX, mouseZ, obstacleOrientation, actualMap, 
                      actualObstacle, 
                      (int)(mouseX / SQUARESIZE), (int)(mouseZ / SQUARESIZE));
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
      else if((mButton & SDL_BUTTON(2)) && (actualObstacle != NULL))
      {
         obstacleOrientation += 1;
      }
      else if((mButton & SDL_BUTTON(3)) && (actualObstacle != NULL))
      {
         actualObstacle = (mapObjeto*)actualObstacle->proximo;
         /* Verify Head Node */
         if(actualObstacle == actualMap->Objetos->primeiro)
         {
            actualObstacle = (mapObjeto*)actualObstacle->proximo;
         }

         while(mButton & SDL_BUTTON(3))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
   }
   else
   {
      state = OBJECTS_STATE_NONE;
      actualObstacle = NULL;
   }
}

/******************************************************
 *                       drawTemporary                *
 ******************************************************/
void objects::drawTemporary()
{
   if( (state == OBJECTS_STATE_ADD) && (actualObstacle != NULL))
   {
      actualObstacle->Desenhar(obstacleX, obstacleZ, 0, obstacleOrientation);
   }
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void objects::insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                          Map* map, mapObjeto* obj, int qx, int qz)
{
   Square* saux = map->quadradoRelativo(qx,qz);
   int ob=0;
   if(saux)
   {
     while( (ob < MAXOBJETOS ) && (saux->objects[ob] != NULL))
     {
        ob++;
     }
     if(ob<MAXOBJETOS)
     {
        saux->objects[ob] = obj;
        saux->Xobjects[ob] = xReal;
        saux->Zobjects[ob] = zReal;
        saux->objectsOrientation[ob] = orObj;
        saux->objectsDesenha[ob] = 1;
        //printf("%d° Object Inserted on %d %d\n",ob,qx+1,qz+1);
                  
        GLMmodel* modelo = (GLMmodel*)obj->modelo3d; 

        float X[2], Z[2];
        X[0] = modelo->x1;
        X[1] = modelo->x2;
        Z[0] = modelo->z1;
        Z[1] = modelo->z2;
        if(orObj!=0)
        {
           GLfloat oldX, oldZ;
           GLfloat cosseno = cos(deg2Rad(orObj));
           GLfloat seno = sin(deg2Rad(orObj));
           int aux;
           for(aux = 0;aux<=1;aux++)
           {
              oldX = X[aux];
              oldZ = Z[aux];
              X[aux] = (oldZ*seno) + (oldX*cosseno);
              Z[aux] = (oldZ*cosseno) - (oldX*seno);
           }
           if(X[0]>X[1])
           {
              oldX = X[0];
              X[0] = X[1];
              X[1] = oldX;
           }
           if(Z[0]>Z[1])
           {
              oldZ = Z[0];
              Z[0] = Z[1];
              Z[1] = oldZ;
           }
       }

       int minqx, minqz, maxqx, maxqz;
       minqx = (int)(X[0] + xReal) / SQUARESIZE;
       minqz = (int)(Z[0] + zReal) / SQUARESIZE;
       maxqx = (int)(X[1] + xReal) / SQUARESIZE;
       maxqz = (int)(Z[1] + zReal) / SQUARESIZE; 
       int X1, Z1;
       Square* qaux;
       for(X1 = minqx; X1<=maxqx; X1++)
       {
          for(Z1 = minqz; Z1 <=maxqz; Z1++) 
          {
             qaux = map->quadradoRelativo(X1,Z1);
             if((qaux) && (qaux != saux))
             {
                ob =0;
                while( (ob < MAXOBJETOS ) && 
                       (qaux->objects[ob] != NULL))
                {
                   ob++;
                }
                if(ob < MAXOBJETOS)
                {
                   qaux->objects[ob] = obj;
                   qaux->Xobjects[ob] = xReal;
                   qaux->Zobjects[ob] = zReal;
                   qaux->objectsDesenha[ob] = 0;
                   //printf("%d° Object Inserted on %d %d\n",ob,X1+1,Z1+1);
                }
             }
          }
       }
     }
     else
       printf("Objects Overflow on Square %d %d\n",qx+1,qz+1);
   }
   else
     printf("Out of Map's Limits!\n");
}

