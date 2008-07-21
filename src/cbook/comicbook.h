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

      /*! Empty the book, deleting all its pages */
      void empty();

      comicPage* pages;   /**< Each comic book page  */
      int totalPages;     /**< Total book pages */
};

#endif

