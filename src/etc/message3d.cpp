#include "message3d.h"
#include "../gui/fonte.h"
#include "../gui/draw.h"


/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3d::message3d(GLfloat x, GLfloat y, GLfloat z, string msg)
{
   previous = NULL;
   next = NULL;
   posX = x;
   posY = y;
   posZ = z;
   live = 0;
   message = msg;

   int size = message.length()*font_incCP();
   halfSize = (size / 5.0);

   /* Define the font and sizes */
   defineFont(FHELVETICA, ALIGN_LEFT, 1);
   color_Set(155,0,0);
   SDL_Surface* s = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       smallestPowerOfTwo(size),
                       32,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   write(s, 0, 0, message.c_str());

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

   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   for(i = 0; i < tot; i++)
   {
      /* Calculate scale factor */
      dist = sqrt( (camX-msg->posX)*(camX-msg->posX) +
                   (camY-msg->posY)*(camY-msg->posY) +
                   (camZ-msg->posZ)*(camZ-msg->posZ) );
      scale = dist / 200.0;

      /* Draw */
      glColor3f(1.0, 1.0, 1.0);
      glBindTexture(GL_TEXTURE_2D, msg->messageTexture);
      glPushMatrix();
      glTranslatef(msg->posX, msg->posY, msg->posZ);
      glBegin(GL_QUADS);
         glTexCoord2f(0,1);
         glVertex3f(-msg->halfSize*modelView[0]*scale,0.0f,
                    -msg->halfSize*modelView[8]*scale);
         glTexCoord2f(1,1);
         glVertex3f(msg->halfSize*modelView[0]*scale,0.0f,
                    msg->halfSize*modelView[8]*scale);
         glTexCoord2f(1,0);
         glVertex3f(msg->halfSize*modelView[0]*scale + scale*8*modelView[1], 
                    scale*8*modelView[5] + scale*msg->halfSize*modelView[4], 
                    scale*msg->halfSize*modelView[8] + scale*8*modelView[9]);
         glTexCoord2f(0,0);
         glVertex3f(-msg->halfSize*modelView[0]*scale + scale*8*modelView[1], 
                    scale*8*modelView[5] - msg->halfSize*modelView[4]*scale,
                    -msg->halfSize*modelView[8]*scale + 8*modelView[9]*scale);
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
   glDisable(GL_TEXTURE_2D);
}



