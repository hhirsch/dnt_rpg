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

#include "tilewall.h"
#include "message.h"

#define TILE_SIZE  45
#define TILE_HALF  TILE_SIZE / 2

using namespace dntMapEditor;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
Tile::Tile()
{
   model = "";
   scNode = NULL;
}
/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
Tile::~Tile()
{
   deleteSceneNode();
}
/***********************************************************************
 *                           deleteSceneNode                           *
 ***********************************************************************/
void Tile::deleteSceneNode()
{
   if(scNode)
   {
      scene sc;
      sc.deleteSceneNode(scNode);
   }
   scNode = NULL;
}
/***********************************************************************
 *                            changeObject                             *
 ***********************************************************************/
void Tile::changeModel(std::string modelName)
{
   vec3_t pos;
   float angle;

   if(!scNode)
   {
      return;
   }

   if(modelName != model)
   {
      model = modelName;
      /* Retrieve current position and angles */
      pos.x = scNode->getPosX();
      pos.y = scNode->getPosY();
      pos.z = scNode->getPosZ();
      angle = scNode->getAngleY();
      /* Delete current sceneNode and create a new */
      deleteSceneNode();
      createSceneNode(pos.x, pos.y, pos.z, angle);
   }
}

/***********************************************************************
 *                           createSceneNode                           *
 ***********************************************************************/
