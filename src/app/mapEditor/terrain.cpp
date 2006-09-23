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
   quadX = (int) (mouseX / SQUARESIZE);
   quadZ = (int) (mouseZ / SQUARESIZE);

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
   Square* saux = actualMap->quadradoRelativo(quadX,quadZ);
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
           saux = actualMap->quadradoRelativo(quadX-1,quadZ);
           if(saux)
           {
              saux->h4 += mod;
           }
           saux = actualMap->quadradoRelativo(quadX,quadZ-1);
           if(saux)
           {
               saux->h2 += mod;
               saux = actualMap->quadradoRelativo(quadX-1,quadZ-1);
               if(saux)
               {
                  saux->h3 += mod;
               }
           }
           else if( (d2<=d1) && (d2<=d3) && (d2<=d4) )
           {
               saux->h2 += mod;
               saux = actualMap->quadradoRelativo(quadX-1,quadZ);
               if(saux)
               {
                  saux->h3 += mod;
               }
               saux = actualMap->quadradoRelativo(quadX,quadZ+1);
               if(saux)
               { 
                  saux->h1 += mod;
                  saux = actualMap->quadradoRelativo(quadX-1,quadZ+1);
                  if(saux)
                  {
                     saux->h4 += mod;
                  } 
               } 
               else if( (d3<=d1) && (d3<=d2) && (d1<=d4) )
               {
                  saux->h3 += mod;
                  saux = actualMap->quadradoRelativo(quadX+1,quadZ);
                  if(saux)
                  {
                     saux->h2 += mod;
                  }
                  saux = actualMap->quadradoRelativo(quadX,quadZ+1);
                  if(saux)
                  {
                     saux->h4 += mod;
                     saux = actualMap->quadradoRelativo(quadX+1,quadZ+1);
                     if(saux)
                     {
                         saux->h1 += mod;
                     }
                  }
               }
               else if( (d4<=d1) && (d4<=d2) && (d4<=d3) )
               {
                  saux->h4 += mod;
                  saux = actualMap->quadradoRelativo(quadX+1,quadZ);
                  if(saux)
                  {
                    saux->h1 += mod;
                  }
                  saux = actualMap->quadradoRelativo(quadX,quadZ-1);
                  if(saux)
                  {
                     saux->h3 += mod;
                     saux = actualMap->quadradoRelativo(quadX+1,quadZ-1);
                     if(saux)
                     {
                         saux->h2 += mod;
                     }
                  }
               }
           }
       }
   }
}

/********************************************************************
 *                            doNivelate                            *
 ********************************************************************/
void terrain::doNivelate(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                         Uint8 mButton)
{
}

/********************************************************************
 *                             doTexture                            *
 ********************************************************************/
void terrain::doTexture(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, GLuint actualTexture )
{
   if(mButton & SDL_BUTTON(1))
   {
      Square* quad = actualMap->quadradoRelativo(quadX, quadZ);
      if(quad)
      {
         quad->textura = actualTexture;
      }
   }
}

/********************************************************************
 *                             drawTemporary                        *
 ********************************************************************/
void terrain::drawTemporary()
{
   Square* quad = actualMap->quadradoRelativo(quadX, quadZ); 
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
   else if((actualTool == TOOL_TERRAIN_UP) || (actualTool == TOOL_TERRAIN_DOWN))
   {
      if(quad)
      {
      }
   }
   else if(actualTool == TOOL_TERRAIN_NIVELATE)
   {
      if(state == STATE_TERRAIN_NIVELATE_STARTED)
      {
      }
      else if(quad)
      {
         glDisable(GL_LIGHTING);
         glColor3f(0.5,0.1,0.6);
         glBegin(GL_POLYGON);
           glVertex3f(quad->x1+HALFSQUARESIZE-2,quad->h1+1,
                      quad->z1+HALFSQUARESIZE-2);
           glVertex3f(quad->x1+HALFSQUARESIZE-2,quad->h2+1,
                      quad->z1+HALFSQUARESIZE+2);
           glVertex3f(quad->x1+HALFSQUARESIZE+2,quad->h3+1,
                      quad->z1+HALFSQUARESIZE+2);
           glVertex3f(quad->x1+HALFSQUARESIZE+2,quad->h4+1,
                      quad->z1+HALFSQUARESIZE-2);
         glEnd();
         glEnable(GL_LIGHTING);
            
      }
   }
}
