#include "message3d.h"
#include "../gui/dntfont.h"
#include "../gui/draw.h"


/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3d::message3d(GLfloat x, GLfloat y, GLfloat z, string msg)
{
   init(x, y, z, msg, 1.0,0.5,0.5);
}

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3d::message3d(GLfloat x, GLfloat y, GLfloat z, string msg,
                     GLfloat R, GLfloat G, GLfloat B)
{
   init(x,y,z,msg, R,G,B);
}

/***********************************************************
 *                           init                          *
 ***********************************************************/
void message3d::init(GLfloat x, GLfloat y, GLfloat z, string msg,
                     GLfloat R, GLfloat G, GLfloat B)
{
   dntFont fnt;

   /* Define Machine Bit Order */
   Uint32 rmask, gmask, bmask, amask;
   #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      rmask = 0xff000000;
      gmask = 0x00ff0000;
      bmask = 0x0000ff00;
      amask = 0x000000ff;
   #else
      rmask = 0x000000ff;
      gmask = 0x0000ff00;
      bmask = 0x00ff0000;
      amask = 0xff000000;
   #endif

   previous = NULL;
   next = NULL;
   posX = x;
   posY = y;
   posZ = z;
   live = 0;
   message = msg;

   /* Define the font and sizes */
   fnt.defineFont(DNT_FONT_ARIAL, 20);
   size = fnt.getStringWidth(message) + 8;
   halfSize = (size / 2.0);
   color_Set(0,0,0);
   SDL_Surface* s = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                         smallestPowerOfTwo(size),
                                         32,
                                         32, rmask, gmask, bmask, amask);
   color_Set((int)floor(R*255),(int)floor(G*255),(int)floor(B*255));
   fnt.write(s, 0, 0, message.c_str(), true);

   setTextureRGBA(s, &messageTexture);

   SDL_FreeSurface(s);
}

/***********************************************************
 *                       Destructor                        *
 ***********************************************************/
message3d::~message3d()
{
   glDeleteTextures(1,&messageTexture);
}

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
messageController::messageController()
{
   first = NULL;
   total = 0;
}

/***********************************************************
 *                        Destructor                       *
 ***********************************************************/
messageController::~messageController()
{
   while(total > 0)
   {
      removeMessage(first);
   }
   total = 0;
}

/***********************************************************
 *                           add                           *
 ***********************************************************/
void messageController::addMessage(GLfloat x, GLfloat y, GLfloat z, string msg)
{
   if(!msg.empty())
   {
      message3d* m = new message3d(x,y,z,msg);
      addMessage(m);
   }
}

/***********************************************************
 *                           add                           *
 ***********************************************************/
void messageController::addMessage(GLfloat x, GLfloat y, GLfloat z, string msg,
                                   GLfloat R, GLfloat G, GLfloat B)
{
   if(!msg.empty())
   {
      message3d* m = new message3d(x,y,z,msg, R, G, B);
      addMessage(m);
   }
}

/***********************************************************
 *                           add                           *
 ***********************************************************/
void messageController::addMessage(message3d* m)
{
   if(m)
   {
      if(first)
      {
         m->next = first;
         m->previous = first->previous;
         m->next->previous = m;
         m->previous->next = m;
      }
      else
      {
         m->next = m;
         m->previous = m;
      }
      first = m;
      total++;
   }
}

/***********************************************************
 *                          remove                         *
 ***********************************************************/
void messageController::removeMessage(message3d* msg)
{
   if(msg)
   {
      if(msg == first)
      {
         first = first->next;
      }
      msg->next->previous = msg->previous;
      msg->previous->next = msg->next;
      delete(msg);
      total--;
      if(total <= 0)
      {
         first = NULL;
      }
   }
}

/***********************************************************
 *                           draw                          *
 ***********************************************************/
void messageController::draw(GLdouble modelView[16], 
                             GLfloat camX, GLfloat camY, GLfloat camZ)
{
   int i;
   int tot = total;
   message3d* msg = first;
   GLfloat scale = 1.0;
   GLfloat dist = 0;
   GLfloat factor = 1.0;

   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   for(i = 0; i < tot; i++)
   {
      /* Calculate Texture factor */
      factor = (float)(msg->size) / (float)(smallestPowerOfTwo(msg->size));

      /* Calculate scale factor */
      dist = sqrt( (camX-msg->posX)*(camX-msg->posX) +
                   (camY-msg->posY)*(camY-msg->posY) +
                   (camZ-msg->posZ)*(camZ-msg->posZ) );
      scale = dist / 500.0;

      /* Draw */
      glColor3f(1.0, 1.0, 1.0);
      glBindTexture(GL_TEXTURE_2D, msg->messageTexture);
      glPushMatrix();
      glTranslatef(msg->posX, msg->posY, msg->posZ);
      glBegin(GL_QUADS);
         glTexCoord2f(0,1);
         glVertex3f(-msg->halfSize*modelView[0]*scale,0.0f,
                    -msg->halfSize*modelView[8]*scale);
         glTexCoord2f(factor,1);
         glVertex3f(msg->halfSize*modelView[0]*scale,0.0f,
                    msg->halfSize*modelView[8]*scale);
         glTexCoord2f(factor,0);
         glVertex3f(msg->halfSize*modelView[0]*scale + scale*16*modelView[1], 
                    scale*16*modelView[5] + scale*msg->halfSize*modelView[4], 
                    scale*msg->halfSize*modelView[8] + scale*16*modelView[9]);
         glTexCoord2f(0,0);
         glVertex3f(-msg->halfSize*modelView[0]*scale + scale*16*modelView[1], 
                    scale*16*modelView[5] - msg->halfSize*modelView[4]*scale,
                    -msg->halfSize*modelView[8]*scale + 16*modelView[9]*scale);
      glEnd();
      glPopMatrix();
      msg->live++;
      msg->posY += 0.5;
      msg = msg->next;

      /* Verify if the message elapsed */
      if(msg->previous->live > MESSAGE3D_MAX_TIME)
      {
         removeMessage(msg->previous);
      }
   }
   glDisable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glEnable(GL_FOG);
}



