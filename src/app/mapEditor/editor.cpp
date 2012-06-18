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

#include "editor.h"
#include "../../etc/extensions.h"
#include "../../etc/dirs.h"
#include "../../etc/npcfile.h"
#include "../../engine/options.h"

using namespace std;
using namespace dntMapEditor;

/*********************************************************************
 *                            Constructor                             *
 *********************************************************************/
Editor::Editor()
{
   int i;
   dirs dir;
   mapOpened = false;
   map = NULL;
   NPCs = NULL;
   particleSystem = new partController();
   particleSystem->init();
   skillsDefinitions skillList;
   skillList.init();
   features = new featsList();
   features->init(dir.getRealFile("feats/"),
                  dir.getRealFile("feats/feats.ftl"));

   /* Set Farso Options */
   Farso::Options farOpt;
   farOpt.setNeededDefaults(dir.getRealFile(DNT_FONT_ARIAL),
         dir.getRealFile("cursors/Walk.png"),
         dir.getRealFile("icons/maximize.png"),
         dir.getRealFile("icons/close.png"),
         gettext("Maximize"), gettext("Close"), gettext("Confirm"),
         gettext("Cancel"));

   /* Init things */
   options opt;
   Farso::init(&screen,"DNT Map Editor", opt.getScreenWidth(),
              opt.getScreenHeight(),  opt.getEnableFullScreen(),
              opt.getAntiAliasing(), opt.getStencilBufferSize());

   init();

   /* Set default colors */
   Farso::Colors farCor;
   farCor.setDefaultColor(Farso::Colors::COLOUR_WINDOW, 152, 148, 140, 220);
   farCor.setDefaultColor(Farso::Colors::COLOUR_MENU, 152, 148, 140, 220);
   farCor.setDefaultColor(Farso::Colors::COLOUR_BUTTON, 152, 148, 140, 220);
   farCor.setDefaultColor(Farso::Colors::COLOUR_BAR, 15, 80, 220, 220);
   farCor.setDefaultColor(Farso::Colors::COLOUR_HIGH, 15, 80, 190, 255);
   farCor.setDefaultColor(Farso::Colors::COLOUR_TEXT, 0, 0, 0, 255);
   farCor.setDefaultColor(Farso::Colors::COLOUR_DIRECTORY, 0, 0, 0, 255);

   farCor.setDefaultColor(Farso::Colors::COLOUR_CONT_0, 169, 169, 169, 255);
   farCor.setDefaultColor(Farso::Colors::COLOUR_CONT_1, 40, 40, 40, 255);
   farCor.setDefaultColor(Farso::Colors::COLOUR_CONT_2, 200, 200, 200, 255);

   /* Alloc the visible Matrix */
   visibleMatrix = new GLfloat*[6];
   for(i = 0; i < 6; i++)
   {
      visibleMatrix[i] = new GLfloat[4];
   }

   /* Load Extensions */
   extensions ext;
   ext.defineAllExtensions();

   gui = new dntMapEditor::GuiIO();
   hour = 12.0;
   gameSun = new sun(hour , OUTDOOR_FARVIEW, OUTDOOR_FARVIEW);
   gameSky = new sky();
   curScene.init();
   wTypes.init();
   curs.init();

   /* Initialize all shaders */
   glslShaders.init();

   aligns al;
   al.init();
   classes cl;
   cl.init();
   races rc;
   rc.init();

   terrainEditor = NULL;
   portalEditor = NULL;
   wallEditor = NULL;
   tileWall = NULL;
   particleEditor = NULL;
}

/*********************************************************************
 *                            Destructor                             *
 *********************************************************************/
Editor::~Editor()
{
   int i;
   if(mapOpened)
   {
      delete(map);
      delete(terrainEditor);
      delete(portalEditor);
      delete(wallEditor);
      delete(tileWall);
      delete(particleEditor);
      delete(npcController);
   }

   objectsList::removeAll();
   glslShaders.finish();

   delete(gameSun);
   delete(gameSky);
   if(particleSystem != NULL)
   {
      particleSystem->finish();
      delete(particleSystem);
   }
   if(NPCs != NULL)
   {
      delete(NPCs);
   }
   if(features)
   {
      features->finish();
      delete(features);
   }
   skillsDefinitions skillList;
   skillList.finish();
   aligns al;
   al.finish();
   classes cl;
   cl.finish();
   races rc;
   rc.finish();
   delete(gui);
   curScene.finish();
   wTypes.finish();
   Farso::end(screen);

   /* Clear the visibleMatrix */
   for(i = 0; i < 6; i++)
   {
      delete[] visibleMatrix[i];
   }
   delete[] visibleMatrix;

}

/*********************************************************************
 *                  redmensionate Window to Screen                   *
 *********************************************************************/
