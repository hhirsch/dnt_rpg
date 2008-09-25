#include "editor.h"

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               editor                                   //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


/************************************************************************
 *                             Constructor                              *
 ************************************************************************/
editor::editor()
{
   /* Get current data directories */
   dir.findDataDirectories();

   /* Initialize SDL/Farso/etc */
   Farso_Init(&screen,"DccNiTghtmare's Particle Editor", 800, 600, false, 0);   

   /* Get OpenGL Extensions */
   ext.defineAllExtensions();

   /* Create the view */
   createView(screen);
   
   /* Clear */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);

   /* Some details  */ 
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Some light */
   GLfloat light_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   glDisable(GL_LIGHTING); 

   /* Create the camera */
   gameCamera = new camera();

   /* Create the GUI */
   gui = new guiInterface(NULL);

   /* Nullify elements */
   p = NULL;
   fileWindow = NULL;
   curFileName = "";
}

/************************************************************************
 *                              Destructor                              *
 ************************************************************************/
editor::~editor()
{
   /* Free Current Particle */
   if(p)
   {
      deleteParticle();
   }
   /* Free GUI */
   delete(gui);
   /* And free the camera */
   delete(gameCamera);
}

/************************************************************************
 *                           createView                                 *
 ************************************************************************/
void editor::createView(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei) screen->w / (GLsizei) screen->h, 1.0, 650.0);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/************************************************************************
 *                          deleteParticle                              *
 ************************************************************************/
void editor::deleteParticle()
{
   if(!p)
   {
      /* No particle to delete, so */
      return;
   }

   /* Delete the particle, by its type */
   switch(type)
   {
      case 1:
      {
         part1* p1 = (part1*)p;
         delete(p1);
         break;
      }
      case 2:
      {
         part2* p2 = (part2*)p;
         delete(p2);
         break;
      }
      case 3:
      {
         part3* p3 = (part3*)p;
         delete(p3);
         break;
      }
      case 4:
      {         
         part4* p4 = (part4*)p;
         delete(p4);
         break;
      }
      case 5:
      {          
         part5* p5 = (part5*)p;
         delete(p5);
         break;
      }
      case 6:
      {         
         part6* p6 = (part6*)p;
         delete(p6);
         break;
      }
      case 7:
      {         
         part7* p7 = (part7*)p;
         delete(p7);
         break;
      }
   }

   p = NULL;
}

/************************************************************************
 *                          createParticle                              *
 ************************************************************************/
bool editor::createParticle(int partType)
{
   if(p)
   {
      /* Must delete the existing one! */
      deleteParticle();
   }

   /* Create the particle, by its type */
   type = partType;

   switch(type)
   {
      case 1:
      { 
         p = (partAux*) new part1(50,60,120,curFileName);
         break;
      }
      case 2:
      {
         p = (partAux*) new part2(50,0,120,curFileName);
         break;
      }
      case 3:
      {
         p = (partAux*) new part3(50,20,120);
         break;
      }
      case 4:
      {
         p = (partAux*) new part4(50,0,120,curFileName);
         break;
      }
      case 5:
      {
         p = (partAux*) new part5(50,30,120,curFileName);
         break;
      }
      case 6:
      {
         p = (partAux*) new part6(50,250,120,curFileName);
         break;
      }
      case 7:
      {
         p = (partAux*) new part7(50,80,120,curFileName);
         break;
      }

      default:
      {
         printf("What the hell is type %d???\n",type);
         return(false);
         break;
      }
   }
   return(true);
}

/************************************************************************
 *                          updateParticle                              *
 ************************************************************************/