void Tile::createSceneNode(float posX, float posY, float posZ, float angleY)
{
   scene sc;
   if(scNode)
   {
      deleteSceneNode();
   }

   scNode = sc.createSceneNode(true, model, posX, posY, posZ, 
         0.0f, angleY, 0.0f);
}

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
TileWall::TileWall(Map* acMap)
{
   state = STATE_OTHER;
   curMap = acMap;
   totalX = 0;
   totalZ = 0;

   /* Must set tiles to change latter */
   format = ".md5def";
   baseName = "models/tiles/cave_1/cave1_";
   totalCenter = 4;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
TileWall::~TileWall()
{
   clear();
}

/***********************************************************************
 *                               clear                                 *
 ***********************************************************************/
void TileWall::clear()
{
   size_t i,j;
   /* Delete all Tiles */
   for(i=0; i < tiles.size(); i++)
   {
      for(j=0; j < tiles[i].size(); j++)
      {
         delete(tiles[i][j]);
      }
      tiles[i].resize(0);
   }
   tiles.resize(0);

   state = STATE_OTHER;
   totalX = 0;
   totalZ = 0;
}

/***********************************************************************
 *                               flush                                 *
 ***********************************************************************/
void TileWall::flush()
{
   /* TODO */
}

/***********************************************************************
 *                             randomCenter                            *
 ***********************************************************************/
std::string TileWall::randomCenter()
{
   dice d(totalCenter);
   char buf[16];
   sprintf(buf, "%d", d.roll());
   return(buf);
}

/***********************************************************************
 *                             setTiles                                *
 ***********************************************************************/
void TileWall::setTiles()
{
   vec2_t needed;
   int absX, prevX;
   int absZ, prevZ;
   int i,j;

   /* First, let's calculate needed totals */
   needed = (finalPos - initPos) / TILE_SIZE;
   if(fabs(needed.x) < 2)
   {
      needed.x = (finalPos.x < initPos.x)?-2:2;
   }
   if(fabs(needed.y) < 2)
   {
      needed.y = (finalPos.y < initPos.y)?-2:2;
   }

   //printf("Total: %d %d\nNeeded: %d %d\n", totalX, totalZ, 
   //       (int)needed.x, (int)needed.y);
   /* Verify if need to update the tiles */
   if( (totalX == (int)needed.x) && (totalZ == (int)needed.y) )
   {
      /* Same as current, no need to update */
      return;
   }

   totalX = (int)needed.x;
   totalZ = (int)needed.y;
   absX = (int)fabs(totalX);
   absZ = (int)fabs(totalZ);
   prevX = (int)tiles.size();

   /* Let's update X axys */
   if(absX > (int)tiles.size())
   {
      /* Resize the vector */
      tiles.resize(absX);

      /* Must create new Z elements */
      for(i = prevX; i < absX; i++)
      {
         /* Must create sceneNodes for all */
         tiles[i].resize(absZ);
         for(j=0; j < absZ; j++)
         {
            tiles[i][j] = new Tile();
            tiles[i][j]->model = baseName+"top"+format;
            tiles[i][j]->createSceneNode(0.0f, 0.0f, 0.0f, 0.0f); 
         }
      }
   }
   else if(absX < (int)tiles.size())
   {
      /* Delete all no more needed objects; */
      for(i=absX; i < (int)tiles.size(); i++)
      {
         for(j=0; j < (int)tiles[i].size(); j++)
         {
            delete(tiles[i][j]);
         }
      }
      /* Resize the vector */
      tiles.resize(absX);
   }

   /* Let's update Z axys */
   for(i=0; i < absX; i++)
   {
      prevZ = (int)tiles[i].size();
      if(absZ > prevZ)
      {
         /* Must create new models at Z */
         tiles[i].resize(absZ);
         for(j=prevZ; j < absZ; j++)
         {
            tiles[i][j] = new Tile();
            tiles[i][j]->model = baseName+"top"+format;
            tiles[i][j]->createSceneNode(0.0f, 0.0f, 0.0f, 0.0f);
         }
      }
      else if(absZ < prevZ)
      {
         /* Must delete no more needed models at Z */
         for(j=absZ; j < prevZ; j++)
         {
            delete(tiles[i][j]);
         }
         /* Finally, resize vector */
         tiles[i].resize(absZ);
      }
   }

   setTileModels();
}

/***********************************************************************
 *                           setTileModels                             *
 ***********************************************************************/
void TileWall::setTileModels()
{
   int i,j;
   std::string model;
   float angle=0.0f;

   /* Set absolute totals */
   int absX = (int)fabs(totalX);
   int absZ = (int)fabs(totalZ);

   /* Get signal value */
   int sigX = (totalX < 0)?-1:1;
   int sigZ = (totalZ < 0)?-1:1;

   for(i=0; i < absX; i++)
   {
      for(j=0; j < absZ; j++)
      {
         /* Sides */
         if(  ( (i == 0) || (i == absX-1) ) &&
              ( (j == 0) || (j == absZ-1) ) )
         {
            /* Set its model to side one */
            model = baseName+"side"+format;

            /* Let's set each angle */
            if( (i == 0) && (j == 0) )
            {
               if(sigX > 0)
               {
                  angle = (sigZ > 0)?180:270;
               }
               else
               {
                  angle = (sigZ > 0)?90:0;
               }
            }
            else if( (i == 0) && (j == absZ-1) )
            {
               if(sigX > 0)
               {
                  angle = (sigZ > 0)?270:180;
               }
               else
               {
                  angle = (sigZ > 0)?0:90;
               }
            }
            else if( (i == absX-1) && (j == absZ-1) )
            {
               if(sigX > 0)
               {
                  angle = (sigZ > 0)?0:90;
               }
               else
               {
                  angle = (sigZ > 0)?270:180;
               }
            }
            else if( (i == absX-1) && (j == 0) )
            {
               if(sigX > 0)
               {
                  angle = (sigZ > 0)?90:0;
               }
               else
               {
                  angle = (sigZ > 0)?180:270;
               }
            }
         }
         else if((i == 0) || (i == absX-1))
         {
            /* Model to center closed-X one */
            model = baseName+"c"+randomCenter()+format;
            if(sigX > 0)
            {
               angle = (i==0)?180.0:0.0f;
            }
            else
            {
               angle = (i==0)?0.0:180.0f;
            }
         }
         else if( (j == 0) || (j == absZ-1) )
         {
            /* Model to center closed-Z one */
            model = baseName+"c"+randomCenter()+format;
            angle = (j==0)?90.0:270.0f;
            angle *= sigZ;
         }
         else
         {
            /* Top models */
            model = baseName+"top"+format;
            angle = 0.0f;
         }
     
         /* Let's change the model, if needed */
         tiles[i][j]->changeModel(model);
         tiles[i][j]->scNode->setAngle(0.0f, angle, 0.0f);
         tiles[i][j]->scNode->setPosition(initPos.x+sigX*i*TILE_SIZE, 0.0f, 
                                          initPos.y+sigZ*j*TILE_SIZE);
      }
   }
}


/***********************************************************************
 *                           verifyactions                             *
 ***********************************************************************/
void TileWall::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ,
      GLfloat floorX, GLfloat floorZ,
      Uint8 mButton, Uint8* keys, int tool)
{
   if(tool == TOOL_TILE_WALL_ADD)
   {
      if( (state == STATE_OTHER) && (mButton & SDL_BUTTON(1)))
      {
         state = STATE_ADD_INIT;
         /*! Set initial position */
         initPos.x = floorX;
         initPos.y = floorZ;
         finalPos.x = floorX;
         finalPos.y = floorZ;
         /*! Set min tiles (2x2 side-tiles) */
         setTiles();
      }
      else if((state == STATE_ADD_INIT) && (mButton & SDL_BUTTON(1)))
      {
         /*! Set tiles up to the current position */
         finalPos.x = floorX;
         finalPos.y = floorZ;
         setTiles();
      }
      else if( (state == STATE_ADD_INIT) && (!(mButton & SDL_BUTTON(1))))
      {
         /* Done inserting, real add it to the map */
         state = STATE_OTHER;
         //flush();
      }
   }
}