void Editor::redmensionateWindow()
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei)screen->w / (GLsizei)screen->h, 1.0,
                  OUTDOOR_FARVIEW);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/*********************************************************************
 *                            Init Function                          *
 *********************************************************************/
void Editor::init()
{
   redmensionateWindow();

   /* Clear */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);

   /* Details Definition */
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Light Definition */
   GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

   glEnable(GL_LIGHTING);
}

/*********************************************************************
 *                              Close Map                            *
 *********************************************************************/
void Editor::closeMap()
{
   if(mapOpened)
   {
      gui->showMessage("Closing actual Map...");

      /* Remove temporary state objects */
      objectEditor->deleteObject();
      npcController->deleteNpc();
      portalEditor->deleteDoor();
      gui->clearState();

      /* Delete things */
      if(NPCs)
      {
         delete(NPCs);
         NPCs = NULL;
      }
      delete(map);
      delete(terrainEditor);
      delete(portalEditor);
      delete(wallEditor);
      delete(tileWall);
      delete(objectEditor);
      delete(particleEditor);
      delete(npcController);
      objectsList::removeAll();
      curScene.finish();;
      curScene.init();
      mapOpened = false;
      map = NULL;
      particleSystem->deleteAll();
      particleSystem->setActualMap(map, NULL);
   }
}

/*********************************************************************
 *                               Open Map                            *
 *********************************************************************/
void Editor::openMap()
{
   if(mapOpened)
   {
      //Close Actual Map
      closeMap();
   }
   gui->showMessage("Opening actual Map...");
   draw();
   map = new Map();
   if(map->open(gui->getFileName()))
   {
      mapOpened = true;
      terrainEditor = new dntMapEditor::Terrain(map);
      portalEditor = new dntMapEditor::Portal(map, gui->getGui());
      gui->setPortalEditor(portalEditor);
      wallEditor = new dntMapEditor::WallController(map);
      tileWall = new dntMapEditor::TileWall(map);
      objectEditor = new dntMapEditor::Objects(map);
      particleEditor = new dntMapEditor::Particles(map);
      curTexture = ((mapTexture*)map->textures.getFirst())->index;
      curTextureName = ((mapTexture*)map->textures.getFirst())->name;

      /* Open NPCs */
      if(NPCs)
      {
         delete(NPCs);
      }
      NPCs = new characterList();
      npcController = new dntMapEditor::Npcs(map, NPCs, features);
      character* per;

      /* Load the NPCs */
      if(!map->getNpcFileName().empty())
      {
         npcFile* arq = new npcFile();

         if(!arq->load(map->getNpcFileName()))
         {
            gui->showMessage("Ouch, can't load NPC's file");
         }
         else
         {
            string name="", arquivo="";
            GLfloat posX=0, posZ=0, ori=0;
            int psycho=0;
            while(arq->getNextCharacter(name, arquivo, posX, posZ, ori, psycho))
            {
                per = NPCs->insertCharacter(arquivo, features, NULL, "");
                per->scNode->set(posX, map->getHeight(posX, posZ), posZ,
                      0.0f, ori, 0.0f);
                per->setPsychoState(psycho);
            }
         }
         delete(arq);
      }

      gui->getNodeEditor()->setMap(map, NPCs);
      /* Open FOG */
      map->fog.apply(map->isOutdoor(), OUTDOOR_FARVIEW, INDOOR_FARVIEW);

      gui->setFog(&map->fog);
      /* Open Particles */
      if(!map->getParticlesFileName().empty())
      {
          particleSystem->loadFromFile(map->getParticlesFileName());
          particleSystem->setActualMap(map, NULL);
          particleSystem->stabilizeAll();
      }
      else
      {
         particleSystem->deleteAll();
         particleSystem->setActualMap(map, NULL);
      }
      /* Real Size when outDoor */
      if(map->isOutdoor())
      {
         sizeX = map->getSizeX() - 14;
         sizeZ = map->getSizeZ() - 14;
      }
      else
      {
         sizeX = map->getSizeX();
         sizeZ = map->getSizeZ();
      }

      float x=0,z=0,a=0;
      map->getInitialPosition(x,z,a);
      gui->gameCamera.updateCamera(x,0,z,0);

      /* Set tile Editor with tiles, if any. */
      tileWall->loadTilesFromMap();

      gui->showMessage("Map opened.");
   }
   else
   {
      gui->showMessage("Failed To open map!\n");
   }
}


/*********************************************************************
 *                               Save Map                            *
 *********************************************************************/