void editor::updateParticle()
{
   /* Must verify if has some particle opened */
   if(!p)
   {
      return;
   }

   /* Has so update it, by its type */
   switch(type)
   {
      case 1:
      {         
         part1* p1 = (part1*)p;
         p1->NextStep(viewMatrix);
         break;
      }
      case 2:
      {
         part2* p2 = (part2*)p;
         p2->NextStep(viewMatrix);
         break;
      }
      case  3:
      {      
         part3* p3 = (part3*)p;
         p3->NextStep(viewMatrix);
         break;
      }
      case 4:
      {
         part4* p4 = (part4*)p;
         p4->NextStep(viewMatrix);
         break;
      }
      case 5:
      {
         part5* p5 = (part5*)p;
         p5->NextStep(viewMatrix);
         break;
      }
      case 6:
      {         
         part6* p6 = (part6*)p;
         p6->NextStep(viewMatrix);
         break;
      }
      case 7:
      {         
         part7* p7 = (part7*)p;
         p7->NextStep(viewMatrix);
         break;
      }
   }
}

/****************************************************************
 *                      Open File  Window                       *
 ****************************************************************/
void editor::openFileWindow(bool load)
{
   if(fileWindow)
   {
       /* Close the current opened */
       gui->closeWindow(fileWindow);
   }

   fileLoading = load;
   fileWindow = gui->insertWindow(200,100,460,285,"File");
   fileSelector = fileWindow->getObjectsList()->insertFileSel(5,18,load,
                                                          "../data/particles/");
   fileSelector->setFilter(".par");
   if( (!fileLoading) && (!curFileName.empty()) )
   {
      fileSelector->setFileName(curFileName);
   }
   fileWindow->setAttributes(false,true,false,false);
   fileWindow->setExternPointer(&fileWindow);
   gui->openWindow(fileWindow);

}

/************************************************************************
 *                           createWindows                              *
 ************************************************************************/
