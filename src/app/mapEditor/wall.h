/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_map_editor_wall_h
#define _dnt_map_editor_wall_h

#include "../../map/map.h"
#include "../../gui/draw.h"
#include "message.h"

namespace dntMapEditor
{

/*! The editor of walls */
class WallController
{
   public:
      /*! Constructor 
       * \param acMap -> pointer to opened map
       * \param g -> pointer to the gui interface */
      WallController(Map* acMap, Farso::GuiInterface* g);
      /*! Destructor */
      ~WallController();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param floorX -> mouse X coordinate on floor
       * \param floorZ -> mouse Z coordinate on floor
       * \param mButton -> mouse button state
       * \param tool -> current Tool
       * \param curTexture -> current Texture Id 
       * \param curTextureName -> current texture name */
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        GLfloat floorX, GLfloat floorZ,
                        Uint8 mButton, Uint8* keys, int& tool, 
                        GLuint curTexture, std::string curTextureName);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary(GLdouble modelView[16], 
                         GLfloat camX, GLfloat camY, GLfloat camZ);

      /*! Treat GUI events
       * \return true if some event occurred here */
      bool eventGot(int eventInfo, Farso::GuiObject* obj);


   private:
      Map* actualMap;         /**< Actual Internal Map */
      int actualTool;         /**< Actual Tool */
      int state;              /**< Actual Internal State */
      wall* actualWall;       /**< Actual Selected Wall */
      bool limitSquare;       /**< Limit Wall to square's vertices*/

      GLuint texture;           /**< Current Texture */
      std::string textureName;   /**< Current Texture Name */

      GLfloat mX, mY, mZ;     /**< Mouse World Coordinates */
      GLfloat fX, fZ;         /**< Mouse Floor Coordinates */

      Uint8 mB;               /**< Mouse Buttons */

      GLfloat initmX, initmZ; /**< Mouse Initial Positions on some States */

      GLuint markTexture;     /**< Actual Wall mark Texture */
      GLuint faceMarkTexture; /**< Current wall face mark texture */

      Farso::GuiInterface* gui;
      Farso::Window* detailWindow;
      Farso::TabButton* detTab;
      Farso::OneTabButton* incX;
      Farso::OneTabButton* decX;
      Farso::OneTabButton* incZ;
      Farso::OneTabButton* decZ;
      wall* detailWall;

      Farso::Window* faceWindow;
      Farso::Button* incXRep;
      Farso::TextBar* curXRep;
      Farso::Button* decXRep;
      Farso::Button* incYRep;
      Farso::TextBar* curYRep;
      Farso::Button* decYRep;
      Farso::Button* incZRep;
      Farso::TextBar* curZRep;
      Farso::Button* decZRep;
      wallTexture* curFace;
      GLuint repX, repY, repZ;

      /*! Open the face window for current face */
      void openFaceWindow();
      /*! Set values on face window */
      void setFaceValues();

      /*! Gets wall on actual Mouse Position
       * \param radius -> delta from mouse position to verify wall in.
       * \return -> pointer to the wall under the mouse or NULL */
      wall* getWall(int radius=1);
      /*! Get the texture of the wall side the mouse is pointing to
       * \return pointer to the wall side texture or NULL */
      wallTexture* getSideTexture(float* x1=NULL, float* z1=NULL,
            float* x2=NULL, float* z2=NULL);
      /*! Do the modifications in Horizontal Vertical Texture repeat */
      void doModifyVerHorTexture();
      /*! Do the modification on texture */
      void doTexture();
      /*! Do the wall cut
       * \return -> true if cut, false if not cut yet*/
      bool doCut();
      /*! Do The Wall Construction */
      void doWall();
      /*! Do the wall destroy
       * \return true if destroyed a wall */
      bool doDestroy();
};

}

#endif