void Editor::saveMap()
{
   string tmp;
   if(mapOpened)
   {
      /* Flush changes on nodes */
      gui->getNodeEditor()->flush();

      /* Flush changes on tiles */
      tileWall->flush();

      /* Remove the models that aren't used */
      curScene.removeUnusedModels();

      /* Save the particles File */
      if(particleSystem->numParticles() > 0)
      {
         map->setParticlesFileName(gui->getFileName()+".par");
         particleSystem->saveToFile(map->getParticlesFileName());
      }
      else
      {
         map->setParticlesFileName("");
      }

      /* Save the NPCs file */
      if(NPCs->getTotal() > 0)
      {
         map->setNpcFileName(gui->getFileName()+".npc");
         npcController->saveFile(map->getNpcFileName());
      }
      else
      {
         map->setNpcFileName("");
      }

      /* Save the Fog File */
      if(map->fog.enabled)
      {
         if(map->fog.fileName.empty())
         {
            map->fog.fileName = (gui->getFileName()+".fog");
         }
         /* Save the fog file */
         map->fog.save();
      }

      /* Save the Map File */
      map->save(gui->getFileName());
      tmp = "Map Saved as:";
      tmp += gui->getFileName();
      gui->showMessage(tmp);

      /* To make sure the selected texture isn't one that was removed */
      nextTexture();
      previousTexture();

      /* Get back tiles to tile Editor.*/
      tileWall->loadTilesFromMap();
   }
   else
   {
      gui->showMessage("Map Not Opened! Only can save opened maps!\n");
   }
}

/*********************************************************************
 *                               New Map                             *
 *********************************************************************/
void Editor::newMap()
{
   if(mapOpened)
   {
      closeMap();
   }

   mapOpened = true;
   map = new Map();

   sizeX = -1, sizeZ = -1;
   string s;
   int type = -1;
   int index = 0;
   int opt=-1;

   /* Get map type */
   while( type == -1 )
   {
      type = getOptionFromUser("Map Type", "Select the map type:",
                               "Indoor", "Outdoor", proj, modl, viewPort);
   }

   if(type == 1)
   {
      /* Select if empty or basic */
      opt = getOptionFromUser("Map Preset", "Select Map preset:",
            "Empty", "Tiles", proj, modl, viewPort);
      if(opt == 2)
      {
         opt = getOptionFromUser("Tile Type", "Select Tile to fill map:",
            "Cave", "Temple", proj, modl, viewPort);
         if(opt != -1)
         {
            type = 2+opt;
         }
      }
   }

   /* Set the type */
   map->setOutdoor(type == 2);

   /* Get map X size */
   while( (sizeX <= 0) || (sizeX > 30))
   {
      s = getStringFromUser("Map X Size", "8", proj, modl, viewPort);
      sscanf(s.c_str(),"%d", &sizeX);
      if( (sizeX <= 0) || (sizeX > 30) )
      {
         Farso::Warning warn;
         warn.show("Error!", "Size must be in range (0,30]", gui->getGui());
      }
   }

   /* Get map Z size */
   while( (sizeZ <= 0) || (sizeZ > 30))
   {
      s = getStringFromUser("Map Z Size", "8", proj, modl, viewPort);
      sscanf(s.c_str(),"%d", &sizeZ);
      if( (sizeZ <= 0) || (sizeZ > 30) )
      {
         Farso::Warning warn;
         warn.show("Error!", "Size must be in range (0,30]", gui->getGui());
      }
   }

   /* If is outdoor, the size is bigger, and can't walk to the created
    * squares to the horizon! */
   if(map->isOutdoor())
   {
      map->newMap(sizeX+14, sizeZ+14);
      index = ((mapTexture*)map->textures.getFirst())->index;
      int k, l;
      Square* saux;
      for(k = 0; k < sizeX+14; k++)
      {
         for(l=0; l < sizeZ+14; l++)
         {
            saux = map->relativeSquare(k,l);
            if( (k < 7) || (l < 7) || (k >= sizeX+7) || (l >= sizeZ+7) )
            {
               saux->flags = 0;
            }
         }
      }
      map->setInitialPosition( ((sizeX+12)*map->squareSize() / 2.0),
                               ((sizeZ+12)*map->squareSize() / 2.0));
      gui->gameCamera.updateCamera(((sizeX+14)*map->squareSize() / 2.0), 0.0,
                                   ((sizeZ+14)*map->squareSize() / 2.0), 0.0);
   }
   else
   {
      if(type == 1)
      {
         map->newMap(sizeX,sizeZ);
         index = ((mapTexture*)map->textures.getFirst())->index;
         /* Insert walls */
         wall* actualWall = map->addWall(0,0,(sizeX)*map->squareSize(),10);
         actualWall->frontTexture.setTextureId(index);
         actualWall->backTexture.setTextureId(index);
         actualWall->leftTexture.setTextureId(index);
         actualWall->rightTexture.setTextureId(index);

         actualWall = map->addWall(0,0,10,(sizeZ)*map->squareSize());
         actualWall->frontTexture.setTextureId(index);
         actualWall->backTexture.setTextureId(index);
         actualWall->leftTexture.setTextureId(index);
         actualWall->rightTexture.setTextureId(index);

         actualWall = map->addWall((sizeX)*map->squareSize()-10,0,
               (sizeX)*map->squareSize(),
               (sizeZ)*map->squareSize());
         actualWall->frontTexture.setTextureId(index);
         actualWall->backTexture.setTextureId(index);
         actualWall->leftTexture.setTextureId(index);
         actualWall->rightTexture.setTextureId(index);

         actualWall = map->addWall(0,(sizeZ)*map->squareSize()-10,
               (sizeX)*map->squareSize(),
               (sizeZ)*map->squareSize());
         actualWall->frontTexture.setTextureId(index);
         actualWall->backTexture.setTextureId(index);
         actualWall->leftTexture.setTextureId(index);
         actualWall->rightTexture.setTextureId(index);
      }
      else if(type == 3)
      {
         /* Cave! */
         map->newMap(sizeX,sizeZ, "texturas/floor_cave/burnt_sand_light.png");
      }
      else if(type == 4)
      {
         /* Temple! */
         map->newMap(sizeX, sizeZ);
      }

      /* Define Position */
      index = ((mapTexture*)map->textures.getFirst())->index;
      map->setInitialPosition( ((sizeX)*map->squareSize() / 2.0),
                            ((sizeZ)*map->squareSize() / 2.0));
   }

   /* Create Editor Controller */
   terrainEditor = new dntMapEditor::Terrain(map);
   portalEditor = new dntMapEditor::Portal(map, gui->getGui());
   gui->setPortalEditor(portalEditor);
   wallEditor = new dntMapEditor::WallController(map);
   tileWall = new dntMapEditor::TileWall(map);
   objectEditor = new dntMapEditor::Objects(map);
   particleEditor = new dntMapEditor::Particles(map);
   curTexture = index;
   curTextureName = ((mapTexture*)map->textures.getFirst())->name;
   NPCs = new characterList();
   npcController = new dntMapEditor::Npcs(map, NPCs, features);
   particleSystem->setActualMap(map, NULL);
   gui->getNodeEditor()->setMap(map, NPCs);

   /* Add the tileset for map, if needed */
   if(type == 3)
   {
      tileWall->fillMapWithTiles("cave");
   }
   else if(type == 4)
   {
      tileWall->fillMapWithTiles("temple");
   }


   gui->showMessage("Created New Game Map!");
   map->fog.apply(map->isOutdoor(), OUTDOOR_FARVIEW, INDOOR_FARVIEW);
   gui->setFog(&map->fog);
}