void editor::createWindows()
{
   window* actWindow, *editWindow, *edit2Window;

   /* Actions Window */
   actWindow = gui->insertWindow(0,0,127,63,"Actions");
   buttonSave = actWindow->getObjectsList()->insertButton(5,27,43,45,"Save",0);
   buttonLoad = actWindow->getObjectsList()->insertButton(44,27,82,45,"Load",0);
   buttonExit = actWindow->getObjectsList()->insertButton(83,27,122,45,
                                                          "Exit",0);
   gui->openWindow(actWindow);

   /* Edit Window */
   editWindow = gui->insertWindow(0,64,127,575,"Edit");
   editWindow->getObjectsList()->insertTextBox(3,17,60,33,0,"MaxLive");
   editWindow->getObjectsList()->insertTextBar(60,17,123,33,"",0);
   editWindow->getObjectsList()->insertTextBox(3,37,60,53,0,"MaxParts");
   editWindow->getObjectsList()->insertTextBar(60,37,123,53,"",0);
   editWindow->getObjectsList()->insertTextBox(3,57,60,73,0,"CenterX");
   editWindow->getObjectsList()->insertTextBar(60,57,123,73,"",0);
   editWindow->getObjectsList()->insertTextBox(3,77,60,93,0,"CenterY");
   editWindow->getObjectsList()->insertTextBar(60,77,123,93,"",0);
   editWindow->getObjectsList()->insertTextBox(3,97,60,113,0,"CenterZ");
   editWindow->getObjectsList()->insertTextBar(60,97,123,113,"",0);
   editWindow->getObjectsList()->insertTextBox(3,117,60,133,0,"Gravity");
   editWindow->getObjectsList()->insertTextBar(60,117,123,133,"",0);
   editWindow->getObjectsList()->insertTextBox(3,137,60,153,0,"InitR");
   editWindow->getObjectsList()->insertTextBar(60,137,123,153,"",0);
   editWindow->getObjectsList()->insertTextBox(3,157,60,173,0,"InitG");
   editWindow->getObjectsList()->insertTextBar(60,157,123,173,"",0);
   editWindow->getObjectsList()->insertTextBox(3,177,60,193,0,"InitB");
   editWindow->getObjectsList()->insertTextBar(60,177,123,193,"",0);
   editWindow->getObjectsList()->insertTextBox(3,197,60,213,0,"FinalR");
   editWindow->getObjectsList()->insertTextBar(60,197,123,213,"",0);
   editWindow->getObjectsList()->insertTextBox(3,217,60,233,0,"FinalG");
   editWindow->getObjectsList()->insertTextBar(60,217,123,233,"",0);
   editWindow->getObjectsList()->insertTextBox(3,237,60,253,0,"FinalB");
   editWindow->getObjectsList()->insertTextBar(60,237,123,253,"",0);
   editWindow->getObjectsList()->insertTextBox(3,257,60,273,0,"Alpha");
   editWindow->getObjectsList()->insertTextBar(60,257,123,273,"",0);
   editWindow->getObjectsList()->insertTextBox(3,277,60,293,0,"MCntX");
   editWindow->getObjectsList()->insertTextBar(60,277,123,293,"",0);
   editWindow->getObjectsList()->insertTextBox(3,297,60,313,0,"MCntY");
   editWindow->getObjectsList()->insertTextBar(60,297,123,313,"",0);
   editWindow->getObjectsList()->insertTextBox(3,317,60,333,0,"MCntZ");
   editWindow->getObjectsList()->insertTextBar(60,317,123,333,"",0);
   editWindow->getObjectsList()->insertTextBox(3,337,60,353,0,"SCntX");
   editWindow->getObjectsList()->insertTextBar(60,337,123,353,"",0);
   editWindow->getObjectsList()->insertTextBox(3,357,60,373,0,"SCntY");
   editWindow->getObjectsList()->insertTextBar(60,357,123,373,"",0);
   editWindow->getObjectsList()->insertTextBox(3,377,60,393,0,"SCntZ");
   editWindow->getObjectsList()->insertTextBar(60,377,123,393,"",0);
   editWindow->getObjectsList()->insertTextBox(3,397,60,413,0,"MPosX");
   editWindow->getObjectsList()->insertTextBar(60,397,123,413,"",0);
   editWindow->getObjectsList()->insertTextBox(3,417,60,433,0,"MPosY");
   editWindow->getObjectsList()->insertTextBar(60,417,123,433,"",0);
   editWindow->getObjectsList()->insertTextBox(3,437,60,453,0,"MPosZ");
   editWindow->getObjectsList()->insertTextBar(60,437,123,453,"",0);
   editWindow->getObjectsList()->insertTextBox(3,457,60,473,0,"SPosX");
   editWindow->getObjectsList()->insertTextBar(60,457,123,473,"",0);
   editWindow->getObjectsList()->insertTextBox(3,474,60,490,0,"SPosY");
   editWindow->getObjectsList()->insertTextBar(60,474,123,490,"",0);
   editWindow->getObjectsList()->insertTextBox(3,491,60,505,0,"SPosZ");
   editWindow->getObjectsList()->insertTextBar(60,491,123,505,"",0);
   gui->openWindow(editWindow);

   /* Second Edit Window */
   edit2Window = gui->insertWindow(672,64,799,575,"Edit-2");
   edit2Window->getObjectsList()->insertTextBox(3,17,60,33,0,"DMColorR");
   edit2Window->getObjectsList()->insertTextBar(60,17,123,33,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,37,60,53,0,"DMColorG");
   edit2Window->getObjectsList()->insertTextBar(60,37,123,53,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,57,60,73,0,"DMColorB");
   edit2Window->getObjectsList()->insertTextBar(60,57,123,73,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,77,60,93,0,"DSColorR");
   edit2Window->getObjectsList()->insertTextBar(60,77,123,93,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,97,60,113,0,"DSColorG");
   edit2Window->getObjectsList()->insertTextBar(60,97,123,113,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,117,60,133,0,"DSColorB");
   edit2Window->getObjectsList()->insertTextBar(60,117,123,133,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,137,60,153,0,"DMVelX");
   edit2Window->getObjectsList()->insertTextBar(60,137,123,153,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,157,60,173,0,"DMVelY");
   edit2Window->getObjectsList()->insertTextBar(60,157,123,173,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,177,60,193,0,"DMVelZ");
   edit2Window->getObjectsList()->insertTextBar(60,177,123,193,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,197,60,213,0,"DSVelX");
   edit2Window->getObjectsList()->insertTextBar(60,197,123,213,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,217,60,233,0,"DSVelY");
   edit2Window->getObjectsList()->insertTextBar(60,217,123,233,"",0);
   edit2Window->getObjectsList()->insertTextBox(3,237,60,253,0,"DSVelZ");
   edit2Window->getObjectsList()->insertTextBar(60,237,123,253,"",0);
   gui->openWindow(edit2Window);
}

