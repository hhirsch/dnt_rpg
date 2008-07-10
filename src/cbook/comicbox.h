#ifndef _dnt_comic_box_h
#define _dnt_comic_box_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

#define COMIC_BOX_STATUS_INACTIVE  0  /**< The box is inactive */
#define COMIC_BOX_STATUS_DONE      1  /**< The box ended its effects */
#define COMIC_BOX_STATUS_RUNNING   2  /**< The box is current running */

/*! The comic box is a quad of the comic page, with its show effect,
 * sound and page position */
class comicBox
{
   public:
      /*! Constructor */
      comicBox();
      /*! Destructor */
      ~comicBox();

      /*! Init the box effects, displaying it
       * \note -> a box must need to be activated to be render */
      void activate();

      /*! Get the current state of the box
       * \return -> comicBox state constant representing current status */
      int getStatus();

      /*! Render the box at its current state and effect */
      void render();

   protected:
      GLfloat vertex[4][3];  /**< The quad vertex */
      GLfloat scaleFactor;   /**< The quad scale factor */

      int effectType;        /**< The effect type */
      int status;            /**< Current quad status */
};

#endif

