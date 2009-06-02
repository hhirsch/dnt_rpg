/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _picture_h
#define _picture_h

#include "draw.h"
#include "guiobject.h"

/*! The Picture Class. A picture is just a image on the GUI */
class picture: public guiObject
{
   public:
      /*! Destructor */
      ~picture();

      /*! Constructor
       * \param x -> x initial coordinate
       * \param y -> y initial coordinate
       * \param w -> width of the picture
       * \param h -> height of the picture
       * \param arquivo -> fileName to opwn or NULL to not open a file */
      picture(int x,int y,int w,int h,const char* arquivo);

      /*! Draw the Picture on the surface
       * \param screen  -> surface to draw the picture */
      void draw(SDL_Surface *screen);

      /*! Set the picture to the surface
       * \param newPicture -> surface to set the picture */
      void set(SDL_Surface* newPicture);

      /*! Get the surface of the picture
       * \return -> surface of the picture */
      SDL_Surface* get();

      /*! Set the picture visible area
       * \param xa -> left
       * \param ya -> up
       * \param xb -> right 
       * \param yb -> bottom */
      void setVisibleArea(int xa, int ya, int xb, int yb);

      /*! Set the picture as all visible */
      void setAllVisible();


   protected:
      SDL_Surface *fig;     /**< A picture surface */
      SDL_Rect area;        /**< The visible area */

};


#endif