/************************************************************************
 *                           treatGuiEvents                             *
 ************************************************************************/
void editor::treatGuiEvents()
{
   int eventInfo = NOTHING;

   done = false;

   /* Get events */
   guiObject* obj = gui->manipulateEvents(mouseX, mouseY, mButton, 
                                          keys, eventInfo);

   if(eventInfo == NOTHING)
   {
      /* No Event, so must treat Camera Input */
      gameCamera->doIO(keys, mButton, mouseX, mouseY, DELTA_CAMERA);

      if(keys[SDLK_r])
      {
         /* Reload Current Particle */
         if(p)
         {
            //deleteParticle();
            //createParticle(currentFileName);
         }
      }
   }
   else if(eventInfo == PRESSED_BUTTON)
   {
      /* Exit was pressed! */
      if(obj == buttonExit)
      {
         done = true;
      }
      else if(obj == buttonSave)
      {
         openFileWindow(false);
      }
      else if(obj == buttonLoad)
      {
         openFileWindow(true);
      }
   }
   /* File Selectors Things */
   else if(eventInfo == FILE_SEL_ACCEPT)
   {
      if(fileWindow)
      {
         if(obj == (guiObject*)fileSelector) 
         {
            curFileName = fileSelector->getFileName();
            gui->closeWindow(fileWindow);
            if(fileLoading)
            {
               /* Load the particle (with curFileName) */
               //TODO Get Particle Type
               createParticle(2);
            }
            else
            {
              /* TODO call Save Function */
              //saveParticle();
            }
         }
      }
   }
   else if(eventInfo == FILE_SEL_CANCEL)
   {
      if(fileWindow)
      {
         /* Just close the window */
         if(obj == (guiObject*)fileSelector) 
         {
            gui->closeWindow(fileWindow);
         }
      }
   }
}

/************************************************************************
 *                                render                                *
 ************************************************************************/
void editor::render()
{
   glClearColor(0,0,0,0);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   glLoadIdentity();
   
   /* Redefine Camera Position */
   gameCamera->lookAt(NULL);
   updateFrustum(viewMatrix,proj,modl);

   /* Part Draw */
   glPushMatrix();
      updateParticle();
   glPopMatrix();

   glColor3f(1.0,1.0,1.0);

   glDisable(GL_BLEND);

   draw2DMode(); 
      gui->draw(proj,modl,viewPort);
   draw3DMode(INDOOR_FARVIEW);

   glFlush();
   SDL_GL_SwapBuffers();
}

/************************************************************************
 *                                 run                                  *
 ************************************************************************/
void editor::run()
{
   createWindows();

   /* Flux Control Variables */
   Uint32 lastUpdate = 0;
   Uint32 varTime = 0;

   while(!done)
   {
      varTime = (SDL_GetTicks() - lastUpdate);
      if(varTime >= UPDATE_RATE)
      {
         lastUpdate = SDL_GetTicks();

         /* Get Keyboard and Mouse State */
         SDL_PumpEvents();
         mButton = SDL_GetMouseState(&mouseX,&mouseY);
         keys = SDL_GetKeyState(NULL);

         /* Treat GUI Events */
         treatGuiEvents();

         /* Render Things */
         render();
      }
   }
}