/*********************************************************************
 *                           Verify Position                         *
 *********************************************************************/
void Editor::verifyPosition()
{
   if(mapOpened)
   {
       if(gui->gameCamera.getCenterX() >
          ((map->getSizeX() * map->squareSize())+20))
       {
          gui->gameCamera.updateCamera( ((map->getSizeX() *
                                             map->squareSize())+20),
                                          gui->gameCamera.getCenterY()-30,
                                          gui->gameCamera.getCenterZ(),
                                          0.0);
       }
       else if(gui->gameCamera.getCenterX() < -20)
       {
          gui->gameCamera.updateCamera( -20 ,
                                          gui->gameCamera.getCenterY()-30,
                                          gui->gameCamera.getCenterZ(),
                                          0.0);
       }
       if(gui->gameCamera.getCenterZ() > ((map->getSizeZ() *
                                           map->squareSize())+20))
       {
          gui->gameCamera.updateCamera( gui->gameCamera.getCenterX(),
                                           gui->gameCamera.getCenterY()-30,
                                          ((map->getSizeZ() *
                                            map->squareSize())+20),
                                          0.0);
       }
       else if(gui->gameCamera.getCenterZ() < -20)
       {
          gui->gameCamera.updateCamera( gui->gameCamera.getCenterX(),
                                          gui->gameCamera.getCenterY()-30,
                                          -20,
                                          0.0);

       }
   }
}

/************************************************************************
 *                         Previous Texture                             *
 ************************************************************************/
int Editor::previousTexture()
{
   int aux=0;
   mapTexture* tex = (mapTexture*)map->textures.getFirst();
   while(aux < map->textures.getTotal())
   {
      if(tex->index == curTexture)
      {
         curTexture = ((mapTexture*)tex->getPrevious())->index;
         curTextureName = ((mapTexture*)tex->getPrevious())->name;
         return(((mapTexture*)tex->getPrevious())->index);
      }
      tex = (mapTexture*)tex->getNext();
      aux++;
   }
   curTexture = ((mapTexture*)map->textures.getFirst())->index;
   curTextureName = ((mapTexture*)map->textures.getFirst())->name;
   return(((mapTexture*)map->textures.getFirst())->index);
}

