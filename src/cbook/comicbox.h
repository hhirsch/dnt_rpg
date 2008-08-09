#ifndef _dnt_comic_box_h
#define _dnt_comic_box_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

#include <string>
using namespace std;

/* Wait Time */
#define COMIC_BOX_DEFAULT_TIMEOUT  5000 /**< Default Box Wait time (5 sec) */

/* Status  */
#define COMIC_BOX_STATUS_INACTIVE  0  /**< The box is inactive */
#define COMIC_BOX_STATUS_DONE      1  /**< The box ended its effects */
#define COMIC_BOX_STATUS_RUNNING   2  /**< The box is current running */

/* Effects */
#define COMIC_BOX_EFFECT_NONE      0  /**< No Effect */
#define COMIC_BOX_EFFECT_SCALE     1  /**< Scale the Box to the size effect */

/*! The comic box is a quad of the comic page, with its show effect,
 * sound and page position */
class comicBox
{
   public:
      /*! Constructor 
       * \param t -> box title */
      comicBox(string t);
      /*! Destructor */
      ~comicBox();

      /*! Set the effect type
       * \param type -> the new effect type to use */
      void setEffect(int type);

      /*! Set the box timeout
       * \param value -> timeout (in ms) */
      void setTimeout(int value);

      /*! Set a box vertex
       * \param vertNumber -> the vertex number [0,3]
       * \param x -> x coordinate 
       * \param y -> y coordinate
       * \param imgX -> current image width
       * \param imgY -> current image height */
      void setVertex(int vertNumber, GLfloat x, GLfloat y, int imgX, int imgY);

      /*! Init the box effects, displaying it
       * \note -> a box must need to be activated to be rendered */
      void activate();

      /*! Get the current state of the box
       * \return -> comicBox state constant representing current status */
      int getStatus();

      /*! Render the box at its current state and effect */
      void render();

      /*! Set the next box 
       * \param box -> next box */
      void setNext(comicBox* box);
      /*! Set the previous box
       * \param page -> previous box */
      void setPrevious(comicBox* box);

      /*! Get the next box 
       * \return next box */
      comicBox* getNext();
      /*! Get the previous box
       * \return previous box */
      comicBox* getPrevious();

      /*! Get the box title
       * \return -> title string */
      string getTitle();

   protected:

      /*! Update the comic box */
      void update();

      GLfloat vertex[4][2];    /**< The quad vertex */
      GLfloat texCoord[4][2];  /**< The Vertex texture */
      GLfloat scaleFactorX;    /**< The quad scale factor for X axys */
      GLfloat scaleFactorY;    /**< The quad scale factor for Y axys */

      GLfloat center[2];       /**< The center Coordinates */

      GLfloat pos[2];        /**< Current Translation */
      GLfloat angle[2];      /**< Current Rotation */
      GLfloat scale[2];      /**< Current Scale (without scale Factor) */

      int effectType;        /**< The effect type */
      int status;            /**< Current quad status */

      Uint32 timer;          /**< a timer */

      Uint32 timeout;        /**< Box Wait time */

      string title;          /**< title  */

      comicBox* next;        /**< Next Box on list */
      comicBox* previous;    /**< Previous Box on list */
};

#endif

