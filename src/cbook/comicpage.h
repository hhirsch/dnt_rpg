
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

   protected:

      comicPage* next;     /**< Next Page of the comic book */
      comicPage* previous; /**< Previous Page of the comic book */

      comicBox* boxes;  /**< Boxes List */
      int totalBoxes;   /**< Total Boxes on the list */
};

#endif