/************************************************************************
 *                             Next Texture                             *
 ************************************************************************/
int Editor::nextTexture()
{
   int aux=0;
   mapTexture* tex = (mapTexture*)map->textures.getFirst();
   while(aux < map->textures.getTotal())
   {
      if(tex->index == curTexture)
      {
         curTexture = ((mapTexture*)tex->getNext())->index;
         curTextureName = ((mapTexture*)tex->getNext())->name;
         return(((mapTexture*)tex->getNext())->index);
      }
      tex = (mapTexture*)tex->getNext();
      aux++;
   }
   curTexture = ((mapTexture*)map->textures.getFirst())->index;
   curTextureName = ((mapTexture*)map->textures.getFirst())->name;
   return(((mapTexture*)map->textures.getFirst())->index);
}

/************************************************************************
 *                            insertTexture                             *
 ************************************************************************/
int Editor::insertTexture(string textureFile)
{
   dirs dir;
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile(textureFile).c_str());
   if(!img)
   {
      gui->showMessage("Error opening texture!");
      return(0);
   }

   SDL_FreeSurface(img);

   return(map->insertTexture(gui->getTextureFileName(),
                             gui->getTextureFileName(), true));


}

/********************************************************************
 *                        RenderSceneryObjects                      *
 ********************************************************************/
void Editor::renderSceneryObjects()
{
   curScene.render(visibleMatrix, false, false, false, NULL, 0.0);
}

/*********************************************************************
 *                                Draw                               *
 *********************************************************************/
