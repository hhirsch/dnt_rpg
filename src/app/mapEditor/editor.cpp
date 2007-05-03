#include "editor.h"

/*********************************************************************
 *                            Constructor                             *
 *********************************************************************/
editor::editor()
{
   mapOpened = false;
   map = NULL;
   NPCs = NULL;
   particleSystem = new partSystem();
   features = new featsList("../data/feats/Ingles/",
                                       "../data/feats/feats.ftl");
   Farso_Iniciar(&screen,"DccNiTghtmare Map Editor 0.1");
   init();

   gui = new(guiIO);
   hour = 12.0;
   gameSun = new sun(hour , FARVIEW, FARVIEW);
   gameSky = new(sky);
   models = new modelList();

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
   }
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
}

/*********************************************************************
 *                  redmensionate Window to Screen                   *
 *********************************************************************/
void editor::redmensionateWindow()
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei)screen->w / (GLsizei)screen->h, 1.0, FARVIEW);
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
 *                               Open Map                            *
 *********************************************************************/
void editor::openMap()
{
   if(mapOpened)
   {
      //Close Actual Map
      gui->showMessage("Closing actual Map...");
      delete(map);
      delete(terrainEditor);
      delete(portalEditor);
      delete(wallEditor);
      delete(objectEditor);
      delete(particleEditor);
      mapOpened = false;
   }
   gui->showMessage("Opening actual Map...");
   draw();
   map = new(Map);
   if(map->open(gui->getFileName(),*models))
   {
      mapOpened = true;
      terrainEditor = new terrain(map);
      portalEditor = new portal(map);
      wallEditor = new wall(map);
      objectEditor = new objects(map, models);
      particleEditor = new particles(map);
      actualTexture = map->Texturas->indice;

      /* Open NPCs */
      if(NPCs)
         delete(NPCs);
      NPCs = new (Lpersonagem);
      personagem* per;
      if(!map->getNpcFileName().empty())
      {
         FILE* arq;
         if(!(arq = fopen(map->getNpcFileName().c_str(),"r")))
         {
            gui->showMessage("Ouch, can't load NPC's file");
         }
         else
         {
            int total;
            int npc;
            char nome[30];
            char arquivo[255];
            float posX, posZ;
            fscanf(arq,"%d",&total);
            for(npc = 0; npc < total; npc++)
            {
                fscanf(arq,"%s %s %f %f",&nome[0],&arquivo[0],
                       &posX,&posZ);
                per = NPCs->insertCharacter(arquivo,features, NULL);
                per->posicaoLadoX = posX;
                per->posicaoLadoZ = posZ;
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
            glFogf(GL_FOG_DENSITY, 0.10);
            glHint(GL_FOG_HINT, GL_DONT_CARE);
            glFogf(GL_FOG_START, 100);
            glFogf(GL_FOG_END, HALFFARVIEW);
         }
      }
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
      if(particleSystem->numParticles() > 0)
      {
         map->setParticlesFileName(gui->getFileName()+".par");
         particleSystem->saveToFile(map->getParticlesFileName());
      }
      else
      {
         map->setParticlesFileName("");
      }
      map->save(gui->getFileName());
      tmp = "Map Saved as:";
      tmp += gui->getFileName();
      gui->showMessage(tmp);
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
      //Close Actual Map
      delete(map);
      delete(terrainEditor);
      delete(portalEditor);
      delete(wallEditor);
      delete(objectEditor);
      delete(particleEditor);
      particleSystem->deleteAll();
      if(NPCs)
      {
         delete(NPCs);
      }
      mapOpened = false;
   }

   mapOpened = true;
   map = new(Map);

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
      map->newMap(sizeX+12, sizeZ+12);
      int k, l;
      Square* saux;
      for(k = 0; k < sizeX+12; k++)
      {
         for(l=0; l < sizeZ+12; l++)
         {
            saux = map->relativeSquare(k,l);
            if( (sizeX < 6) || (sizeZ < 6) || (sizeX >= sizeX+6) ||
                (sizeZ >= sizeZ+6))
            {
               saux->flags &= !PISAVEL;
            }
         }
      }
      gui->gameCamera.actualizeCamera(((sizeX+12)*SQUARESIZE / 2.0), 0.0, 
                                      ((sizeZ+12)*SQUARESIZE / 2.0), 0.0);
   }
   else
   {
      map->newMap(sizeX,sizeZ);
   }
   terrainEditor = new terrain(map);
   portalEditor = new portal(map);
   wallEditor = new wall(map);
   objectEditor = new objects(map, models);
   particleEditor = new particles(map);
   actualTexture = map->Texturas->indice;
   NPCs = new (Lpersonagem);
   gui->showMessage("Created New Game Map!");
   glEnable(GL_FOG);
   {
      GLfloat color[3]={1.0,1.0,1.0};
      glFogi(GL_FOG_MODE,GL_LINEAR);
      glFogfv(GL_FOG_COLOR, color);
      glFogf(GL_FOG_DENSITY, 0.10);
      glHint(GL_FOG_HINT, GL_DONT_CARE);
      glFogf(GL_FOG_START, 100);
      glFogf(GL_FOG_END, HALFFARVIEW);
   }
}

