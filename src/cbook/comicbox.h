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

#ifndef _dnt_comic_box_h
#define _dnt_comic_box_h

#include "../etc/list.h"

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

#include <string>

/* Wait Time */
#define COMIC_BOX_DEFAULT_TIMEOUT  5000 /**< Default Box Wait time (5 sec) */

/* Status  */
#define COMIC_BOX_STATUS_INACTIVE  0  /**< The box is inactive */
#define COMIC_BOX_STATUS_DONE      1  /**< The box ended its effects */
#define COMIC_BOX_STATUS_RUNNING   2  /**< The box is current running */

/* Effects */
#define COMIC_BOX_EFFECT_NONE      0  /**< No Effect */
#define COMIC_BOX_EFFECT_SCALE     1  /**< Scale the Box to the size effect */
#define COMIC_BOX_EFFECT_ROTATE    2  /**< Rotate the Box scalling effect */

/*! The comic box is a quad of the comic page, with its show effect,
 * sound and page position */
class comicBox: public dntListElement
{
   public:
      /*! Constructor 
       * \param t -> box title */
      comicBox(std::string t);
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

      /*! Skip the box effect, putting it at COMIC_BOX_STATUS_DONE state */
      void skip();

      /*! Get the current state of the box
       * \return -> comicBox state constant representing current status */
      int getStatus();

      /*! Render the box at its current state and effect */
      void render();

      /*! Get the box title
       * \return -> title string */
      std::string getTitle();

   protected:

      /*! Update the comic box */
      void update();

      GLfloat vertex[4][2];    /**< The quad vertex */
      GLfloat texCoord[4][2];  /**< The Vertex texture */
      GLfloat scaleFactorX;    /**< The quad scale factor for X axys */
      GLfloat scaleFactorY;    /**< The quad scale factor for Y axys */

      GLfloat center[2];       /**< The center Coordinates */

      GLfloat pos[2];        /**< Current Translation */
      GLfloat angle[3];      /**< Current Rotation */
      GLfloat scale[2];      /**< Current Scale (without scale Factor) */

      int effectType;        /**< The effect type */
      int status;            /**< Current quad status */

      Uint32 timer;          /**< a timer */

      Uint32 timeout;        /**< Box Wait time */

      std::string title;     /**< title  */
};

/*! The chain list of comic boxes */
class comicBoxList: public dntList
{
   public:
      /*! Constructor */
      comicBoxList();
      /*! Destructor */
      ~comicBoxList();

   protected:
      /*! Delete a comicBox pointer
       * \paramn obj -> pointer to the comicBox */
      void freeElement(dntListElement* obj);
};

#endif