void Editor::draw()
{
   options opt;
   GLenum errorCode;
   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   glLoadIdentity();
   glClearColor(0.0,0.0,0.0,1.0);

   /* Invert multitexture if desired */
   if(gui->getInvertMultiTexture())
   {
      if(opt.getSplattingType() == DNT_SPLATTING_NONE)
      {
         opt.setSplattingType(DNT_SPLATTING_SHADER);
      }
      else
      {
         opt.setSplattingType(DNT_SPLATTING_NONE);
      }
   }

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   /* Redefine camera position */
   gui->cameraPos(map);
   updateFrustum( visibleMatrix, proj, modl);
   gameSun->updateHourOfDay(hour, 0.0, 0.0);
   gameSun->setLight();

   /* FIXME: when lights enabled in map editor, must ignore the following
    * light disable. */
   /* Disable GL_LIGHT1 for bump mapping correct usage without 
    * enabled map lights */
   glDisable(GL_LIGHT1);
   GLfloat p[4]={0.0f, 0.0f, 0.0f, 2.0f};
   glPushMatrix();
   glLightfv(GL_LIGHT1, GL_POSITION, p);
   glPopMatrix();

   /* Draw Things */
   if(mapOpened)
   {
      /* SKY */

      if(map->isOutdoor())
      {
         /* Sky */
         glPushMatrix();
            glTranslatef(gui->gameCamera.getCameraX(), 0.0,
                         gui->gameCamera.getCameraZ());
            gameSky->draw(map,gameSun->getRotation(),!map->getFog().enabled);
         glPopMatrix();

         /* Real Map Limits */
         glPushMatrix();
            GLfloat ambient[] = { 0.94, 0.192, 0.12, 0.45 };
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ambient);
            glColor4fv(ambient);
            glBegin(GL_QUADS);
               glNormal3f(0,1,0);
               glVertex3f(7*map->squareSize()-5, 0.5, 7*map->squareSize());
               glVertex3f(7*map->squareSize()-5, 0.5,
                          (sizeZ+7)*map->squareSize());
               glVertex3f(7*map->squareSize()+5, 0.5,
                          (sizeZ+7)*map->squareSize());
               glVertex3f(7*map->squareSize()+5, 0.5, 7*map->squareSize());

               glNormal3f(0,1,0);
               glVertex3f((sizeX+7)*map->squareSize()-5, 0.5,
                          7*map->squareSize());
               glVertex3f((sizeX+7)*map->squareSize()-5, 0.5,
                          (sizeZ+7)*map->squareSize());
               glVertex3f((sizeX+7)*map->squareSize()+5, 0.5,
                          (sizeZ+7)*map->squareSize());
               glVertex3f((sizeX+7)*map->squareSize()+5, 0.5,
                          7*map->squareSize());

               glNormal3f(0,1,0);
               glVertex3f(7*map->squareSize(), 0.5, 7*map->squareSize()-5);
               glVertex3f((sizeX+7)*map->squareSize(), 0.5,
                          7*map->squareSize()-5);
               glVertex3f((sizeX+7)*map->squareSize(), 0.5,
                          7*map->squareSize()+5);
               glVertex3f(7*map->squareSize(), 0.5, 7*map->squareSize()+5);

               glNormal3f(0,1,0);
               glVertex3f(7*map->squareSize(), 0.5,
                          (sizeZ+7)*map->squareSize()-5);
               glVertex3f((sizeX+7)*map->squareSize(), 0.5,
                          (sizeZ+7)*map->squareSize()-5);
               glVertex3f((sizeX+7)*map->squareSize(), 0.5,
                          (sizeZ+7)*map->squareSize()+5);
               glVertex3f(7*map->squareSize(), 0.5,
                          (sizeZ+7)*map->squareSize()+5);

            glEnd();
         glPopMatrix();
      }
      glColor4f(1.0,1.0,1.0,1.0);

      /* Render Map */
      glPushMatrix();
         map->render(gui->gameCamera.getCameraX(),
                     gui->gameCamera.getCameraY(),
                     gui->gameCamera.getCameraZ(),
                     visibleMatrix,
                     gui->gameCamera.getCameraX(),
                     gui->gameCamera.getCameraZ());
      glPopMatrix();

      /* Render All Static Scenery Objects */
      renderSceneryObjects();

      glPushMatrix();
         gui->getNodeEditor()->drawTemporary();
         if(gui->getState() == GUI_IO_STATE_TERRAIN)
         {
            terrainEditor->drawTemporary();
         }
         else if(gui->getState() == GUI_IO_STATE_PORTAL)
         {
            portalEditor->drawTemporary();
         }
         else if(gui->getState() == GUI_IO_STATE_WALL)
         {
            wallEditor->drawTemporary(modl,gui->gameCamera.getCameraX(),
                                      gui->gameCamera.getCameraY(),
                                      gui->gameCamera.getCameraZ());
         }
         else if(gui->getState() == GUI_IO_STATE_TILE_WALL)
         {
            tileWall->drawTemporary();
         }
         else if(gui->getState() == GUI_IO_STATE_OBJECTS)
         {
            objectEditor->drawTemporary();
         }
         else if(gui->getState() == GUI_IO_STATE_PARTICLES)
         {
            particleEditor->drawTemporary(visibleMatrix);
            dntMapEditor::GrassWindow* gr = gui->getGrassWindow();
            if(gr)
            {
               //gr->drawTemporary();
            }
            dntMapEditor::WaterWindow* wt = gui->getWaterWindow();
            if(wt)
            {
               wt->drawTemporary();
            }
         }
         else if(gui->getState() == GUI_IO_STATE_NPCS)
         {
            npcController->drawTemporary();
         }
      glPopMatrix();
   }

   glColor4f(1.0,1.0,1.0,1.0);

   /* Draw Particles */
   glPushMatrix();
     particleSystem->updateAll(visibleMatrix, true);
   glPopMatrix();
   glDisable(GL_FOG);

   //gameSun->drawSun();

   glColor4f(1.0,1.0,1.0,1.0);

   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   glDisable(GL_DEPTH_TEST);
   /* Draw GUI */
   glPushMatrix();
      draw2DMode();
      gui->draw(proj, modl, viewPort);

      /* Cursor */
      glPushMatrix();
         curs.draw(mouseX, mouseY, 0);
      glPopMatrix();

      draw3DMode(OUTDOOR_FARVIEW);
   glPopMatrix();
   glEnable(GL_FOG);
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);

   glColor4f(1.0,1.0,1.0,1.0);

   /* Draw Active Texture */
   if(mapOpened)
   {
      gluUnProject(Farso::SCREEN_X,Farso::SCREEN_Y, 0.01, modl, proj, viewPort,
                   &x1, &y1, &z1);
      gluUnProject(Farso::SCREEN_X,Farso::SCREEN_Y-50,0.01, modl, proj, viewPort,
                   &x2, &y2, &z2);
      gluUnProject(Farso::SCREEN_X-50,Farso::SCREEN_Y-50,0.01,modl,proj,viewPort,
                   &x3, &y3, &z3);
      gluUnProject(Farso::SCREEN_X-50,Farso::SCREEN_Y,0.01, modl, proj, viewPort,
                   &x4, &y4, &z4);

      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, curTexture );
      glBegin(GL_QUADS);
         glColor3f(1,1,1);
         glTexCoord2f(1,0);
         glVertex3f(x1,y1,z1);
         glTexCoord2f(1,1);
         glVertex3f(x2,y2,z2);
         glTexCoord2f(0,1);
         glVertex3f(x3,y3,z3);
         glTexCoord2f(0,0);
         glVertex3f(x4,y4,z4);
      glEnd();
      glDisable(GL_TEXTURE_2D);

      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);


   }

   glEnable(GL_FOG);
   glFlush();

   while( (errorCode = glGetError()) != GL_NO_ERROR)
   {
      cerr << "OpenGL Error: " << gluErrorString(errorCode) << endl;
   }

   SDL_GL_SwapBuffers();

   /* Invert multitexture if desired */
   if(gui->getInvertMultiTexture())
   {
      if(opt.getSplattingType() == DNT_SPLATTING_NONE)
      {
         opt.setSplattingType(DNT_SPLATTING_SHADER);
      }
      else
      {
         opt.setSplattingType(DNT_SPLATTING_NONE);
      }
   }
}

