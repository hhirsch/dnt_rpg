#include "terrain.h"

#define STATE_TERRAIN_OTHER            0
#define STATE_TERRAIN_NIVELATE_STARTED 1

/********************************************************************
 *                             Constructor                          *
 ********************************************************************/
terrain::terrain(Map* map)
{
   actualMap = map;
   state = STATE_TERRAIN_OTHER;
}

/********************************************************************
 *                             Destructor                           *
 ********************************************************************/
terrain::~terrain()
{
   actualMap = NULL;
}

/********************************************************************
 *                           verifyAction                           *
 ********************************************************************/
void terrain::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                           Uint8 mButton, int tool, GLuint actualTexture)
{
   quadX = (int) (mouseX / SQUARE_SIZE);
   quadZ = (int) (mouseZ / SQUARE_SIZE);

   mX = mouseX;
   mY = mouseY;
   mZ = mouseZ;

   Square* saux;
   saux = actualMap->relativeSquare(quadX, quadZ);

   if(saux)
   {
      GLfloat dx1 = fabs(mX - saux->x1) / SQUARE_SIZE;
      GLfloat dz1 = fabs(mZ - saux->z1) / SQUARE_SIZE;
      GLfloat dx2 = fabs(mX - saux->x2) / SQUARE_SIZE;
      GLfloat dz2 = fabs(mZ - saux->z2) / SQUARE_SIZE;
      GLfloat ha = (dx2 * saux->h1) + 
                   (dx1 * saux->h4);
      GLfloat hb = (dx2 * saux->h2) + 
                   (dx1 * saux->h3);

      height = (ha * dz2) + (hb * dz1);
   }
   else
   {
      height = 0;
   }

   actualTool = tool;

   if(tool == TOOL_TERRAIN_TEXTURE)
   {
      doTexture(mouseX, mouseY, mouseZ, mButton, actualTexture);
   }
   else if(tool == TOOL_TERRAIN_NIVELATE)
   {
      doNivelate(mouseX, mouseY, mouseZ, mButton);
   }
   else
   {
      doUpDown(mouseX, mouseY, mouseZ, mButton);
   }
}

/********************************************************************
 *                              doUpDown                            *
 ********************************************************************/
void terrain::doUpDown(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                       Uint8 mButton)
{
   Square* saux = actualMap->relativeSquare(quadX,quadZ);
   int mod;

   if(actualTool == TOOL_TERRAIN_UP)
   {
      mod = 1;
   }
   else
   {
      mod = -1;
   }

   if( (saux) && (mButton & SDL_BUTTON(1)))
   {
 
       GLfloat d1,d2,d3,d4; //Distances to Square Vertices

       GLfloat dx1 = mouseX - saux->x1;
       dx1 *= dx1;
       GLfloat dz1 = mouseZ - saux->z1;
       dz1 *= dz1;
       GLfloat dx2 = mouseX - saux->x2;
       dx2 *= dx2;
       GLfloat dz2 = mouseZ - saux->z2;
       dz2 *= dz2;

       d1 = (sqrt( dx1 + dz1 ));
       d2 = (sqrt( dx1 + dz2 ));
       d3 = (sqrt( dx2 + dz2 ));
       d4 = (sqrt( dx2 + dz1 ));

       if( (d1<=d2) && (d1<=d3) && (d1<=d4) )
       {
           saux->h1 += mod;
           saux->setDivisions();
           saux = actualMap->relativeSquare(quadX-1,quadZ);
           if(saux)
           {
              saux->h4 += mod;
              saux->setDivisions();
           }
           saux = actualMap->relativeSquare(quadX,quadZ-1);
           if(saux)
           {
               saux->h2 += mod;
               saux->setDivisions();
               saux = actualMap->relativeSquare(quadX-1,quadZ-1);
               if(saux)
               {
                  saux->h3 += mod;
                  saux->setDivisions();
               }
           }
       }
       else if( (d2<=d1) && (d2<=d3) && (d2<=d4) )
       {
           saux->h2 += mod;
           saux->setDivisions();
           saux = actualMap->relativeSquare(quadX-1,quadZ);
           if(saux)
           {
              saux->h3 += mod;
              saux->setDivisions();
           }
           saux = actualMap->relativeSquare(quadX,quadZ+1);
           if(saux)
           { 
              saux->h1 += mod;
              saux->setDivisions();
              saux = actualMap->relativeSquare(quadX-1,quadZ+1);
              if(saux)
              {
                 saux->h4 += mod;
                 saux->setDivisions();
              } 
           } 
       }
       else if( (d3<=d1) && (d3<=d2) && (d1<=d4) )
       {
          saux->h3 += mod;
          saux->setDivisions();
          saux = actualMap->relativeSquare(quadX+1,quadZ);
          if(saux)
          {
             saux->h2 += mod;
             saux->setDivisions();
          }
          saux = actualMap->relativeSquare(quadX,quadZ+1);
          if(saux)
          {
             saux->h4 += mod;
             saux->setDivisions();
             saux = actualMap->relativeSquare(quadX+1,quadZ+1);
             if(saux)
             {
                saux->h1 += mod;
                saux->setDivisions();
             }
          }
       }
       else if( (d4<=d1) && (d4<=d2) && (d4<=d3) )
       {
          saux->h4 += mod;
          saux->setDivisions();
          saux = actualMap->relativeSquare(quadX+1,quadZ);
          if(saux)
          {
             saux->h1 += mod;
             saux->setDivisions();
          }
          saux = actualMap->relativeSquare(quadX,quadZ-1);
          if(saux)
          {
             saux->h3 += mod;
             saux->setDivisions();
             saux = actualMap->relativeSquare(quadX+1,quadZ-1);
             if(saux)
             {
                saux->h2 += mod;
                saux->setDivisions();
             }
          }
       }
       actualMap->createSplats();
   }
}

