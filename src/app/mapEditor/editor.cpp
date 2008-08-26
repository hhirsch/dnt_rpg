#include "editor.h"
#include "../../etc/extensions.h"
#include "../../etc/dirs.h"

/*********************************************************************
 *                            Constructor                             *
 *********************************************************************/
editor::editor()
{
   dirs dir;
   mapOpened = false;
   map = NULL;
   NPCs = NULL;
   particleSystem = new partController();
   features = new featsList(dir.getRealFile("feats/"),
                            dir.getRealFile("feats/feats.ftl"));

   //FIXME, verify if the antialising is disable or not!
   Farso_Init(&screen,"DccNiTghtmare Map Editor 0.2", 800, 600, false, 0);

   init();

   /* Load Extensions */
   extensions ext;
   ext.defineAllExtensions();

   gui = new(guiIO);
   hour = 12.0;
   gameSun = new sun(hour , OUTDOOR_FARVIEW, OUTDOOR_FARVIEW);
   gameSky = new(sky);
   models = new modelList();
   wTypes = new weaponTypes();
   objectsList = new lObject();

   terrainEditor = NULL;
   portalEditor = NULL;
   wallEditor = NULL;
   particleEditor = NULL;
}

/*********************************************************************
 *                            Destructor                             *
 *********************************************************************/
editor::~editor()
{
   if(mapOpened)
   {
      delete(map);
      delete(terrainEditor);
      delete(portalEditor);
      delete(wallEditor);
      delete(particleEditor);
      delete(npcController);
   }
   delete(objectsList);
   delete(gameSun);
   delete(gameSky);
   if(particleSystem != NULL)
   {
      delete(particleSystem);
   }
   if(NPCs != NULL)
   {
      delete(NPCs);
   }
   if(features)
   {
      delete(features);
   }
   delete(gui);
   delete(models);
   delete(wTypes);
   Farso_End(screen);
}

/*********************************************************************
 *                  redmensionate Window to Screen                   *
 *********************************************************************/
void editor::redmensionateWindow()
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
void editor::init()
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
void editor::closeMap()
{
   if(mapOpened)
   {
      gui->showMessage("Closing actual Map...");
      delete(map);
      delete(terrainEditor);
      delete(portalEditor);
      delete(wallEditor);
      delete(objectEditor);
      delete(particleEditor);
      delete(npcController);
      delete(objectsList);
      objectsList = new lObject();
      delete(models);
      models = new modelList();
      mapOpened = false;
   }
}

/*********************************************************************
 *                               Open Map                            *
 *********************************************************************/
