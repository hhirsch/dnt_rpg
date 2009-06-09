/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

      /*! Treat text bars
       * \param obj -> text bar object where the event occurs */
      void treatTextBars(guiObject* obj);

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

      /*! Update all text bars when loading a particle */
      void updateTexts();


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


      /**********************Type*Window*******************************/
      window* typeWindow;       /**< Particle Type Window */
      button* waterButton;      /**< Water Type */
      button* fireButton;       /**< Fire Type */
      button* smokeButton;      /**< Smoke Type */
      button* bloodButton;      /**< Blood Type */
      button* lightningButton;  /**< Lightning Type */
      button* snowButton;       /**< Snow Type */
      void openTypeWindow();

      string curFileName;      /**< Filename of Current Particle Open */

      GLfloat** viewMatrix;      /**< Current Visible Matrix */
      GLdouble proj[16];         /**< Current Projection Matrix */
      GLdouble modl[16];         /**< Current Model View Matrix */
      GLint viewPort[4];         /**< Current View Port Vector */

      
      /**********************Edit*Window*******************************/
      /* One billion text bars...  */
      textBar* maxLiveEdit;
      textBar* maxPartsEdit;
      textBar* centerXEdit;
      textBar* centerYEdit;
      textBar* centerZEdit;
      textBar* gravityEdit;
      textBar* initREdit;
      textBar* initGEdit;
      textBar* initBEdit;
      textBar* finalREdit;
      textBar* finalGEdit;
      textBar* finalBEdit;
      textBar* alphaEdit;
      textBar* mCntXEdit;
      textBar* mCntYEdit;
      textBar* mCntZEdit;
      textBar* sCntXEdit;
      textBar* sCntYEdit;
      textBar* sCntZEdit;
      textBar* mPosXEdit;
      textBar* mPosYEdit;
      textBar* mPosZEdit;
      textBar* sPosXEdit;
      textBar* sPosYEdit;
      textBar* sPosZEdit;


      /**********************Edit*2*Window*****************************/
      /* Two billion text bars... */
      textBar* dMColorREdit;
      textBar* dMColorGEdit;
      textBar* dMColorBEdit;
      textBar* dSColorREdit;
      textBar* dSColorGEdit;
      textBar* dSColorBEdit;
      textBar* dMVelXEdit;
      textBar* dMVelYEdit;
      textBar* dMVelZEdit;
      textBar* dSVelXEdit;
      textBar* dSVelYEdit;
      textBar* dSVelZEdit;
      textBar* initVelXEdit;
      textBar* initVelYEdit;
      textBar* initVelZEdit;
};

#endif

