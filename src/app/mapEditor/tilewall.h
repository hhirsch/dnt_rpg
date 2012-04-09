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

#ifndef _dnt_map_editor_tile_wall_h
#define _dnt_map_editor_tile_wall_h

#include <vector>

#include "../../map/map.h"
#include "../../etc/quaternion.h"
#include "message.h"


namespace dntMapEditor
{

class Tile
{
   public:
      /*! Constructor */
      Tile();
      /*! Destructor */
      ~Tile();
      /*! Create tile sceneNode */
      void createSceneNode(float posX, float posY, float posZ, float angleY);
      /*! Delete tile sceneNode */
      void deleteSceneNode();
      /*! Change current tile model */
      void changeModel(std::string modelName);

      std::string model; /**< Model of this tile */
      sceneNode* scNode; /**< Scene node related to this tile */
};

class TileWall
{
   public:
      /*! Constructor */
      TileWall(Map* acMap);
      /*! Destructor */
      ~TileWall();

      /*! Clear internal things. Called when close an edited map. */
      void clear();

      /*! Flush current tiles to the map, as objects */
      void flush();

      /*! Verify Action on tile Wall */
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ,
            GLfloat floorX, GLfloat floorZ,
            Uint8 mButton, Uint8* keys, int tool);

   protected:
      enum TileWallState
      {
         STATE_OTHER=0,
         STATE_ADD_INIT
      };

      /*! Set the existing tiles from initial to current position */
      void setTiles();
      void setTileModels();
      /*! Get random a center tile number */
      std::string randomCenter(int total);

      std::vector< std::vector<Tile*> > tiles; /**< Current created tiles */
      Map* curMap;     /**< Current on-edit map */
      int state;       /**< Current state */

      vec2_t initPos;   /**< Initial Position */
      vec2_t finalPos;  /**< Current Position */
      int totalX;       /**< total X, with direction signal */
      int totalZ;       /**< total Z, with direction signal */

      std::string format;   /**< tile model format (with .) */
      std::string baseName; /**< tile base name */
      int totalCenter;      /**< Number of center tiles */
      int totalLineCenter;  /**< Number of line center tiles */
};

}

#endif