/********************************************************************
 *                            pointInSquare                         *
 ********************************************************************/
bool terrain::pointInSquare(GLfloat x, GLfloat y, 
                            GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
   if( (x >= x1) && (x <= x2) )
   {
      if( (y >= y1) && (y <= y2))
      {
         return(true);
      }
   }
   return(false);
}

/********************************************************************
 *                            doNivelate                            *
 ********************************************************************/
void terrain::doNivelate(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                         Uint8 mButton)
{
   if(state == STATE_TERRAIN_OTHER)
   {
      if(mButton & SDL_BUTTON(1))
      {
         quadInitX = quadX;
         quadInitZ = quadZ;
         initmX = mX;
         initmZ = mZ;
         nivelHeight = height;
         state = STATE_TERRAIN_NIVELATE_STARTED;
      }
   }
   else if(state == STATE_TERRAIN_NIVELATE_STARTED)
   {
      if(!(mButton & SDL_BUTTON(1)))
      {
         state = STATE_TERRAIN_OTHER;

         //Do Nivelation
         
         //For all Squares on Selected Area
         int x;
         int z;

         GLfloat dx; GLfloat dz;
         Square* saux = NULL;

         /* Change > <, if needed */
         if(quadInitX > quadX)
         {
            x = quadInitX;
            quadInitX = quadX;
            quadX = x;
         }
         if(quadInitZ > quadZ)
         {
            z = quadInitZ;
            quadInitZ = quadZ;
            quadZ = z;
         }

         /* Change > < if needed */
         if(initmX > mX)
         {
            dx = initmX;
            initmX = mX;
            mX = dx;
         }
         if(initmZ > mZ)
         {
            dz = initmZ;
            initmZ = mZ;
            mZ = dz;
         }

         for(x = quadInitX; x <= quadX; x++)
         {
            for(z = quadInitZ; z <= quadZ; z++)
            {
               saux = actualMap->relativeSquare(x,z);
               if(saux)
               {
                  if(pointInSquare(saux->x1, saux->z1,initmX, initmZ, mX, mZ))
                  {
                      saux->h1 = nivelHeight;
                  }
                  if(pointInSquare(saux->x1, saux->z2,initmX, initmZ, mX, mZ))
                  {
                      saux->h2 = nivelHeight;
                  }
                  if(pointInSquare(saux->x2, saux->z2,initmX, initmZ, mX, mZ))
                  {
                      saux->h3 = nivelHeight;
                  }
                  if(pointInSquare(saux->x2, saux->z1,initmX, initmZ, mX, mZ))
                  {
                      saux->h4 = nivelHeight;
                  }
                  saux->setDivisions();
               }
            }
         }
         actualMap->createSplats();
      }
   }
}

