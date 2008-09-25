
#ifndef _dnt_part_editor_h
#define _dnt_part_editor_h

#include "../../gui/farso.h"
#include "../../particle/part1.h"
#include "../../particle/part2.h"
#include "../../particle/part3.h"
#include "../../particle/part4.h"
#include "../../particle/part5.h"
#include "../../particle/part6.h"
#include "../../particle/part7.h"
#include "../../engine/culling.h"
#include "../../etc/dirs.h"
#include "../../engine/camera.h"

#include "partaux.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

/*! The partEditor main class */
class editor
{
   public:
      /*! Constructor */ 
      editor();
      /*! Destructor */
      ~editor();
      
      /*! Run the editor */
      void run();


   protected:

      /*! Create all Windows */
      void createWindows();

      /*! Treat GUI Related Events */
      void treatGuiEvents();

      /*! create (or Update) the viewPort
       * \param screen -> screen used */
      void createView(SDL_Surface *screen);

      /*! Create a Particle to edit
       * \param input -> particle filename from input 
       * \param partType -> particle type
       * \return -> true if can create, false otherwise */
      bool createParticle(int partType);

      /*! Delete the current particle */
      void deleteParticle();

      /*! Update the particle */
      void updateParticle();

      /*! Render things */
      void render();


      int type;    /**< Current particle type */
      partAux* p;  /**< The Current Particle */

      bool done;   /**< Flux Controll Variable */

      Uint8 mButton;  /**< Current Mouse Buttons Status */
      Uint8 *keys;    /**< Current Keyboard Status */
      int mouseX,     /**< Current Mouse X Coordinate */
          mouseY;     /**< Current Mouse Y Coordinate */


      SDL_Surface* screen; /**< The screen used */

      dirs dir;            /**< Directories Definition */
      extensions ext;      /**< OpenGL Extensions */
      guiInterface* gui;   /**< The farso Gui Used */

      camera* gameCamera; /**< The OpenGL Camera Used */

      button* buttonLoad; /**< Load Button */
      button* buttonSave; /**< Save Button */
      button* buttonExit; /**< Exit Button */

      /**********************File*Window*******************************/
      window* fileWindow;      /**< File Selector Window (for load/save) */
      fileSel* fileSelector;   /**< The file selector itself */
      bool fileLoading;        /**< If is loading or saving */
      void openFileWindow(bool load);

      string curFileName;      /**< Filename of Current Particle Open */

      GLfloat viewMatrix[6][4];  /**< Current Visible Matrix */
      GLdouble proj[16];         /**< Current Projection Matrix */
      GLdouble modl[16];         /**< Current Model View Matrix */
      GLint viewPort[4];         /**< Current View Port Vector */
};

#endif

