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

   /*baseName = "models/tiles/temple/temple_";
   totalCenter = 1;
   totalLineCenter = 1;*/
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
 *                               getModel                              *
 ***********************************************************************/
std::string TileWall::getModel(int type)
{
   std::string ret;
   switch(type)
   {
      case Tile::TYPE_SINGLE:
      {
         ret = baseName + "single" + format;
      }
      break;
      case Tile::TYPE_TOP:
      {
         ret = baseName + "top" + format;
      }
      break;
      case Tile::TYPE_TOP_SIDE_1:
      {
         ret = baseName + "top_s1" + format;
      }
      break;
      case Tile::TYPE_TOP_SIDE_2:
      {
         ret = baseName + "top_s2" + format;
      }
      break;
      case Tile::TYPE_TOP_SIDE_2_2:
      {
         ret = baseName + "top_s2_2" + format;
      }
      break;
      case Tile::TYPE_TOP_SIDE_3:
      {
         ret = baseName + "top_s3" + format;
      }
      break;
      case Tile::TYPE_TOP_SIDE_4:
      {
         ret = baseName + "top_s4" + format;
      }
      break;
      case Tile::TYPE_CENTER:
      {
         ret = baseName+"c"+randomCenter(totalCenter)+format;
      }
      break;
      case Tile::TYPE_CENTER_SIDE_1:
      {
         ret = baseName+"c1_s1"+format;
      }
      break;
      case Tile::TYPE_CENTER_SIDE_1_2:
      {
         ret = baseName+"c1_s1_2"+format;
      }
      break;
      case Tile::TYPE_CENTER_SIDE_2:
      {
         ret = baseName+"c1_s2"+format;
      }
      break;
      case Tile::TYPE_LINE_CENTER:
      {
         ret = baseName+"lc"+randomCenter(totalLineCenter)+format;
      }
      break;
      case Tile::TYPE_SIDE:
      {
         ret = baseName+"side"+format;
      }
      break;
      case Tile::TYPE_SIDE_SIDE:
      {
         ret = baseName+"side_s"+format;
      }
      break;
      case Tile::TYPE_LINE_SIDE:
      {
         ret = baseName+"lside"+format;
      }
      break;
   }

   return(ret);
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
   int type = Tile::TYPE_TOP;
   float angle=0.0f;

   float iX = initPos.x;
   float iZ = initPos.y;

   int numSideNeighbors;
   int numDiagonalNeighbors;

   /* Set absolute totals */
   int absX = (int)fabs(totalX);
   int absZ = (int)fabs(totalZ);

   if(totalX < 0)
   {
      iX = finalPos.x;
   }
   if(totalZ < 0)
   {
      iZ = finalPos.y;
   }

   /* Let's set all tiles */
   for(i=0; i < absX; i++)
   {
      for(j=0; j < absZ; j++)
      {
         /* Only set existing tiles */
         if(tiles[i][j]->scNode)
         {
            /* Let's count how many neighboors it has */
            numSideNeighbors = numTileSideNeighbors(tiles[i][j], absX, absZ);
            numDiagonalNeighbors = numTileDiagonalNeighbors(tiles[i][j], 
                  absX, absZ);

            /* And set its model by the neighbors */
            if(numSideNeighbors == 0)
            {
               /* No side neighbors case. Must be a single.*/
               angle = 0.0f;
               type = Tile::TYPE_SINGLE;
            }
            else if(numSideNeighbors == 1)
            {
               /* just one side neighbors*/
               angle = 0.0f;
               type = Tile::TYPE_LINE_SIDE;
               /* Set angles */
               if(haveNeighbor(tiles[i][j], 1, 0, absX, absZ))
               {
                  /* First X */
                  angle = 180.0f;
               }
               else if(haveNeighbor(tiles[i][j], 0, -1, absX, absZ))
               {
                  /* Final Z */
                  angle = -90.0f;
               }
               else if(haveNeighbor(tiles[i][j], 0, 1, absX, absZ))
               {
                  /* First Z */
                  angle = 90.0f;
               }
            }
            else if(numSideNeighbors == 2)
            {
               /* Two side neighbors. Side or Middle line? */
               if( (haveNeighbor(tiles[i][j], -1, 0, absX, absZ)) &&
                   (haveNeighbor(tiles[i][j], 1, 0, absX, absZ)) )
               {
                  /* middle X line */
                  type = Tile::TYPE_LINE_CENTER;
                  angle = 0.0f;
               }
               else if( (haveNeighbor(tiles[i][j], 0, -1, absX, absZ)) &&
                        (haveNeighbor(tiles[i][j], 0, 1, absX, absZ)) )
               {
                  /* middle Z line */
                  type = Tile::TYPE_LINE_CENTER;
                  angle = 90.0f;
               }
               else
               {
                  /* must be a side. verify each side type. */
                  type = Tile::TYPE_SIDE;

                  /* Let's set angle */
                  if( (haveNeighbor(tiles[i][j], -1, 0, absX, absZ)) &&
                      (haveNeighbor(tiles[i][j], 0, 1, absX, absZ)) )
                  {
                     angle = 90.0f;
                     if(!haveNeighbor(tiles[i][j], -1, 1, absX, absZ))
                     {
                        type = Tile::TYPE_SIDE_SIDE;
                     }
                  }
                  else if( (haveNeighbor(tiles[i][j], 1, 0, absX, absZ)) &&
                           (haveNeighbor(tiles[i][j], 0, 1, absX, absZ)) )
                  {
                     angle = 180.0f;
                     if(!haveNeighbor(tiles[i][j], 1, 1, absX, absZ))
                     {
                        type = Tile::TYPE_SIDE_SIDE;
                     }
                  }
                  else if( (haveNeighbor(tiles[i][j], 1, 0, absX, absZ)) &&
                           (haveNeighbor(tiles[i][j], 0, -1, absX, absZ)) )
                  {
                     angle = -90.0f;
                     if(!haveNeighbor(tiles[i][j], 1, -1, absX, absZ))
                     {
                        type = Tile::TYPE_SIDE_SIDE;
                     }
                  }
                  else
                  {
                     angle = 0.0f;
                     if(!haveNeighbor(tiles[i][j], -1, -1, absX, absZ))
                     {
                        type = Tile::TYPE_SIDE_SIDE;
                     }
                  }
               }
            }
            else if(numSideNeighbors == 3)
            {
               /* A middle! */
               type = Tile::TYPE_CENTER;
               
               /* Let's set its angle (and special cases for center_sides) */
               if(!haveNeighbor(tiles[i][j], -1, 0, absX, absZ))
               {
                  angle = 180.0f;
                  if(!haveNeighbor(tiles[i][j], 1, -1, absX, absZ))
                  {
                     if(!haveNeighbor(tiles[i][j], 1, 1, absX, absZ))
                     {
                        type = Tile::TYPE_CENTER_SIDE_2;
                     }
                     else
                     {
                        type = Tile::TYPE_CENTER_SIDE_1_2;
                     }
                  }
                  else if(!haveNeighbor(tiles[i][j], 1, 1, absX, absZ))
                  {
                     type = Tile::TYPE_CENTER_SIDE_1;
                  }
               }
               else if(!haveNeighbor(tiles[i][j], 0, -1, absX, absZ))
               {
                  angle = 90.0f;
                  if(!haveNeighbor(tiles[i][j], 1, 1, absX, absZ))
                  {
                     if(!haveNeighbor(tiles[i][j], -1, 1, absX, absZ))
                     {
                        type = Tile::TYPE_CENTER_SIDE_2;
                     }
                     else
                     {
                        type = Tile::TYPE_CENTER_SIDE_1_2;
                     }
                  }
                  else if(!haveNeighbor(tiles[i][j], -1, 1, absX, absZ))
                  {
                     type = Tile::TYPE_CENTER_SIDE_1;
                  }
               }
               else if(!haveNeighbor(tiles[i][j], 0, 1, absX, absZ))
               {
                  angle = -90.0f;
                  if(!haveNeighbor(tiles[i][j], -1, -1, absX, absZ))
                  {
                     if(!haveNeighbor(tiles[i][j], 1, -1, absX, absZ))
                     {
                        type = Tile::TYPE_CENTER_SIDE_2;
                     }
                     else
                     {
                        type = Tile::TYPE_CENTER_SIDE_1_2;
                     }
                  }
                  else if(!haveNeighbor(tiles[i][j], 1, -1, absX, absZ))
                  {
                     type = Tile::TYPE_CENTER_SIDE_1;
                  }
               }
               else
               {
                  angle = 0.0f;
                  if(!haveNeighbor(tiles[i][j], -1, 1, absX, absZ))
                  {
                     if(!haveNeighbor(tiles[i][j], -1, -1, absX, absZ))
                     {
                        type = Tile::TYPE_CENTER_SIDE_2;
                     }
                     else
                     {
                        type = Tile::TYPE_CENTER_SIDE_1_2;
                     }
                  }
                  else if(!haveNeighbor(tiles[i][j], -1, -1, absX, absZ))
                  {
                     type = Tile::TYPE_CENTER_SIDE_1;
                  }
               }
            }
            else if(numSideNeighbors == 4)
            {
               /* All side neighbors, Must be a top. verify each top type */
               if(numDiagonalNeighbors == 0)
               {
                  /* Top with all diagonals complements */
                  type = Tile::TYPE_TOP_SIDE_4;
                  angle = 0.0f;
               }
               else if(numDiagonalNeighbors == 1)
               {
                  /* Top with 3 diagonals complements */
                  type = Tile::TYPE_TOP_SIDE_3;

                  if(haveNeighbor(tiles[i][j], -1, -1, absX, absZ))
                  {
                     angle = 90.0f;
                  }
                  else if(haveNeighbor(tiles[i][j], -1, 1, absX, absZ))
                  {
                     angle = 180.0f;
                  }
                  else if(haveNeighbor(tiles[i][j], 1, -1, absX, absZ))
                  {
                     angle = 0.0f;
                  }
                  else
                  {
                     angle = -90.0f;
                  }
               }
               else if(numDiagonalNeighbors == 2)
               {
                  /* Top with 2 diagonals complements */
                  if( (!haveNeighbor(tiles[i][j], -1, -1, absX, absZ)) &&
                      (!haveNeighbor(tiles[i][j], 1, 1, absX, absZ)) )
                  {
                     type = Tile::TYPE_TOP_SIDE_2;
                     angle = 0.0f;
                  }
                  else if( (!haveNeighbor(tiles[i][j], -1, 1, absX, absZ)) &&
                           (!haveNeighbor(tiles[i][j], 1, -1, absX, absZ)) )
                  {
                     type = Tile::TYPE_TOP_SIDE_2;
                     angle = 90.0f;
                  }
                  else
                  {
                     /* Top, with 2 sided diagonal complements */
                     type = Tile::TYPE_TOP_SIDE_2_2;
                     if( (!haveNeighbor(tiles[i][j], -1, 1, absX, absZ)) &&
                         (!haveNeighbor(tiles[i][j], -1, -1, absX, absZ)) )
                     {
                        angle = 180.0f;
                     }
                     else if( (!haveNeighbor(tiles[i][j], 1, -1, absX, absZ)) &&
                         (!haveNeighbor(tiles[i][j], -1, -1, absX, absZ)) )
                     {
                        angle = 90.0f;
                     }
                     else if( (!haveNeighbor(tiles[i][j], -1, 1, absX, absZ)) &&
                         (!haveNeighbor(tiles[i][j], 1, 1, absX, absZ)) )
                     {
                        angle = -90.0f;
                     }
                     else
                     {
                        angle = 0.0f;
                     }
                  }
               }
               else if(numDiagonalNeighbors == 3)
               {
                  /* Top with 1 diagonal complement */
                  type = Tile::TYPE_TOP_SIDE_1;

                  /* Set angles */
                  angle = 0.0f;
                  if(!haveNeighbor(tiles[i][j], -1, -1, absX, absZ))
                  {
                     angle = 180.0f;
                  }
                  else if(!haveNeighbor(tiles[i][j], -1, 1, absX, absZ))
                  {
                     angle = -90.0f;
                  }
                  else if(!haveNeighbor(tiles[i][j], 1, -1, absX, absZ))
                  {
                     angle = 90.0f;
                  }
               }
               else if(numDiagonalNeighbors == 4)
               {
                  /* Top without complements */
                  type = Tile::TYPE_TOP;
               }
            }

            /* Let's change the model, if needed */
            tiles[i][j]->changeModel(type, getModel(type));
            tiles[i][j]->scNode->setAngle(0.0f, angle, 0.0f);
            tiles[i][j]->scNode->setPosition(iX+i*TILE_SIZE, 0.0f, 
                                             iZ+j*TILE_SIZE);

         }
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
 *                       numTileDiagonalNeighbors                      *
 ***********************************************************************/
int TileWall::numTileDiagonalNeighbors(Tile* t, int absX, int absZ)
{
   int total=0;
   int di, dj;
   
   /* Verify only diagonal neighbors */
   for(di = -1; di < 2; di += 2)
   {
      for(dj = -1; dj < 2; dj += 2)
      if(haveNeighbor(t, di, dj, absX, absZ))
      {
         total++;
      }
   }

   return(total);
}


/***********************************************************************
 *                         numTileSideNeighbors                        *
 ***********************************************************************/
int TileWall::numTileSideNeighbors(Tile* t, int absX, int absZ)
{
   int total=0;
   int d;
   
   /* Verify di -1 and + 1 */
   for(d=-1; d < 2; d += 2)
   {
      if(haveNeighbor(t, d, 0, absX, absZ))
      {
         total++;
      }
   }
   
   /* Verify dj -1 and + 1 */
   for(d=-1; d < 2; d += 2)
   {
      if(haveNeighbor(t, 0, d, absX, absZ))
      {
         total++;
      }
   }

   return(total);
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
   /* Get tile under mouse */
   curTile = getTileUnder(mouseX, mouseY, mouseZ);

   /* Verify if tried to delete it! */
   if( (mButton & SDL_BUTTON(1)) && (curTile) )
   {
      /* Wait until release mouse button */
      while(mButton & SDL_BUTTON(1))
      {
         SDL_PumpEvents();
         int x,y;
         mButton = SDL_GetMouseState(&x,&y);
      }

      /* Delete its scene node */
      curTile->deleteSceneNode();

      /* Reset tiles */
      setTileModels();
   }
}

/***********************************************************************
 *                            changeTile                               *
 ***********************************************************************/
void TileWall::changeTile(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
      Uint8 mButton)
{
   int nextType;

   /* Get tile under mouse */
   curTile = getTileUnder(mouseX, mouseY, mouseZ);

   /* Verify if tried to change it! */
   if( (mButton & SDL_BUTTON(1)) && (curTile) )
   {
      /* Wait mouse release */
      while(mButton & SDL_BUTTON(1))
      {
         SDL_PumpEvents();
         int x,y;
         mButton = SDL_GetMouseState(&x,&y);
      }
      nextType = ( (curTile->type + 1) % Tile::TOTAL_TILES);
      curTile->changeModel(nextType, getModel(nextType));
   }

   /* Right button to change orientation */
   if( (mButton & SDL_BUTTON(3)) && (curTile) )
   {
      /* Wait mouse release */
      while(mButton & SDL_BUTTON(3))
      {
         SDL_PumpEvents();
         int x,y;
         mButton = SDL_GetMouseState(&x,&y);
      }
      curTile->scNode->setAngle(0.0f, curTile->scNode->getAngleY()+90, 0.0f);
   }
}

/***********************************************************************
 *                           verifyactions                             *
 ***********************************************************************/
void TileWall::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ,
      GLfloat floorX, GLfloat floorZ,
      Uint8 mButton, Uint8* keys, int tool)
{
   /* Set floor by tiles */
   floorX = ((int)(floorX / TILE_SIZE) ) * TILE_SIZE;
   floorZ = ((int)(floorZ / TILE_SIZE) ) * TILE_SIZE;

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
   else if(tool == TOOL_TILE_WALL_CHANGE)
   {
      changeTile(mouseX, mouseY, mouseZ, mButton);
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