/********************************************************************
 *                             doTexture                            *
 ********************************************************************/
void terrain::doTexture(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, GLuint actualTexture )
{
   if(mButton & SDL_BUTTON(1))
   {
      Square* quad = actualMap->relativeSquare(quadX, quadZ);
      if(quad)
      {
         quad->texture = actualTexture;
         actualMap->createSplats();
      }
   }
}

/********************************************************************
 *                             drawTemporary                        *
 ********************************************************************/
void terrain::drawTemporary()
{
   Square* quad = actualMap->relativeSquare(quadX, quadZ); 

   /************************
    *    Texture Stuff     *
    ************************/
   if( (actualTool == TOOL_TERRAIN_TEXTURE) && (quad))
   {
      glDisable(GL_LIGHTING);
      glColor3f(0.2,0.2,0.9);
      glBegin(GL_POLYGON);
        glVertex3f(quad->x1,quad->h1+1,quad->z1);
        glVertex3f(quad->x1,quad->h2+1,quad->z2);
        glVertex3f(quad->x2,quad->h3+1,quad->z2);
        glVertex3f(quad->x2,quad->h4+1,quad->z1);
      glEnd();
      glEnable(GL_LIGHTING);
   }
   /***************************
    * Terrain Up & Down Stuff *
    ***************************/
   else if((quad) && 
           (((actualTool == TOOL_TERRAIN_UP) || 
             (actualTool == TOOL_TERRAIN_DOWN))))
   {
      if(actualTool == TOOL_TERRAIN_UP)
      {
         glColor3f(0.2,0.8,0.6);
      }
      else
      {
         glColor3f(0.8,0.2,0.1);
      }
      glDisable(GL_LIGHTING);
      glBegin(GL_POLYGON);
        glVertex3f(mX-2,actualMap->getHeight(mX-2,mZ-2)+1,mZ-2);
        glVertex3f(mX-2,actualMap->getHeight(mX-2,mZ+2)+1,mZ+2);
        glVertex3f(mX+2,actualMap->getHeight(mX+2,mZ+2)+1,mZ+2);
        glVertex3f(mX+2,actualMap->getHeight(mX+2,mZ-2)+1,mZ-2);
      glEnd();
      glEnable(GL_LIGHTING);
   }
   /************************
    *   Nivelate Stuff     *
    ************************/
   else if(actualTool == TOOL_TERRAIN_NIVELATE)
   {
      if(state == STATE_TERRAIN_NIVELATE_STARTED)
      {
         Square* oth = actualMap->relativeSquare(quadInitX, quadInitZ);
         if( (oth) && (quad))
         {
            glDisable(GL_LIGHTING);
            glColor3f(0.5,0.1,0.6);
            glBegin(GL_POLYGON);
              glVertex3f(initmX,nivelHeight+0.1,initmZ);
              glVertex3f(initmX,nivelHeight+0.1,mZ);
              glVertex3f(mX,nivelHeight+0.1,mZ);
              glVertex3f(mX,nivelHeight+0.1,initmZ);
            glEnd();
            glEnable(GL_LIGHTING);
         }
      }
      else if(quad)
      {
         glDisable(GL_LIGHTING);
         glColor3f(0.5,0.1,0.6);
         glBegin(GL_POLYGON);
           glVertex3f(mX-2,height+0.1,mZ-2);
           glVertex3f(mX-2,height+0.1,mZ+2);
           glVertex3f(mX+2,height+0.1,mZ+2);
           glVertex3f(mX+2,height+0.1,mZ-2);
         glEnd();
         glEnable(GL_LIGHTING);
            
      }
   }
   glColor3f(1.0,1.0,1.0);
}

