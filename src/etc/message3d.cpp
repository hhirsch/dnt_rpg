#include "message3d.h"
#include "../gui/dntfont.h"
#include "../gui/draw.h"
#include "../engine/options.h"


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
   fnt.defineFont(DNT_FONT_ARIAL, 12);
   size = fnt.getStringWidth(message) + 8;
   halfSize = (size / 2.0);
   color_Set(0,0,0);
   SDL_Surface* s = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                         smallestPowerOfTwo(size),
                                         32,
                                         32, rmask, gmask, bmask, amask);
   color_Set((int)floor(R*255),(int)floor(G*255),(int)floor(B*255));
   fnt.write(s, 0, 0, message.c_str(), true);

   glGenTextures(1,&messageTexture);
   setTextureRGBA(s, messageTexture);

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
void messageController::draw(GLdouble modelView[16], GLdouble projection[16], 
                             GLint viewPort[4])
{
   int i;
   int tot = total;
   message3d* msg = first;
   options option;

   if(option.antiAliasing)
   {
      glDisable(GL_MULTISAMPLE);
   }

   GLdouble winX=0, winY=0, winZ=0;

   /*GLfloat scale = 1.0;
   GLfloat dist = 0;*/

   GLfloat factor = 1.0;

   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   for(i = 0; i < tot; i++)
   {
      if(! (gluProject(msg->posX, msg->posY, msg->posZ,
                       modelView, projection, viewPort,
                       &winX, &winY, &winZ) ) )
      {
         /* Just for debug!  */
         printf("no!\n");
      }

      
      /* Calculate Texture factor */
      factor = (float)(msg->size) / (float)(smallestPowerOfTwo(msg->size));

      /* Draw */
      glColor3f(1.0, 1.0, 1.0);
      glBindTexture(GL_TEXTURE_2D, msg->messageTexture);
      glPushMatrix();
      glBegin(GL_QUADS);
         glTexCoord2f(0,1);
         glVertex2f(winX - msg->halfSize, winY - 16);
         glTexCoord2f(factor,1);
         glVertex2f(winX + msg->halfSize, winY - 16);
         glTexCoord2f(factor,0);
         glVertex2f(winX + msg->halfSize, winY + 16);
         glTexCoord2f(0,0);
         glVertex2f(winX - msg->halfSize, winY + 16);
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

   if(option.antiAliasing)
   {
      glEnable(GL_MULTISAMPLE);
   }

   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glEnable(GL_FOG);
}