void editor::openMap()
{
   dirs dir;
   if(mapOpened)
   {
      //Close Actual Map
      closeMap();
   }
   gui->showMessage("Opening actual Map...");
   draw();
   map = new Map(objectsList);
   if(map->open(gui->getFileName(),*models,*wTypes))
   {
      mapOpened = true;
      terrainEditor = new terrain(map);
      portalEditor = new portal(map);
      wallEditor = new wallController(map);
      objectEditor = new objects(map, models);
      particleEditor = new particles(map);
      actualTexture = map->textures->index;

      /* Open NPCs */
      if(NPCs)
         delete(NPCs);
      NPCs = new (characterList);
      npcController = new npcs(map, NPCs, features);
      character* per;
      if(!map->getNpcFileName().empty())
      {
         FILE* arq;
         if(!(arq = fopen(dir.getRealFile(map->getNpcFileName()).c_str(),"r")))
         {
            gui->showMessage("Ouch, can't load NPC's file");
         }
         else
         {
            int total;
            int npc;
            char name[30];
            char arquivo[255];
            float posX, posZ;
            fscanf(arq,"%d",&total);
            for(npc = 0; npc < total; npc++)
            {
                fscanf(arq,"%s %s %f %f",&name[0],&arquivo[0],
                       &posX,&posZ);
                per = NPCs->insertCharacter(arquivo,features, NULL, "");
                per->xPosition = posX;
                per->zPosition = posZ;
                per->update(0); 
                per->calculateBoundingBox();
            }
            fclose(arq);
         }  
      }
      /* Open FOG */
      if(map->fog.enabled)
      {
         glEnable(GL_FOG);
         {
            glFogi(GL_FOG_MODE,GL_LINEAR);
            glFogfv(GL_FOG_COLOR,map->fog.color);
            glFogf(GL_FOG_DENSITY,map->fog.density);
            glHint(GL_FOG_HINT,GL_DONT_CARE);
            glFogf(GL_FOG_START,map->fog.start);
            glFogf(GL_FOG_END,map->fog.end);
         }
      }
      else
      {
         glEnable(GL_FOG);
         {
            GLfloat color[3]={1.0,1.0,1.0};
            glFogi(GL_FOG_MODE,GL_LINEAR);
            glFogfv(GL_FOG_COLOR, color);
            glFogf(GL_FOG_DENSITY, 5.00);
            glHint(GL_FOG_HINT, GL_DONT_CARE);
            glFogf(GL_FOG_START, 200);
            glFogf(GL_FOG_END, (OUTDOOR_FARVIEW) + 4000);
         }
      }
      gui->setFog(&map->fog);
      /* Open Particles */
      if(!map->getParticlesFileName().empty())
      {
          particleSystem->loadFromFile(map->getParticlesFileName());
          particleSystem->stabilizeAll();
      }
      else
      {
         particleSystem->deleteAll();
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
void editor::saveMap()
{
   string tmp;
   if(mapOpened)
   {
      /* Remove the models that aren't used */
      models->removeUnusedModels();

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
   }
   else
   {
      gui->showMessage("Map Not Opened! Only can save opened maps!\n");
   }
}

/*********************************************************************
 *                               New Map                             *
 *********************************************************************/
void editor::newMap()
{
   if(mapOpened)
   {
      closeMap();
   }

   mapOpened = true;
   map = new Map(objectsList);

   sizeX = -1, sizeZ = -1;
   string s;
   int type = -1;

   /* Get map type */
   while( type == -1 )
   {
      type = getOptionFromUser("Map Type", "Select the map type:",
                               "Indoor", "Outdoor", proj, modl, viewPort);
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
         showMessage("Error!", "Size must be in range (0,30]", proj, modl,
                     viewPort);
      }
   }

   /* Get map Z size */
   while( (sizeZ <= 0) || (sizeZ > 30))
   {
      s = getStringFromUser("Map Z Size", "8", proj, modl, viewPort);
      sscanf(s.c_str(),"%d", &sizeZ);
      if( (sizeZ <= 0) || (sizeZ > 30) )
      {
         showMessage("Error!", "Size must be in range (0,30]", proj, modl, 
                     viewPort);
      }
   }

   /* If is outdoor, the size is bigger, and can't walk to the created
    * squares to the horizon! */
   if(map->isOutdoor())
   {
      map->newMap(sizeX+14, sizeZ+14);
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
      map->newMap(sizeX,sizeZ);
      /* Insert walls */
      wall* actualWall = map->addWall(0,0,(sizeX)*map->squareSize(),10);
      actualWall->dX = 16; actualWall->dY = 16; actualWall->dZ = 16;
      actualWall->frontTexture = map->textures->index;
      actualWall->backTexture = map->textures->index;

      actualWall = map->addWall(0,0,10,(sizeZ)*map->squareSize());
      actualWall->dX = 16; actualWall->dY = 16; actualWall->dZ = 16;
      actualWall->frontTexture = map->textures->index;
      actualWall->backTexture = map->textures->index;

      actualWall = map->addWall((sizeX)*map->squareSize()-10,0,
                                (sizeX)*map->squareSize(), 
                                (sizeZ)*map->squareSize());
      actualWall->dX = 16; actualWall->dY = 16; actualWall->dZ = 16;
      actualWall->frontTexture = map->textures->index;
      actualWall->backTexture = map->textures->index;
      
      actualWall = map->addWall(0,(sizeZ)*map->squareSize()-10,
                                (sizeX)*map->squareSize(),
                                (sizeZ)*map->squareSize());
      actualWall->dX = 16; actualWall->dY = 16; actualWall->dZ = 16;
      actualWall->frontTexture = map->textures->index;
      actualWall->backTexture = map->textures->index;

      /* Define Position */
      map->setInitialPosition( ((sizeX)*map->squareSize() / 2.0),
                            ((sizeZ)*map->squareSize() / 2.0));
   }

   /* Create Editor Controller */
   terrainEditor = new terrain(map);
   portalEditor = new portal(map);
   wallEditor = new wallController(map);
   objectEditor = new objects(map, models);
   particleEditor = new particles(map);
   actualTexture = map->textures->index;
   NPCs = new (characterList);
   npcController = new npcs(map, NPCs, features);
   gui->showMessage("Created New Game Map!");
   glEnable(GL_FOG);
   {
      GLfloat color[3]={0.8,0.8,0.8};
      glFogi(GL_FOG_MODE,GL_LINEAR);
      glFogfv(GL_FOG_COLOR, color);
      glFogf(GL_FOG_DENSITY, 0.0010);
      glHint(GL_FOG_HINT, GL_DONT_CARE);
      glFogf(GL_FOG_START, 200);
      glFogf(GL_FOG_END, (OUTDOOR_FARVIEW / 2.0)+200);
   }
   gui->setFog(&map->fog); 
}

/*********************************************************************
 *                           Verify Position                         *
 *********************************************************************/
void editor::verifyPosition()
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
int editor::previousTexture()
{
   int aux=0;
   texture* tex = map->textures;
   while(aux < map->numTextures)
   {
      if(tex->index == actualTexture)
      {
         actualTexture = tex->previous->index;
         return(tex->previous->index);
      }
      tex = tex->next;
      aux++;
   }
   actualTexture = map->textures->index;
   return(map->textures->index);
}

/************************************************************************
 *                             Next Texture                             *
 ************************************************************************/
int editor::nextTexture()
{
   int aux=0;
   texture* tex = map->textures;
   while(aux < map->numTextures)
   {
      if(tex->index == actualTexture)
      {
         actualTexture = tex->next->index;
         return(tex->next->index);
      }
      tex = tex->next;
      aux++;
   }
   actualTexture = map->textures->index;
   return(map->textures->index);
}

/************************************************************************
 *             Insere Textura na Lista de textures                      *
 ************************************************************************/
int editor::insertTexture(string textureFile)
{
   dirs dir;
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile(textureFile).c_str());
   if(!img)
   {
      gui->showMessage("Error opening texture!");
      return(0);
   }

   Uint8 R,G,B,tR,tG,tB;
   
   Uint32 pixel = pixel_Get(img,0,0);
   SDL_GetRGB(pixel,img->format, &R, &G, &B);
   tR = R;
   tG = G;
   tB = B;

   int x, y;
   /* Get the Medium Color Value of the texture */
   for(x=0;x < img->w; x++)
   {
      for(y=0; y < img->h; y++)
      {
         pixel = pixel_Get(img,x,y);
         SDL_GetRGB(pixel,img->format, &R, &G, &B);
         tR = (tR+R) / 2;
         tG = (tG+G) / 2;
         tB = (tB+B) / 2;
      }
   }

   SDL_FreeSurface(img);

   return(map->insertTexture(gui->getTextureFileName(),
                             gui->getTextureFileName(), tR, tG, tB));

   
}

/********************************************************************
 *                        RenderSceneryObjects                      *
 ********************************************************************/
void editor::renderSceneryObjects()
{
   model3d* mdl = models->getFirst();
   int i;
   for(i = 0; i< models->getTotalModels(); i++)
   {
      /* Only Render here the Static Scenery Objects */
      if(mdl->isStaticScenery())
      {
         mdl->draw(visibleMatrix, false);
      }
      mdl = mdl->next;
   }
}

/*********************************************************************
 *                                Draw                               *
 *********************************************************************/
void editor::draw()
{
   GLenum errorCode;
   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   glLoadIdentity();
   glClearColor(0.0,0.0,0.0,1.0);


   /* Redefine camera position */
   gui->cameraPos(map);
   updateFrustum( visibleMatrix, proj, modl);
   gameSun->updateHourOfDay(hour, 0.0, 0.0);
   gameSun->setLight();
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
            gameSky->draw(map,gameSun->getRotation());
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
         map->draw(gui->gameCamera.getCameraX(), gui->gameCamera.getCameraY(), 
                   gui->gameCamera.getCameraZ(), visibleMatrix,
                   gui->gameCamera.getCameraX(), gui->gameCamera.getCameraZ());
      glPopMatrix();

      /* Render All Static Scenery Objects */
      renderSceneryObjects();

      glPushMatrix();   
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
         else if(gui->getState() == GUI_IO_STATE_OBJECTS)
         {
            objectEditor->drawTemporary();
         }
         else if(gui->getState() == GUI_IO_STATE_PARTICLES)
         {
            particleEditor->drawTemporary(visibleMatrix);
            grassWindow* gr = gui->getGrassWindow();
            if(gr)
            {
               gr->drawTemporary();
            }
            waterWindow* wt = gui->getWaterWindow();
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

   /* Draw the NPCs */
   if(NPCs)
   {
      character* per = NPCs->getFirst();
      int aux;
      GLfloat x[4]; GLfloat z[4];
      GLfloat min[3],max[3];
      for(aux=0;aux < NPCs->getTotal();aux++)
      {
         /* Actualize NPCs */
         per->update(WALK_UPDATE/*seconds*/);   
         per->calculateBoundingBox();
         /* Verify Bounding Box */
         x[0] = per->min[0];
         z[0] = per->min[2];
         x[1] = per->min[0];
         z[1] = per->max[2]; 
         x[2] = per->max[0];
         z[2] = per->max[2];
         x[3] = per->max[0];
         z[3] = per->min[2];
         rotTransBoundingBox(per->orientation, x, z,per->xPosition, 
                             per->min[1] + per->yPosition, 
                             per->max[1] + per->yPosition,
                             per->zPosition, min, max );

         /* Only Draw Visible Characters */
         if(visibleCube(min[0],min[1],min[2],max[0],max[1],max[2],
                        visibleMatrix))
         {
            glPushMatrix();
               glTranslatef(per->xPosition, per->yPosition,
                            per->zPosition);
               glRotatef(per->orientation,0,1,0);
               per->render();
             /*per->RenderBoundingBox();
               glColor3f(0.6,0.1,0.1);
               glBegin(GL_POLYGON);
                   glVertex3f(per->min[0],per->min[1]+1,per->min[2]);
                   glVertex3f(per->min[0],per->min[1]+1,per->max[2]);
                   glVertex3f(per->max[0],per->min[1]+1,per->max[2]);
                   glVertex3f(per->max[0],per->min[1]+1,per->min[2]);
               glEnd();*/
            glPopMatrix();
         }
         per = (character*) per->next;
      }
   }
   glColor4f(1.0,1.0,1.0,1.0);

   /* Draw Particles */
   glPushMatrix();
     particleSystem->updateAll(xReal,yReal,zReal,visibleMatrix, true);
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
      draw3DMode(OUTDOOR_FARVIEW);
   glPopMatrix();
   glEnable(GL_FOG);
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);

   glColor4f(1.0,1.0,1.0,1.0);

   /* Draw Active Texture */
   if(mapOpened)
   {
      gluUnProject(SCREEN_X,SCREEN_Y, 0.01, modl, proj, viewPort, 
                   &x1, &y1, &z1);
      gluUnProject(SCREEN_X,SCREEN_Y-50,0.01, modl, proj, viewPort, 
                   &x2, &y2, &z2);
      gluUnProject(SCREEN_X-50,SCREEN_Y-50,0.01,modl,proj,viewPort, 
                   &x3, &y3, &z3);
      gluUnProject(SCREEN_X-50,SCREEN_Y,0.01, modl, proj, viewPort,  
                   &x4, &y4, &z4);

      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, actualTexture );
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
}

/*********************************************************************
 *                              doEditorIO                           *
 *********************************************************************/
void editor::doEditorIO()
{
   GLfloat wx, wy, wz;

   wx = mouseX; wy = 600-mouseY; 

   glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
   gluUnProject( wx, wy, wz, modl, proj, viewPort, &xReal, &yReal, &zReal);

   if( (gui->getState() == GUI_IO_STATE_TERRAIN) && (mapOpened))
   {
      terrainEditor->verifyAction(xReal, yReal, zReal, mButton, gui->getTool(), 
                                  actualTexture);
      particleSystem->setActualMap(map, NULL);
   }
   else if( (gui->getState() == GUI_IO_STATE_PORTAL) && (mapOpened))
   {
      string doorFile = gui->getSelectedText();
      if( (portalEditor->getDoor() == NULL) || 
          (doorFile != portalEditor->getDoorFileName())
        )
      {
         if(!doorFile.empty())
         {
            object* obj = map->insertObject(doorFile, *models, *wTypes);
            portalEditor->defineDoor((mapObject*)obj, gui->getSelectedText());
         }
      }
      portalEditor->verifyAction(xReal, yReal, zReal, mButton, gui->getTool(),
                                 proj, modl, viewPort);
   }
   else if( (gui->getState() == GUI_IO_STATE_WALL) && (mapOpened))
   {
      int tl = gui->getTool();
      wallEditor->verifyAction(xReal, yReal, zReal, mButton, keys, 
                               tl, actualTexture);
      gui->setTool(tl);
   }
   else if( (gui->getState() == GUI_IO_STATE_OBJECTS) && (mapOpened))
   {
      string objFile = gui->getSelectedText();
      if( (!objFile.empty()) && (objFile != objectEditor->getObjectFileName()))
      {
         mapObject* obj = (mapObject*) map->insertObject(objFile, *models, 
                                                         *wTypes);
         objectEditor->defineActualObject(obj, objFile);
      }
      objectEditor->verifyAction(keys, xReal, yReal, zReal, mButton, mouseX, mouseY,
                                 gui->getTool(), proj, modl, viewPort);
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

}

/*********************************************************************
 *                              verifyIO                             *
 *********************************************************************/
void editor::verifyIO()
{
   int guiEvent;
   bool outdoor = false;

   SDL_PumpEvents();
   keys = SDL_GetKeyState(NULL);
   mButton = SDL_GetMouseState(&mouseX,&mouseY);

   if(mapOpened)
   {
      outdoor = map->isOutdoor();
   }

   guiEvent = gui->doIO(mouseX, mouseY, mButton, keys, outdoor);
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
void editor::run()
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

