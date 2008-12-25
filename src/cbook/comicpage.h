
#ifndef _dnt_comic_page_h
#define _dnt_comic_page_h

#include "comicbox.h"

/*! The Comic Page is the representation of a page,
 * including all comic boxes and its effects */
class comicPage
{
   public:
      /*! Constructor */
      comicPage();
      /*! Destructor */
      ~comicPage();

      /*! Set the next page 
       * \param page -> next page */
      void setNext(comicPage* page);
      /*! Set the previous page
       * \param page -> previous page */
      void setPrevious(comicPage* page);

      /*! Get the next page 
       * \return next page */
      comicPage* getNext();
      /*! Get the previous page
       * \return previous page */
      comicPage* getPrevious();

      /*! Insert a comic box at the page
       * \param box -> comicBox to insert */
      void insertBox(comicBox* box);

      /*! Insert (draw) a text to the page texture
       * \param x1 -> x first coordinate 
       * \param y1 -> y first coordinate
       * \param x2 -> x last coordinate
       * \param y2 -> y last coordinate
       * \param text -> string with text to insert */
      void insertText(int x1, int y1, int x2, int y2, string text); 
      
      /*! Insert (draw) a text to the page texture
       * \param x1 -> x first coordinate 
       * \param y1 -> y first coordinate
       * \param x2 -> x last coordinate
       * \param y2 -> y last coordinate
       * \param text -> string with text to insert
       * \param fontSize -> font size to use 
       * \param fontStyle -> font style to use 
       * \param R -> red color
       * \param G -> green color
       * \param B -. blue color */
      void insertText(int x1, int y1, int x2, int y2, string text,
                      int fontSize, int fontStyle, int R, int G, int B);

      /*! Set the texture
       * \textureFile -> image file to use as texture
       * \return -> true if can load the image */
      bool defineTexture(string textureFile);

      /*! Flush the SDL texture to an openGL texture */
      void flushTexture();

      /*! Get the first box on the list
       * \return -> pointer to the first box */
      comicBox* getFirstBox();
      /*! Get total boxes
       * \return -> total number of boxes on page */
      int getTotalBoxes();

      /*! Render the page (updating the active box) */
      void render();

      /*! Get the current page texture width
       * \return -> page width */
      int getWidth();

      /*! Get the current page texture height
       * \return -> page height */
      int getHeight();

   protected:

      comicPage* next;     /**< Next Page of the comic book */
      comicPage* previous; /**< Previous Page of the comic book */

      SDL_Surface* texture; /**< Page SDL Texture */
      GLuint tex;           /**< Page OpenGL Texture */

      comicBox* boxes;  /**< Boxes List */
      int totalBoxes;   /**< Total Boxes on the list */
};

#endif


