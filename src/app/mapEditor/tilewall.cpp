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

#include <math.h>

#define TILE_SIZE  45
#define TILE_HALF  TILE_SIZE / 2

using namespace dntMapEditor;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
Tile::Tile(int indexi, int indexj)
{
   model = "";
   scNode = NULL;
   type = -1;
   i = indexi;
   j = indexj;
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
void Tile::changeModel(int t, std::string modelName)
{
   vec3_t pos;
   float angle;

   if(!scNode)
   {
      return;
   }

   if( (modelName != model) && (type != t) )
   {
      model = modelName;
      type = t;
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
   curTile = NULL;

   /* Must set tiles to change latter */
   format = ".md5def";
   baseName = "models/tiles/cave_1/cave1_";
   totalCenter = 4;
   totalLineCenter = 2;
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
   curTile = NULL;
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
std::string TileWall::randomCenter(int i)
{
   char buf[16];
   if(i > 1)
   {
      dice d(i);
      sprintf(buf, "%d", d.roll());
   }
   else
   {
      sprintf(buf, "%d", 1);
   }
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
   needed.x = round(needed.x);
   needed.y = round(needed.y);
   if(fabs(needed.x) < 1)
   {
      needed.x = (finalPos.x < initPos.x)?-1:1;
   }
   if(fabs(needed.y) < 1)
   {
      needed.y = (finalPos.y < initPos.y)?-1:1;
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
            tiles[i][j] = new Tile(i, j);
            tiles[i][j]->type = Tile::TYPE_SINGLE;
            tiles[i][j]->model = baseName+"single"+format;
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
            tiles[i][j] = new Tile(i, j);
            tiles[i][j]->type = Tile::TYPE_TOP;
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
   int type = Tile::TYPE_TOP;
   float angle=0.0f;

   /* Set absolute totals */
   int absX = (int)fabs(totalX);
   int absZ = (int)fabs(totalZ);

   /* Get signal value */
   int sigX = (totalX < 0)?-1:1;
   int sigZ = (totalZ < 0)?-1:1;

   /* Let's threat special cases, and general one */
   if((absX == 1) && (absZ == 1))
   {
      /* Special Case 1: 1x1 grid. only use a single-type mesh. */
      model = baseName+"single"+format;
      tiles[0][0]->changeModel(Tile::TYPE_SINGLE, model);
      tiles[0][0]->scNode->setAngle(0.0f, 0.0f, 0.0f);
      tiles[0][0]->scNode->setPosition(initPos.x, 0.0f, initPos.y);
      /* We're done! */
      return;
   }

   else if( (absX == 1) || (absZ == 1) )
   {
      /* Special Case 2: Single line grid. use line-type meshes. */
      for(i=0; i < absX; i++)
      {
         for(j=0; j < absZ; j++)
         {
            angle = (absX == 1)?90.0f:0.0f;
            if( ((i == 0) && (absZ == 1)) ||
                ((j == 0) && (absX == 1)) )
            {
               /* Initial Edge */
               model = baseName+"lside"+format;
               type = Tile::TYPE_LINE_SIDE;
               if(  ( (absZ == 1) && (sigX > 0) ) ||
                    ( (absX == 1) && (sigZ < 0) ) )
               {
                  angle += 180;
               }
            }
            else if( ((i == absX-1) && (absZ == 1)) ||
                     ((j == absZ-1) && (absX == 1)) )
            {
               /* Final Edge */
               model = baseName+"lside"+format;
               type = Tile::TYPE_LINE_SIDE;
               if(  ( (absZ == 1) && (sigX < 0) ) ||
                    ( (absX == 1) && (sigZ > 0) ) )
               {
                  angle += 180;
               }
            }
            else
            {
               /* Center */
               model = baseName+"lc"+randomCenter(totalLineCenter)+format;
               type = Tile::TYPE_LINE_CENTER;
            }
            /* Let's change the model, if needed */
            tiles[i][j]->changeModel(type, model);
            tiles[i][j]->scNode->setAngle(0.0f, angle, 0.0f);
            tiles[i][j]->scNode->setPosition(initPos.x+sigX*i*TILE_SIZE, 0.0f, 
                  initPos.y+sigZ*j*TILE_SIZE);
         }
      }

      /* We're done! */
      return;
   }

   /* General Case. Use normal meshes. */
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
            type = Tile::TYPE_SIDE;

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
            model = baseName+"c"+randomCenter(totalCenter)+format;
            type = Tile::TYPE_CENTER;
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
            model = baseName+"c"+randomCenter(totalCenter)+format;
            type = Tile::TYPE_CENTER;
            angle = (j==0)?90.0:270.0f;
            angle *= sigZ;
         }
         else
         {
            /* Top models */
            model = baseName+"top"+format;
            type = Tile::TYPE_TOP;
            angle = 0.0f;
         }
     
         /* Let's change the model, if needed */
         tiles[i][j]->changeModel(type, model);
         tiles[i][j]->scNode->setAngle(0.0f, angle, 0.0f);
         tiles[i][j]->scNode->setPosition(initPos.x+sigX*i*TILE_SIZE, 0.0f, 
                                          initPos.y+sigZ*j*TILE_SIZE);
      }
   }
}

/***********************************************************************
 *                           getTileUnder                              *
 ***********************************************************************/
Tile* TileWall::getTileUnder(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ)
{
   /* Set absolute totals */
   int i,j;
   int absX = (int)fabs(totalX);
   int absZ = (int)fabs(totalZ);

   /* Create a bounding box for mouse position */
   boundingBox mouseBox;
   mouseBox.setMin(mouseX-4, mouseY-4.0, mouseZ-4);
   mouseBox.setMax(mouseX+4, mouseY+4.0, mouseZ+4);

   /* Let's search throught all tiles */
   for(i=0; i < absX; i++)
   {
      for(j=0; j < absZ; j++)
      {
         if(tiles[i][j]->scNode)
         {
            if(tiles[i][j]->scNode->getBoundingBox().intercepts(mouseBox))
            {
               /* Got tile under */
               return(tiles[i][j]);
            }
         }
      }
   }

   /* No tile under */
   return(NULL);
}

/***********************************************************************
 *                           haveNeighbor                              *
 ***********************************************************************/
bool TileWall::haveNeighbor(Tile* t, int di, int dj, int absX, int absZ)
{
   if( (t->i+di >= 0) && (t->i+di < absX) &&
       (t->j+dj >= 0) && (t->j+dj < absZ) )
   {
      return(tiles[t->i+di][t->j+dj]->scNode != NULL);
   }

   return(false);
}

/***********************************************************************
 *                            removeTile                               *
 ***********************************************************************/
void TileWall::removeTile(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
      Uint8 mButton)
{
   Tile* neighTile;
   std::string model;
   int absX = (int)fabs(totalX);
   int absZ = (int)fabs(totalZ);
   
   /* Get tile under mouse */
   curTile = getTileUnder(mouseX, mouseY, mouseZ);

   /* Verify if tried to delete it! */
   if( (mButton & SDL_BUTTON(1)) && (curTile) )
   {
      /* TODO: Wait until release mouse button */

      /* Delete its scene node */
      curTile->deleteSceneNode();

      /* Verify Up tile style */
      if(haveNeighbor(curTile, 0, -1, absX, absZ))
      {
         neighTile = tiles[curTile->i][curTile->j-1];
         if(haveNeighbor(neighTile, 0, -1, absX, absZ))
         {
            model = baseName+"c"+randomCenter(totalCenter)+format;
            neighTile->changeModel(Tile::TYPE_CENTER, model);
            neighTile->scNode->setAngle(0.0f, 90.0f, 0.0f);
         }
         else
         {
            model = baseName+"lc"+randomCenter(totalLineCenter)+format;
            neighTile->changeModel(Tile::TYPE_LINE_CENTER, model);
            neighTile->scNode->setAngle(0.0f, 0.0f, 0.0f);
         }
      }

      /* Verify Down tile style */
      if(haveNeighbor(curTile, 0, 1, absX, absZ))
      {
         neighTile = tiles[curTile->i][curTile->j+1];
         if(haveNeighbor(neighTile, 0, 1, absX, absZ))
         {
            model = baseName+"c"+randomCenter(totalCenter)+format;
            neighTile->changeModel(Tile::TYPE_CENTER, model);
            neighTile->scNode->setAngle(0.0f, -90.0f, 0.0f);
         }
         else
         {
            model = baseName+"lc"+randomCenter(totalLineCenter)+format;
            neighTile->changeModel(Tile::TYPE_LINE_CENTER, model);
            neighTile->scNode->setAngle(0.0f, 180.0f, 0.0f);
         }
      }

      /* Verify left tile style */
      if(haveNeighbor(curTile, -1, 0, absX, absZ))
      {
         neighTile = tiles[curTile->i-1][curTile->j];
         if(haveNeighbor(neighTile, -1, 0, absX, absZ))
         {
            model = baseName+"c"+randomCenter(totalCenter)+format;
            neighTile->changeModel(Tile::TYPE_CENTER, model);
            neighTile->scNode->setAngle(0.0f, 180.0f, 0.0f);
         }
         else
         {
            model = baseName+"lc"+randomCenter(totalLineCenter)+format;
            neighTile->changeModel(Tile::TYPE_LINE_CENTER, model);
            neighTile->scNode->setAngle(0.0f, 90.0f, 0.0f);
         }
      }
      
      /* Verify Right tile style */
      if(haveNeighbor(curTile, 1, 0, absX, absZ))
      {
         neighTile = tiles[curTile->i+1][curTile->j];
         if(haveNeighbor(neighTile, 1, 0, absX, absZ))
         {
            model = baseName+"c"+randomCenter(totalCenter)+format;
            neighTile->changeModel(Tile::TYPE_CENTER, model);
            neighTile->scNode->setAngle(0.0f, 0.0f, 0.0f);
         }
         else
         {
            model = baseName+"lc"+randomCenter(totalLineCenter)+format;
            neighTile->changeModel(Tile::TYPE_LINE_CENTER, model);
            neighTile->scNode->setAngle(0.0f, -90.0f, 0.0f);
         }
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
   else if(tool == TOOL_TILE_WALL_REMOVE)
   {
      removeTile(mouseX, mouseY, mouseZ, mButton);
   }
}

/***********************************************************************
 *                          drawTemporary                              *
 ***********************************************************************/
void TileWall::drawTemporary()
{
   if((curTile) && (curTile->scNode))
   {
      glPushMatrix();
         curTile->scNode->getBoundingBox().render();
      glPopMatrix();
   }
}