/*********************************************************************
 *                           Verify Position                         *
 *********************************************************************/
void editor::verifyPosition()
{
   if(mapOpened)
   {
       if(gui->gameCamera.getCenterX() > ((map->getSizeX() * SQUARESIZE)+20))
       {
          gui->gameCamera.actualizeCamera( ((map->getSizeX() * SQUARESIZE)+20),
                                          gui->gameCamera.getCenterY(),
                                          gui->gameCamera.getCenterZ(), 
                                          0.0);
       }
       else if(gui->gameCamera.getCenterX() < -20)
       {
          gui->gameCamera.actualizeCamera( -20 ,
                                          gui->gameCamera.getCenterY(),
                                          gui->gameCamera.getCenterZ(), 
                                          0.0);
       }
       if(gui->gameCamera.getCenterZ() > ((map->getSizeZ() * SQUARESIZE)+20))
       {
          gui->gameCamera.actualizeCamera( gui->gameCamera.getCenterX(),
                                          gui->gameCamera.getCenterY(),
                                          ((map->getSizeZ() * SQUARESIZE)+20), 
                                          0.0);
       }
       else if(gui->gameCamera.getCenterZ() < -20)
       {
          gui->gameCamera.actualizeCamera( gui->gameCamera.getCenterX(),
                                          gui->gameCamera.getCenterY(),
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
   texture* tex = map->Texturas;
   while(aux < map->numtexturas-1)
   {
      if(tex->proximo->indice == actualTexture)
      {
         actualTexture = tex->indice;
         return(tex->indice);
      }
      tex = tex->proximo;
      aux++;
   }
   actualTexture = map->Texturas->indice;
   return(map->Texturas->indice);
}

/************************************************************************
 *                             Next Texture                             *
 ************************************************************************/
int editor::nextTexture()
{
   int aux=0;
   texture* tex = map->Texturas;
   while(aux < map->numtexturas)
   {
      if(tex->indice == actualTexture)
      {
         if(tex->proximo)
         {
           actualTexture = tex->proximo->indice;
           return(tex->proximo->indice);
         }
         else
         {
            actualTexture = tex->indice;
           return(tex->indice);
         }
      }
      tex = tex->proximo;
      aux++;
   }
   actualTexture = 0;
   return(-1);
}

/************************************************************************
 *             Insere Textura na Lista de Texturas                      *
 ************************************************************************/
int editor::insertTexture()
{
   texture* tex;

   SDL_Surface* img = IMG_Load(gui->getTextureFileName().c_str());
   if(!img)
   {
      gui->showMessage("Error on open texture");
      return(-1);
   }

   /* Insere realmente a textura */ 
   tex = (texture*) new(texture);
   if(map->numtexturas == 0)
   {
      map->Texturas = tex;
      tex->proximo = NULL;
   }
   else
   {
      tex->proximo = map->Texturas;
      map->Texturas = tex;
   }

   tex->arqNome = gui->getTextureFileName();
   tex->nome = gui->getTextureFileName();

   SDL_Surface *imgPotencia = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,imgPotencia,NULL);
 
   Uint8 R,G,B;
   
   Uint32 pixel = pixel_Pegar(imgPotencia,15,15);
   SDL_GetRGB(pixel,imgPotencia->format, &R, &G, &B);
   tex->R = R;
   tex->G = G;
   tex->B = B;

   glGenTextures(1, &(tex->indice));
   glBindTexture(GL_TEXTURE_2D, tex->indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imgPotencia->w,imgPotencia->h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, imgPotencia->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   map->numtexturas++;

   string tmp;
   tmp = "Inserted Texture:";
   tmp += gui->getTextureFileName();
   gui->showMessage(tmp);

   /* Libera a memoria utilizada */
   SDL_FreeSurface(img);
   SDL_FreeSurface(imgPotencia);
   return(tex->indice);
}

/*********************************************************************
 *                                Draw                               *
 *********************************************************************/
void editor::draw()
{

   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   glLoadIdentity();
   glClearColor(0.0,0.0,0.0,1.0);


   /* Redefine camera position */
   gui->cameraPos();
   AtualizaFrustum( visibleMatrix, proj, modl);
   gameSun->actualizeHourOfDay(hour);
   /* Draw Things */
   if(mapOpened)
   {
      /* SKY */

      if(map->isOutdoor())
      {
         /* Sky */
         glPushMatrix();
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
               glVertex3f(6*SQUARESIZE-5, 0.5, 6*SQUARESIZE);
               glVertex3f(6*SQUARESIZE-5, 0.5, (sizeZ+6)*SQUARESIZE);
               glVertex3f(6*SQUARESIZE+5, 0.5, (sizeZ+6)*SQUARESIZE);
               glVertex3f(6*SQUARESIZE+5, 0.5, 6*SQUARESIZE);

               glNormal3f(0,1,0);
               glVertex3f((sizeX+6)*SQUARESIZE-5, 0.5, 6*SQUARESIZE);
               glVertex3f((sizeX+6)*SQUARESIZE-5, 0.5, (sizeZ+6)*SQUARESIZE);
               glVertex3f((sizeX+6)*SQUARESIZE+5, 0.5, (sizeZ+6)*SQUARESIZE);
               glVertex3f((sizeZ+6)*SQUARESIZE+5, 0.5, 6*SQUARESIZE);

               glNormal3f(0,1,0);
               glVertex3f(6*SQUARESIZE, 0.5, 6*SQUARESIZE-5);
               glVertex3f((sizeX+6)*SQUARESIZE, 0.5, 6*SQUARESIZE-5);
               glVertex3f((sizeX+6)*SQUARESIZE, 0.5, 6*SQUARESIZE+5);
               glVertex3f(6*SQUARESIZE, 0.5, 6*SQUARESIZE+5);

               glNormal3f(0,1,0);
               glVertex3f(6*SQUARESIZE, 0.5, (sizeZ+6)*SQUARESIZE-5);
               glVertex3f((sizeX+6)*SQUARESIZE, 0.5, (sizeZ+6)*SQUARESIZE-5);
               glVertex3f((sizeX+6)*SQUARESIZE, 0.5, (sizeZ+6)*SQUARESIZE+5);
               glVertex3f(6*SQUARESIZE, 0.5, (sizeZ+6)*SQUARESIZE+5);

            glEnd();
         glPopMatrix();
      }
      glColor3f(1.0,1.0,1.0);

      glPushMatrix();
         map->draw(gui->gameCamera.getCameraX(), gui->gameCamera.getCameraY(), 
                   gui->gameCamera.getCameraZ(), visibleMatrix);
      glPopMatrix();

         glDisable(GL_LIGHTING);
         glBegin(GL_QUADS);
         glVertex3f(-10, -5, -10);
         glVertex3f(-10, -5, map->getSizeZ()*SQUARESIZE+10);
         glVertex3f(map->getSizeX()*SQUARESIZE+10, -5,
                    map->getSizeZ()*SQUARESIZE+10);
         glVertex3f(map->getSizeX()*SQUARESIZE+10, -5, -10);
//         glVertex3f(-10, -10, map->x*SQUARESIZE+10, map->z*SQUARESIZE+10);
         glEnd();
         glEnable(GL_LIGHTING);
         
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
            wallEditor->drawTemporary();
         }
         else if(gui->getState() == GUI_IO_STATE_OBJECTS)
         {
            objectEditor->drawTemporary();
         }
         else if(gui->getState() == GUI_IO_STATE_PARTICLES)
         {
            particleEditor->drawTemporary(visibleMatrix);
         }
   }

   glColor3f(1.0,1.0,1.0);

   /* Draw the NPCs */
   if(NPCs)
   {
      personagem* per = (personagem*) NPCs->primeiro->proximo;
      int aux;
      GLfloat x[4]; GLfloat z[4];
      GLfloat min[3],max[3];
      for(aux=0;aux < NPCs->getTotal();aux++)
      {
         /* Actualize NPCs */
         per->update(WALK_ACTUALIZATION/*seconds*/);   
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
         rotTransBoundingBox(per->orientacao, x, z,per->posicaoLadoX, 
                             per->min[1] + per->posicaoLadoY, 
                             per->max[1] + per->posicaoLadoY,
                             per->posicaoLadoZ, min, max );

         /* Only Draw Visible Characters */
         if(quadradoVisivel(min[0],min[1],min[2],max[0],max[1],max[2],
            visibleMatrix))
         {
            glPushMatrix();
               glTranslatef(per->posicaoLadoX, 0 ,per->posicaoLadoZ);
               glRotatef(per->orientacao,0,1,0);
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
         per = (personagem*) per->proximo;
      }
   }

   /* Draw Particles */
   glPushMatrix();
     particleSystem->actualizeAll(0,0,visibleMatrix, true);
   glPopMatrix();
   glDisable(GL_FOG);

   //gameSun->drawSun();

   glColor3f(1.0,1.0,1.0);

   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   /* Draw GUI */
   glPushMatrix();
      gui->draw(proj, modl, viewPort);
   glPopMatrix();
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);


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
   glFlush();
   SDL_GL_SwapBuffers();
}

/*********************************************************************
 *                              doEditorIO                           *
 *********************************************************************/
void editor::doEditorIO()
{
   GLdouble xReal,yReal,zReal;
   GLfloat wx, wy, wz;

   wx = mouseX; wy = 600-mouseY; 

   glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
   gluUnProject( wx, wy, wz, modl, proj, viewPort, &xReal, &yReal, &zReal);

   
   if( (gui->getState() == GUI_IO_STATE_TERRAIN) && (mapOpened))
   {
      terrainEditor->verifyAction(xReal, yReal, zReal, mButton, gui->getTool(), 
                                  actualTexture);
      particleSystem->setActualMap(map);
   }
   else if( (gui->getState() == GUI_IO_STATE_PORTAL) && (mapOpened))
   {
      if(portalEditor->getDoor() == NULL)
      {
         //FIXME take a door object!
         /*mapObjeto* porta = (mapObjeto*) map->Objetos->primeiro->proximo;
         while( (porta != map->Objetos->primeiro) && 
                (strcmp(porta->nome,"Door") != 0) ) 
         {
            porta = (mapObjeto*) porta->proximo;
         }
         if( (porta != map->Objetos->primeiro) )
         {
            portalEditor->defineDoor(porta);
         }*/
      }
      portalEditor->verifyAction(xReal, yReal, zReal, mButton, gui->getTool(),
                                 proj, modl, viewPort);
   }
   else if( (gui->getState() == GUI_IO_STATE_WALL) && (mapOpened))
   {
      wallEditor->verifyAction(xReal, yReal, zReal, mButton, keys, 
                               gui->getTool(), actualTexture);
   }
   else if( (gui->getState() == GUI_IO_STATE_OBJECTS) && (mapOpened))
   {
      objectEditor->verifyAction(xReal, yReal, zReal, mButton, mouseX, mouseY,
                                 gui->getTool(), proj, modl, viewPort);
   }
   else if( (gui->getState() == GUI_IO_STATE_PARTICLES) && (mapOpened))
   {
      particleEditor->verifyAction(xReal, yReal, zReal, mButton, keys, 
                                   gui->getTool(), particleSystem, proj, 
                                   modl, viewPort, gui->getSelectedText());
   }

}

/*********************************************************************
 *                              verifyIO                             *
 *********************************************************************/
void editor::verifyIO()
{
   int guiEvent;

   SDL_PumpEvents();
   keys = SDL_GetKeyState(NULL);
   mButton = SDL_GetMouseState(&mouseX,&mouseY);

   guiEvent = gui->doIO(mouseX, mouseY, mButton, keys);
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
        previousTexture();
   }
   else if(guiEvent == GUI_IO_TEXTURE_NEXT)
   {
     if(mapOpened)
        nextTexture();
   }
   else if(guiEvent == GUI_IO_TEXTURE_INSERT)
   {
      if(mapOpened)
      {
        insertTexture();
      }
   }
   else if(guiEvent == GUI_IO_OBJECT_INSERT)
   {
      if(mapOpened)
      {
         mapObject* obj=map->insertMapObject(gui->getObjectFileName().c_str(),
                                             *models);
         if(obj != NULL)
         {
            gui->showMessage(gui->getObjectFileName()+" Inserted.");
            objectEditor->defineActualObject(obj);
         }
         else
         {
            gui->showMessage(gui->getObjectFileName()+" Failed to Insert!.");
         }
      }
      else
      {
         gui->showMessage("Can't Insert while Map Not Opened!");
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
      if(SDL_GetTicks() - time >= ACTUALIZATION_RATE)
      {
         time = SDL_GetTicks();

         verifyPosition();
         verifyIO();
         
         draw();
      }
      else
      {
         int t = SDL_GetTicks();
         if(ACTUALIZATION_RATE - (t - time) > 5)
            SDL_Delay(ACTUALIZATION_RATE - (t - time));
      }
   }

}