/*********************************************************************
 *                         updateMouseFloorPos                       *
 *********************************************************************/
void Editor::updateMouseFloorPos()
{
   GLfloat wx = mouseX,
           wy = Farso::SCREEN_Y - mouseY;

   GLdouble cX=0, cY=0, cZ=0;
   GLdouble fX=0, fY=0, fZ=0;

   GLfloat t=0, vd=0;

   /* Get at camera */
   gluUnProject(wx, wy, 0, modl, proj, viewPort, &cX, &cY, &cZ);

   /* Get at far */
   gluUnProject(wx, wy, 0.9, modl, proj, viewPort, &fX, &fY, &fZ);

   /* Calculate */
   vd = fY - cY;
   if(vd == 0)
   {
      /* Vector Paralell to the floor */
      return;
   }
   t = (-cY / vd);
   xFloor = cX + t*(fX-cX);
   zFloor = cZ + t*(fZ-cZ);
}


/*********************************************************************
 *                              doEditorIO                           *
 *********************************************************************/
void Editor::doEditorIO()
{
   GLfloat wx, wy, wz;

   wx = mouseX; wy = Farso::SCREEN_Y-mouseY;

   glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
   gluUnProject( wx, wy, wz, modl, proj, viewPort, &xReal, &yReal, &zReal);

   updateMouseFloorPos();

   if( (gui->getState() == GUI_IO_STATE_TERRAIN) && (mapOpened))
   {
      terrainEditor->verifyAction(xReal, yReal, zReal, xFloor, zFloor,
            mButton, gui->getTool(), curTexture);
      particleSystem->setActualMap(map, NULL);
   }
   else if( (gui->getState() == GUI_IO_STATE_PORTAL) && (mapOpened))
   {
      string doorFile = gui->getSelectedText();
      if( (portalEditor->getDoor() == NULL) ||
          (doorFile != portalEditor->getDoorFileName())
        )
      {
         if(!portalEditor->getDoorFileName().empty())
         {
            portalEditor->deleteDoor();
         }
         if(!doorFile.empty())
         {
            object* obj = createObject(doorFile, map->getFileName());
            portalEditor->defineDoor((object*)obj, gui->getSelectedText());
         }
      }
      portalEditor->verifyAction(xReal, yReal, zReal, mButton, gui->getTool(),
                                 proj, modl, viewPort);
   }
   else if( (gui->getState() == GUI_IO_STATE_WALL) && (mapOpened))
   {
      int tl = gui->getTool();
      wallEditor->verifyAction(xReal, yReal, zReal, xFloor, zFloor,
            mButton, keys, tl, curTexture, curTextureName);
      gui->setTool(tl);
   }
   else if( (gui->getState() == GUI_IO_STATE_TILE_WALL) )
   {
      tileWall->verifyAction(xReal, yReal, zReal, xFloor, zFloor,
                  mButton, keys, gui->getTool());
   }
   else if( (gui->getState() == GUI_IO_STATE_OBJECTS) && (mapOpened))
   {
      /* Object Add */
      if(gui->getTool() == TOOL_OBSTACLE_ADD)
      {
         string objFile = gui->getSelectedText();
         if( (!objFile.empty()) &&
             (objFile != objectEditor->getObjectFileName()))
         {
            if(!objectEditor->getObjectFileName().empty())
            {
               objectEditor->deleteObject();
            }
            object* obj = createObject(objFile, map->getFileName());
            objectEditor->defineActualObject(obj, objFile);
         }
         /*sceneNode* sc=*/objectEditor->verifyAction(keys, xReal, yReal, zReal,
               mButton, mouseX, mouseY, gui->getTool(), proj, modl, viewPort);
         #if 0
         if(sc)
         {
            /* Set scene node Editor to the new edited object */
            gui->getNodeEditor()->selectNode(sc);
            gui->getNodeEditor()->openWindow();
         }
         #endif
      }
      /* Object Node Editor */
      else if(gui->getTool() == TOOL_NODE_EDITOR)
      {
         gui->getNodeEditor()->verifyAction(keys, xReal, yReal, zReal, mButton,
               mouseX, mouseY, proj, modl, viewPort);
      }
   }
   else if( (gui->getState() == GUI_IO_STATE_PARTICLES) && (mapOpened))
   {
      particleEditor->verifyAction(xReal, yReal, zReal, mButton, keys,
                                   gui, particleSystem, proj,
                                   modl, viewPort, gui->getSelectedText(),
                                   gui->getGrassWindow(),
                                   gui->getWaterWindow(),
                                   map);
   }
   else if( (gui->getState() == GUI_IO_STATE_NPCS) && (mapOpened))
   {
      string npcFile = gui->getSelectedText();
      if( (!npcFile.empty()) && (npcFile != npcController->getNpcFileName()))
      {
         if(!npcController->getNpcFileName().empty())
         {
            npcController->deleteNpc();
         }
         npcController->defineActualNpc(npcFile);
      }
      npcController->verifyAction(xReal, yReal, zReal, mButton, mouseX, mouseY,
                                  gui->getTool(), proj, modl, viewPort);
   }
   else if( (gui->getState() == GUI_IO_STATE_MUSIC) && (mapOpened))
   {
      string musicFile = gui->getSelectedText();
      if( (!musicFile.empty()) && (map->getMusicFileName() != musicFile))
      {
         char msg[1024];
         map->setMusicFileName(musicFile);
         sprintf(msg,"Defined Music as: %s", musicFile.c_str());
         gui->showMessage(msg);
      }
   }


   /* Delete temporary unused models */
   if(mapOpened)
   {
      if( (gui->getTool() != TOOL_OBSTACLE_ADD) &&
            (!objectEditor->getObjectFileName().empty()) )
      {
         objectEditor->deleteObject();
      }
      if( (gui->getState() != GUI_IO_STATE_NPCS) &&
          (!npcController->getNpcFileName().empty()) )
      {
         npcController->deleteNpc();
      }
      if( (gui->getTool() != TOOL_PORTAL_DOOR) &&
          (!portalEditor->getDoorFileName().empty()))
      {
         portalEditor->deleteDoor();
      }
   }

}

