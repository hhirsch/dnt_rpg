/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_part_editor_h
#define _dnt_part_editor_h

#include "../../gui/farso.h"
#include "../../particle/particle.h"
#include "../../engine/culling.h"
#include "../../etc/dirs.h"
#include "../../engine/camera.h"

#include "partaux.h"
#include "partelementwindow.h"
#include "partwindow.h"

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
       * \return -> true if can create, false otherwise */
      bool createParticle();

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
      Farso::GuiInterface* gui;   /**< The farso Gui Used */

      camera* gameCamera; /**< The OpenGL Camera Used */

      Farso::Button* buttonLoad; /**< Load Button */
      Farso::Button* buttonSave; /**< Save Button */
      Farso::Button* buttonExit; /**< Exit Button */

      Farso::Window* actWindow;
      partWindow* particleWindow;        /**< Window to edit particle */

      /**********************File*Window*******************************/
      Farso::Window* fileWindow;  /**< File Selector Window (for load/save) */
      Farso::FileSel* fileSelector;   /**< The file selector itself */
      bool fileLoading;        /**< If is loading or saving */
      void openFileWindow(bool load);


      std::string curFileName;      /**< Filename of Current Particle Open */

      GLfloat** viewMatrix;      /**< Current Visible Matrix */
      GLdouble proj[16];         /**< Current Projection Matrix */
      GLdouble modl[16];         /**< Current Model View Matrix */
      GLint viewPort[4];         /**< Current View Port Vector */
};

#endif

