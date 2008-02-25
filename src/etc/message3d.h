
#ifndef _message3d_h
#define _message3d_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;

#define MESSAGE3D_MAX_TIME  75 /**< max time showing the message */

/*! It's a message on the 3D world, that will be show, and will 
 * go upwards until time elapsed. */
class message3d
{
   public:
      /*! Constructor
       * \param x -> x position
       * \param y -> y position
       * \param z -> z position
       * \param msg -> message to show */
      message3d(GLfloat x, GLfloat y, GLfloat z, string msg);
      /*! Constructor
       * \param x -> x position
       * \param y -> y position
       * \param z -> z position
       * \param msg -> message to show
       * \param R -> red color [0..1]
       * \param G -> green color [0..1]
       * \param B -> blue color [0..1]*/
      message3d(GLfloat x, GLfloat y, GLfloat z, string msg,
                GLfloat R, GLfloat G, GLfloat B);
      /*! Destructor */
      ~message3d();

      /*! Set the controller as friend */
      friend class messageController;

   protected:
      /*! Init structs 
       * \param x -> x position
       * \param y -> y position
       * \param z -> z position
       * \param msg -> message to show
       * \param R -> red color [0..1]
       * \param G -> green color [0..1]
       * \param B -> blue color [0..1]*/
      void init(GLfloat x, GLfloat y, GLfloat z, string msg,
                GLfloat R, GLfloat G, GLfloat B);



      GLfloat posX,          /**< X position */
              posY,          /**< Y position */
              posZ;          /**< Z position */
      int size;              /**< Message Size (in pixels) */
      GLfloat halfSize;      /**< Half size */
      string message;        /**< the message to show */
      GLuint messageTexture; /**< The texture load by the message */
      int live;              /**< Time living */
      message3d* next;       /**< Next message on list */
      message3d* previous;   /**< Previous message on list */
};

/*! The message3d controller */
class messageController
{
   public:
      /*! Constructor */
      messageController();
      /*! Destructor */
      ~messageController();

      /*! Actualize and draw all messages
       * \param modelView -> the model view matrix
       * \param projection -> projection matrix 
       * \param viewport -> viewport vector */
      void draw(GLdouble modelView[16], GLdouble projection[16], 
                GLint viewPort[4]);

      /* Add a message
       * \param x -> x position
       * \param y -> y position
       * \param z -> z position
       * \param msg -> message to show */
      void addMessage(GLfloat x, GLfloat y, GLfloat z, string msg);

      /*! Add a message
       * \param x -> x position
       * \param y -> y position
       * \param z -> z position
       * \param msg -> message to show
       * \param R -> red color [0..1]
       * \param G -> green color [0..1]
       * \param B -> blue color [0..1]*/
      void addMessage(GLfloat x, GLfloat y, GLfloat z, string msg,
                      GLfloat R, GLfloat G, GLfloat B);


   protected:
      /*! Remove message from controller
       * \param msg -> pointer to the message to remove */
      void removeMessage(message3d* msg);

      /*! Add a message to the controller
       * \param m -> pointer to the message to add */
      void addMessage(message3d* m);

      message3d* first;    /**< The first message on the list */
      int total;           /**< Total messages on the list */
};

#endif

