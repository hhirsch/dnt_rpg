#ifndef _dnt_comicbook_h
#define _dnt_comicbook_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "comicpage.h"
#include "comicbox.h"

/*! The comic book class is the DNT cutscene abstraction */
class comicBook
{
   public:
      /*! Constructor */
      comicBook();
      /*! Destructor */
      ~comicBook();

      /*! Load a comicBook from file
       * \param bookFile -> comicBook file name 
       * \return -> true if the load was successfull */
      bool load(string bookFile);

      /*! Run the comic book */
      void run();

   protected:

      /*! Render the current page (with or without an scale effect)
       * \param curPage -> current page to render
       * \param scale -> scale factor to use */
      void render(comicPage* curPage, float scale=1.0);

      /*! Insert a page at the book pages list
       * \param t -> page title 
       * \return -> pointer to the comic page created */
      comicPage* insertPage(string t);

      /*! Empty the book, deleting all its pages */
      void empty();

      GLuint skipTexture; /**< The Skip Texture */
      float skipScale;    /**< The skip scale factor */
      float skipSum;      /**< The sum to the skip scale */


      comicPage* pages;   /**< Each comic book page  */
      int totalPages;     /**< Total book pages */
      string title;       /**< Book Title */
};

#endif

