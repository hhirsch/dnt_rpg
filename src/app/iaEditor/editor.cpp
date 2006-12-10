#include "editor.h"

/*********************************************************************
 *                            Constructor                             *
 *********************************************************************/
editor::editor()
{
   wx = -1;
   wy = -1;
   wz = -1;
   mapOpened = false;
   map = NULL;
   actualObject = NULL;
   
   /* Particles */
   enableParticles = true;
   /*particleSystem = new partSystem();*/

   /*features = new featsList("../data/feats/Ingles/",
                                       "../data/feats/feats.ftl");*/
   Farso_Iniciar(&screen,"DccNiTghtmare IA Editor 0.1");
   init();

//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

   gui = new(guiIO);
   
   hour = 12.0;
   gameSun = new sun(hour , FARVIEW, FARVIEW);

   simulationStarted = false;

   agentsSimulation = new(agents);

}

/*********************************************************************
 *                            Destructor                             *
 *********************************************************************/
editor::~editor()
{
   if(mapOpened)
   {
      delete(map);
   }
   delete(gameSun);
   delete(agentsSimulation);
   /*if(particleSystem != NULL)
   {
      delete(particleSystem);
   }*/
   /*if(NPCs != NULL)
   {
      delete(NPCs);
   }
   if(features)
   {
      delete(features);
   }*/
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
      gui->showMessage("Closing actual Map...");
      delete(map);
      mapOpened = false;
   }
   gui->showMessage("Opening actual Map...");
   draw();
   map = new(Map);

   //TODO prompt for filename
   //
   if(map->open("../data/mapas/rua.map"))
   {
      mapOpened = true;

      /* Open NPCs */
      /*if(NPCs)
         delete(NPCs);
      NPCs = new (Lpersonagem);
      personagem* per;
      if(!map->npcFileName.empty())
      {
         FILE* arq;
         if(!(arq = fopen(map->npcFileName.c_str(),"r")))
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
                per = NPCs->InserirPersonagem(arquivo,features);
                per->posicaoLadoX = posX;
                per->posicaoLadoZ = posZ;
            }
            fclose(arq);
         }  
      }*/
      
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
      /* Open Particles 
      if(!map->particlesFileName.empty())
      {
          particleSystem->loadFromFile(map->particlesFileName);
          particleSystem->stabilizeAll();
      }
      else
      {
         particleSystem->deleteAll();
      }*/

      actualObject = (mapObjeto*) map->Objetos->primeiro->proximo;
      gui->showMessage("Map opened.");
   }
   else
   {
      gui->showMessage("Failed To open map!\n");
   }
}


/*********************************************************************
 *                               Save IAFile                         *
 *********************************************************************/
void editor::saveIAFile()
{
   gui->showMessage("Not yet implemented!");
}

/*********************************************************************
 *                              Open IAFile                          *
 *********************************************************************/
void editor::openIAFile()
{
   gui->showMessage("Not yet implemented!");
}

/*********************************************************************
 *                               New IAFile                          *
 *********************************************************************/
void editor::newIAFile()
{
   gui->showMessage("Not yet implemented!");
}

/*********************************************************************
 *                           Verify Position                         *
 *********************************************************************/
void editor::verifyPosition()
{
   if(mapOpened)
   {
       if(gui->gameCamera.getCenterX() > ((map->x * SQUARESIZE)+20))
       {
         gui->gameCamera.actualizeCamera( ((map->x * SQUARESIZE)+20) ,
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
       if(gui->gameCamera.getCenterZ() > ((map->z * SQUARESIZE)+20))
       {
          gui->gameCamera.actualizeCamera( gui->gameCamera.getCenterX(),
                                          gui->gameCamera.getCenterY(),
                                          ((map->z * SQUARESIZE)+20), 
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

/*********************************************************************
 *                                Draw                               *
 *********************************************************************/
void editor::draw()
{

   //GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glLoadIdentity();
   glClearColor(0.3,0.3,0.8,1.0);


   /* Redefine camera position */
   gui->cameraPos();
   AtualizaFrustum( visibleMatrix, proj, modl);
   gameSun->actualizeHourOfDay(hour);

   glPushMatrix();
   
   /* Draw Things */
   if(mapOpened)
   {
      map->draw(gui->gameCamera.getCameraX(), gui->gameCamera.getCameraY(), 
                   gui->gameCamera.getCameraZ(), visibleMatrix);
      agentsSimulation->draw();
   }

   glColor3f(1.0,1.0,1.0);
         
   /* Draw the NPCs */
#if 0
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

#endif

   /* Draw Particles 
   if(enableParticles)
   {
      glPushMatrix();
         particleSystem->actualizeAll(0,0,visibleMatrix);
      glPopMatrix();
   }*/

   /* Draw GUI */
   gui->draw(proj, modl, viewPort);

   glPopMatrix();
   
   glFlush();
   SDL_GL_SwapBuffers();
}

/*********************************************************************
 *                              doEditorIO                           *
 *********************************************************************/
void editor::doEditorIO()
{
   if( (wx != mouseX) || (wy != 600-mouseY))
   {   
      wx = mouseX; wy = 600-mouseY; 
      glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
      gluUnProject( wx, wy, wz, modl, proj, viewPort, &xReal, &yReal, &zReal);
   }
   


   if(!simulationStarted)
   {
      agentsSimulation->verifyAction(xReal, yReal, zReal, mButton, 
                                     gui->getTool(), map, &actualObject);
   }

   if( (gui->getTool() == TOOL_SIM_PLAY) && (!simulationStarted))
   {
      simulationStarted = true;
   }
   else if( (gui->getTool() == TOOL_SIM_PAUSE) && (simulationStarted))
   {
      simulationStarted = false;
   }
   else if( (gui->getTool() == TOOL_SIM_STOP) && (simulationStarted))
   {
      simulationStarted = false;
      //TODO Restart State
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

   guiEvent = gui->doIO(mouseX, mouseY, mButton, keys, 
                        agentsSimulation->scores[0], 
                        agentsSimulation->scores[1], 
                        agentsSimulation->scores[2]);

   if(guiEvent == GUI_IO_EXIT)
   {
      quit = true;
   }
   else if(guiEvent == GUI_IO_NOTHING)
   {
      doEditorIO();
   }
   else if(guiEvent == GUI_IO_SAVE_IAFILE)
   {
      agentsSimulation->verifyAction(xReal, yReal, zReal, mButton, 
                                     TOOL_SIM_NONE, map, &actualObject);

      gui->showMessage(agentsSimulation->saveState(gui->getFileName()));
   }
   else if(guiEvent == GUI_IO_OPEN_IAFILE)
   {
      agentsSimulation->verifyAction(xReal, yReal, zReal, mButton, 
                                     TOOL_SIM_NONE, map, &actualObject);

      gui->showMessage(agentsSimulation->loadState(gui->getFileName()));
   }
   else if(guiEvent == GUI_IO_NEW_IAFILE)
   {
      delete(agentsSimulation);
      agentsSimulation = new(agents);
      gui->showMessage("New IA Started!");
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

   openMap();
   
   while(!quit)
   {
      if(SDL_GetTicks() - time >= ACTUALIZATION_RATE)
      {
         time = SDL_GetTicks();

         verifyPosition();
         verifyIO();

         if(simulationStarted)
         {
            agentsSimulation->actualize( map );
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