/*********************************************************************
 *                              verifyIO                             *
 *********************************************************************/
void Editor::verifyIO()
{
   int guiEvent;
   bool outdoor = false;

   SDL_PumpEvents();
   keys = SDL_GetKeyState(NULL);
   mButton = SDL_GetMouseState(&mouseX,&mouseY);

   /* Let's check some events */
   SDL_Event event;
   mouseWheel = 0;
   while(SDL_PollEvent(&event))
   {
      if(event.type == SDL_QUIT)
      {
      }
      else if(event.type == SDL_MOUSEBUTTONDOWN)
      {
         if(event.button.button == SDL_BUTTON_WHEELUP)
         {
            mouseWheel++;
         }
         else if(event.button.button == SDL_BUTTON_WHEELDOWN)
         {
            mouseWheel--;
         }
      }
   }

   if(mapOpened)
   {
      outdoor = map->isOutdoor();
   }

   gui->updateMouseWorldCoordinates(xReal, zReal);
   guiEvent = gui->doIO(mouseX, mouseY, mButton, keys, mouseWheel, outdoor);
   if(guiEvent == GUI_IO_EXIT)
   {
      quit = true;
   }
   else if(guiEvent == GUI_IO_NEW_MAP)
   {
      newMap();
   }
   else if(guiEvent == GUI_IO_OPEN_MAP)
   {
      openMap();
   }
   else if(guiEvent == GUI_IO_SAVE_MAP)
   {
      saveMap();
   }
   else if(guiEvent == GUI_IO_TEXTURE_PREVIOUS)
   {
      if(mapOpened)
      {
         previousTexture();
      }
   }
   else if(guiEvent == GUI_IO_TEXTURE_NEXT)
   {
      if(mapOpened)
      {
         nextTexture();
      }
   }
   else if(guiEvent == GUI_IO_TEXTURE_INSERT)
   {
      if(mapOpened)
      {
         if(insertTexture(gui->getTextureFileName()))
         {
            gui->showMessage(gettext("Texture inserted!"));
         }
         else
         {
            gui->showMessage(gettext("Can't insert texture!"));
         }
      }
   }
   else if(guiEvent == GUI_IO_NOTHING)
   {
      doEditorIO();
   }
   else if(guiEvent == GUI_IO_NEW_STATE)
   {
      gui->showMessage(messageForTool(gui->getTool()));
   }
}

/*********************************************************************
 *                                 Run                               *
 *********************************************************************/
void Editor::run()
{

   quit = false;
   int time = 0;

   while(!quit)
   {
      if(SDL_GetTicks() - time >= UPDATE_RATE)
      {
         time = SDL_GetTicks();

         verifyPosition();
         verifyIO();

         draw();
      }
      else
      {
         int t = SDL_GetTicks();
         if(UPDATE_RATE - (t - time) > 5)
            SDL_Delay(UPDATE_RATE - (t - time));
      }
   }

}

