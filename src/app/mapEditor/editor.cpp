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

   terrainEditor = NULL;
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
   }
   delete(gameSun);
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
      delete(map);
      delete(terrainEditor);
      mapOpened = false;
   }
   map = new(Map);
   if(map->open(gui->getFileName()))
   {
      mapOpened = true;
      terrainEditor = new terrain(map);
      actualTexture = map->Texturas->indice;

      /* Open NPCs */
      if(NPCs)
         delete(NPCs);
      NPCs = new (Lpersonagem);
      personagem* per;
      if(!map->npcFileName.empty())
      {
         FILE* arq;
         if(!(arq = fopen(map->npcFileName.c_str(),"r")))
         {
            printf("Ouch, can't load NPC's file: %s.\n",
                   map->npcFileName.c_str());
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
                per = NPCs->InserirPersonagem(arquivo,features);
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
         glDisable(GL_FOG);
      }
      /* Open Particles */
      if(!map->particlesFileName.empty())
      {
          particleSystem->loadFromFile(map->particlesFileName);
          particleSystem->stabilizeAll();
      }
      else
      {
         particleSystem->deleteAll();
      }
   }
   else
   {
      printf("Failed To open!\n");
   }
}


/*********************************************************************
 *                               Save Map                            *
 *********************************************************************/
void editor::saveMap()
{
   if(mapOpened)
   {
      map->save(gui->getFileName());
   }
   else
   {
      //TODO GUI MESSAGES!
      printf("Map Not Opened\n");
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
      particleSystem->deleteAll();
      if(NPCs)
      {
         delete(NPCs);
      }
      mapOpened = false;
   }

   mapOpened = true;
   map = new(Map);
   //TODO Get Map Size!!!!
   map->newMap(8,8);
   terrainEditor = new terrain(map);
   actualTexture = map->Texturas->indice;
   NPCs = new (Lpersonagem);
}

/*********************************************************************
 *                           Verify Position                         *
 *********************************************************************/
void editor::verifyPosition()
{
   if(mapOpened)
   {
       if(gui->centerX > ((map->x * SQUARESIZE)+20))
       {
          gui->centerX = ((map->x * SQUARESIZE)+20);
       }
       else if(gui->centerX < -20)
       {
          gui->centerX = -20;
       }
       if(gui->centerZ > ((map->z * SQUARESIZE)+20))
       {
          gui->centerZ = ((map->z * SQUARESIZE)+20);
       }
       else if(gui->centerZ < -20)
       {
          gui->centerZ = -20;
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

/*********************************************************************
 *                                Draw                               *
 *********************************************************************/
void editor::draw()
{

   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glLoadIdentity();
   glClearColor(0.3,0.3,0.8,1.0);


   /* Redefine camera position */
   gui->camera();
   AtualizaFrustum( visibleMatrix, proj, modl);
   gameSun->actualizeHourOfDay(hour);

   glPushMatrix();
   
   /* Draw Things */
   if(mapOpened)
   {
         map->draw(gui->cameraX, gui->cameraY, gui->cameraZ, visibleMatrix);
         if(gui->getState() == GUI_IO_STATE_TERRAIN)
         {
            terrainEditor->drawTemporary();
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
      for(aux=0;aux < NPCs->total;aux++)
      {
         /* Actualize NPCs */
         per->m_calModel->update(WALK_ACTUALIZATION/*seconds*/);   
         per->CalculateBoundingBox();
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
               per->Render();
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
      particleSystem->actualizeAll(0,0,visibleMatrix);
   glPopMatrix();

   /* Draw GUI */
   gui->draw(proj, modl, viewPort);

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

   glPopMatrix();
   
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
   }
}

/*********************************************************************
 *                                 Run                               *
 *********************************************************************/
void editor::run()
{
   
   bool quit = false;
   int time = 0;
   int guiEvent;

   
   
   while(!quit)
   {
      if(SDL_GetTicks() - time >= ACTUALIZATION_RATE)
      {
         time = SDL_GetTicks();

         verifyPosition();

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
         else if(guiEvent == GUI_IO_NOTHING)
         {
            doEditorIO();
         }
         
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

