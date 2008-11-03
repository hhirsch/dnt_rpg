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
   int i;
   /* Alloc the visible Matrix */
   viewMatrix = new GLfloat*[6];
   for(i = 0; i < 6; i++)
   {
      viewMatrix[i] = new GLfloat[4];
   }

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
   int i;

   /* Free Current Particle */
   if(p)
   {
      deleteParticle();
   }
   /* Free GUI */
   delete(gui);
   /* And free the camera */
   delete(gameCamera);
   /* Clear the visibleMatrix */
   for(i = 0; i < 6; i++)
   {
      delete[] viewMatrix[i];
   }
   delete[] viewMatrix;
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

   updateTexts();
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
         p1->nextStep(viewMatrix);
         break;
      }
      case 2:
      {
         part2* p2 = (part2*)p;
         p2->nextStep(viewMatrix);
         break;
      }
      case  3:
      {      
         part3* p3 = (part3*)p;
         p3->nextStep(viewMatrix);
         break;
      }
      case 4:
      {
         part4* p4 = (part4*)p;
         p4->nextStep(viewMatrix);
         break;
      }
      case 5:
      {
         part5* p5 = (part5*)p;
         p5->nextStep(viewMatrix);
         break;
      }
      case 6:
      {         
         part6* p6 = (part6*)p;
         p6->nextStep(viewMatrix);
         break;
      }
      case 7:
      {         
         part7* p7 = (part7*)p;
         p7->nextStep(viewMatrix);
         break;
      }
   }
}

/****************************************************************
 *                       Open Type Window                       *
 ****************************************************************/
void editor::openTypeWindow()
{
   typeWindow = gui->insertWindow(200,100,320,285,"Particle Type");

   waterButton = typeWindow->getObjectsList()->insertButton(15,27,105,45,
                                                            "Waterfall",0);
   fireButton = typeWindow->getObjectsList()->insertButton(15,47,105,65,
                                                           "Fire",0);
   smokeButton = typeWindow->getObjectsList()->insertButton(15,67,105,85,
                                                            "Smoke",0);
   bloodButton = typeWindow->getObjectsList()->insertButton(15,87,105,105,
                                                            "Blood",0);
   lightningButton = typeWindow->getObjectsList()->insertButton(15,107,105,125,
                                                                "Lightning",0);
   snowButton = typeWindow->getObjectsList()->insertButton(15,127,105,145,
                                                            "Snow",0);
   typeWindow->setExternPointer(&typeWindow);
   gui->openWindow(typeWindow);
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
   maxLiveEdit = editWindow->getObjectsList()->insertTextBar(60,17,123,33,"",0);
   editWindow->getObjectsList()->insertTextBox(3,37,60,53,0,"MaxParts");
   maxPartsEdit = editWindow->getObjectsList()->insertTextBar(60,37,123,53,
                                                              "",0);
   editWindow->getObjectsList()->insertTextBox(3,57,60,73,0,"CenterX");
   centerXEdit = editWindow->getObjectsList()->insertTextBar(60,57,123,73,"",0);
   editWindow->getObjectsList()->insertTextBox(3,77,60,93,0,"CenterY");
   centerYEdit = editWindow->getObjectsList()->insertTextBar(60,77,123,93,"",0);
   editWindow->getObjectsList()->insertTextBox(3,97,60,113,0,"CenterZ");
   centerZEdit = editWindow->getObjectsList()->insertTextBar(60,97,123,113,
                                                             "",0);
   editWindow->getObjectsList()->insertTextBox(3,117,60,133,0,"Gravity");
   gravityEdit = editWindow->getObjectsList()->insertTextBar(60,117,123,133,
                                                              "",0);
   editWindow->getObjectsList()->insertTextBox(3,137,60,153,0,"InitR");
   initREdit = editWindow->getObjectsList()->insertTextBar(60,137,123,153,"",0);
   editWindow->getObjectsList()->insertTextBox(3,157,60,173,0,"InitG");
   initGEdit = editWindow->getObjectsList()->insertTextBar(60,157,123,173,"",0);
   editWindow->getObjectsList()->insertTextBox(3,177,60,193,0,"InitB");
   initBEdit = editWindow->getObjectsList()->insertTextBar(60,177,123,193,"",0);
   editWindow->getObjectsList()->insertTextBox(3,197,60,213,0,"FinalR");
   finalREdit = editWindow->getObjectsList()->insertTextBar(60,197,123,213,
                                                            "",0);
   editWindow->getObjectsList()->insertTextBox(3,217,60,233,0,"FinalG");
   finalGEdit = editWindow->getObjectsList()->insertTextBar(60,217,123,233,
                                                            "",0);
   editWindow->getObjectsList()->insertTextBox(3,237,60,253,0,"FinalB");
   finalBEdit = editWindow->getObjectsList()->insertTextBar(60,237,123,253,
                                                            "",0);
   editWindow->getObjectsList()->insertTextBox(3,257,60,273,0,"Alpha");
   alphaEdit = editWindow->getObjectsList()->insertTextBar(60,257,123,273,"",0);
   editWindow->getObjectsList()->insertTextBox(3,277,60,293,0,"MCntX");
   mCntXEdit = editWindow->getObjectsList()->insertTextBar(60,277,123,293,"",0);
   editWindow->getObjectsList()->insertTextBox(3,297,60,313,0,"MCntY");
   mCntYEdit = editWindow->getObjectsList()->insertTextBar(60,297,123,313,"",0);
   editWindow->getObjectsList()->insertTextBox(3,317,60,333,0,"MCntZ");
   mCntZEdit = editWindow->getObjectsList()->insertTextBar(60,317,123,333,"",0);
   editWindow->getObjectsList()->insertTextBox(3,337,60,353,0,"SCntX");
   sCntXEdit = editWindow->getObjectsList()->insertTextBar(60,337,123,353,"",0);
   editWindow->getObjectsList()->insertTextBox(3,357,60,373,0,"SCntY");
   sCntYEdit = editWindow->getObjectsList()->insertTextBar(60,357,123,373,"",0);
   editWindow->getObjectsList()->insertTextBox(3,377,60,393,0,"SCntZ");
   sCntZEdit = editWindow->getObjectsList()->insertTextBar(60,377,123,393,"",0);
   editWindow->getObjectsList()->insertTextBox(3,397,60,413,0,"MPosX");
   mPosXEdit = editWindow->getObjectsList()->insertTextBar(60,397,123,413,"",0);
   editWindow->getObjectsList()->insertTextBox(3,417,60,433,0,"MPosY");
   mPosYEdit = editWindow->getObjectsList()->insertTextBar(60,417,123,433,"",0);
   editWindow->getObjectsList()->insertTextBox(3,437,60,453,0,"MPosZ");
   mPosZEdit = editWindow->getObjectsList()->insertTextBar(60,437,123,453,"",0);
   editWindow->getObjectsList()->insertTextBox(3,457,60,473,0,"SPosX");
   sPosXEdit = editWindow->getObjectsList()->insertTextBar(60,457,123,473,"",0);
   editWindow->getObjectsList()->insertTextBox(3,474,60,490,0,"SPosY");
   sPosYEdit = editWindow->getObjectsList()->insertTextBar(60,474,123,490,"",0);
   editWindow->getObjectsList()->insertTextBox(3,491,60,505,0,"SPosZ");
   sPosZEdit = editWindow->getObjectsList()->insertTextBar(60,491,123,505,"",0);
   gui->openWindow(editWindow);

   /* Second Edit Window */
   edit2Window = gui->insertWindow(672,64,799,575,"Edit-2");
   edit2Window->getObjectsList()->insertTextBox(3,17,60,33,0,"DMColorR");
   dMColorREdit = edit2Window->getObjectsList()->insertTextBar(60,17,123,33,
                                                               "",0);
   edit2Window->getObjectsList()->insertTextBox(3,37,60,53,0,"DMColorG");
   dMColorGEdit = edit2Window->getObjectsList()->insertTextBar(60,37,123,53,
                                                               "",0);
   edit2Window->getObjectsList()->insertTextBox(3,57,60,73,0,"DMColorB");
   dMColorBEdit = edit2Window->getObjectsList()->insertTextBar(60,57,123,73,
                                                               "",0);
   edit2Window->getObjectsList()->insertTextBox(3,77,60,93,0,"DSColorR");
   dSColorREdit = edit2Window->getObjectsList()->insertTextBar(60,77,123,93,
                                                               "",0);
   edit2Window->getObjectsList()->insertTextBox(3,97,60,113,0,"DSColorG");
   dSColorGEdit = edit2Window->getObjectsList()->insertTextBar(60,97,123,113,
                                                               "",0);
   edit2Window->getObjectsList()->insertTextBox(3,117,60,133,0,"DSColorB");
   dSColorBEdit = edit2Window->getObjectsList()->insertTextBar(60,117,123,133,
                                                               "",0);
   edit2Window->getObjectsList()->insertTextBox(3,137,60,153,0,"DMVelX");
   dMVelXEdit = edit2Window->getObjectsList()->insertTextBar(60,137,123,153,
                                                             "",0);
   edit2Window->getObjectsList()->insertTextBox(3,157,60,173,0,"DMVelY");
   dMVelYEdit = edit2Window->getObjectsList()->insertTextBar(60,157,123,173,
                                                             "",0);
   edit2Window->getObjectsList()->insertTextBox(3,177,60,193,0,"DMVelZ");
   dMVelZEdit = edit2Window->getObjectsList()->insertTextBar(60,177,123,193,
                                                             "",0);
   edit2Window->getObjectsList()->insertTextBox(3,197,60,213,0,"DSVelX");
   dSVelXEdit = edit2Window->getObjectsList()->insertTextBar(60,197,123,213,
                                                             "",0);
   edit2Window->getObjectsList()->insertTextBox(3,217,60,233,0,"DSVelY");
   dSVelYEdit = edit2Window->getObjectsList()->insertTextBar(60,217,123,233,
                                                             "",0);
   edit2Window->getObjectsList()->insertTextBox(3,237,60,253,0,"DSVelZ");
   dSVelZEdit = edit2Window->getObjectsList()->insertTextBar(60,237,123,253,
                                                             "",0);
   edit2Window->getObjectsList()->insertTextBox(3,257,60,273,0,"initVelX");
   initVelXEdit = edit2Window->getObjectsList()->insertTextBar(60,257,123,273,
                                                             "",0);
   edit2Window->getObjectsList()->insertTextBox(3,277,60,293,0,"initVelY");
   initVelYEdit = edit2Window->getObjectsList()->insertTextBar(60,277,123,293,
                                                             "",0);
   edit2Window->getObjectsList()->insertTextBox(3,297,60,313,0,"initVelZ");
   initVelZEdit = edit2Window->getObjectsList()->insertTextBar(60,297,123,313,
                                                             "",0);
   gui->openWindow(edit2Window);
}

/************************************************************************
 *                           treatTextBars                              *
 ************************************************************************/
void editor::treatTextBars(guiObject* obj)
{
   if(!p)
   {
      /* No need to treat text bars if no 
       * particle was opened */
      return;
   }

   /* Get the value */
   float value = 0;
   sscanf(obj->getText().c_str(), "%f", &value);

   if(obj == maxLiveEdit)
   {
      p->setMaxLive((int)value);
   }
   else if(obj == maxPartsEdit)
   {
      if(value == 0)
      {
         value = 1;
      }
      p->setMaxParticles((int)value);
      p->finish();
      p->init((int)value, p->getDrawMode());
   }
   else if(obj == centerXEdit)
   {
      p->setCenterX(value);
   }
   else if(obj == centerYEdit)
   {
      p->setCenterY(value);
   }
   else if(obj == centerZEdit)
   {
      p->setCenterZ(value);
   }
   else if(obj == gravityEdit)
   {
      p->setGravity(value);
   }
   else if(obj == initREdit)
   {
      p->setInitR(value);
   }
   else if(obj == initGEdit)
   {
      p->setInitG(value);
   }
   else if(obj == initBEdit)
   {
      p->setInitB(value);
   }
   else if(obj == finalREdit)
   {
      p->setFinalR(value);
   }
   else if(obj == finalGEdit)
   {
      p->setFinalG(value);
   }
   else if(obj == finalBEdit)
   {
      p->setFinalB(value);
   }
   else if(obj == alphaEdit)
   {
      p->setAlpha(value);
   }
   else if(obj == mCntXEdit)
   {
      p->setDMultCenterX(value);
   }
   else if(obj == mCntYEdit)
   {
      p->setDMultCenterY(value);
   }
   else if(obj == mCntZEdit)
   {
      p->setDMultCenterZ(value);
   }
   else if(obj == sCntXEdit)
   {
      p->setDSumCenterX(value);
   }
   else if(obj == sCntYEdit)
   {
      p->setDSumCenterY(value);
   }
   else if(obj == sCntZEdit)
   {
      p->setDSumCenterZ(value);
   }
   else if(obj == mPosXEdit)
   {
      p->setDMultPosX(value);
   }
   else if(obj == mPosYEdit)
   {
      p->setDMultPosY(value);
   }
   else if(obj == mPosZEdit)
   {
      p->setDMultPosZ(value);
   }
   else if(obj == sPosXEdit)
   {
      p->setDSumPosX(value);
   }
   else if(obj == sPosYEdit)
   {
      p->setDSumPosY(value);
   }
   else if(obj == sPosZEdit)
   {
      p->setDSumPosZ(value);
   }

   /**********************Edit*2*Window*****************************/
   else if(obj == dMColorREdit)
   {
      p->setDMultColorR(value);
   }
   else if(obj == dMColorGEdit)
   {
      p->setDMultColorG(value);
   }
   else if(obj == dMColorBEdit)
   {
      p->setDMultColorB(value);
   }
   else if(obj == dSColorREdit)
   {
      p->setDSumColorR(value);
   }
   else if(obj == dSColorGEdit)
   {
      p->setDSumColorG(value);
   }
   else if(obj == dSColorBEdit)
   {
      p->setDSumColorB(value);
   }
   else if(obj == dMVelXEdit)
   {
      p->setDMultVelX(value);
   }
   else if(obj == dMVelYEdit)
   {
      p->setDMultVelY(value);
   }
   else if(obj == dMVelZEdit)
   {
      p->setDMultVelZ(value);
   }
   else if(obj == dSVelXEdit)
   {
      p->setDSumVelX(value);
   }
   else if(obj == dSVelYEdit)
   {
      p->setDSumVelY(value);
   }
   else if(obj == dSVelZEdit)
   {
      p->setDSumVelZ(value);
   }
   else if(obj == initVelXEdit)
   {
      p->setInitVelX(value);
   }
   else if(obj == initVelYEdit)
   {
      p->setInitVelY(value);
   }
   else if(obj == initVelZEdit)
   {
      p->setInitVelZ(value);
   }
}

/************************************************************************
 *                            updateTexts                               *
 ************************************************************************/
void editor::updateTexts()
{
   char aux[10];
  
   sprintf(aux,"%d", p->getMaxLive());
   maxLiveEdit->setText(aux);
   sprintf(aux,"%d", p->getMaxParticles());
   maxPartsEdit->setText(aux);
   sprintf(aux,"%.3f", p->getCenterX());
   centerXEdit->setText(aux);
   sprintf(aux,"%.3f", p->getCenterY());
   centerYEdit->setText(aux);
   sprintf(aux,"%.3f", p->getCenterZ());
   centerZEdit->setText(aux);
   sprintf(aux,"%.3f", p->getGravity());
   gravityEdit->setText(aux);
   sprintf(aux,"%.3f", p->getInitR());
   initREdit->setText(aux);
   sprintf(aux,"%.3f", p->getInitG());
   initGEdit->setText(aux);
   sprintf(aux,"%.3f", p->getInitB());
   initBEdit->setText(aux);
   sprintf(aux,"%.3f", p->getFinalR());
   finalREdit->setText(aux);
   sprintf(aux,"%.3f", p->getFinalG());
   finalGEdit->setText(aux);
   sprintf(aux,"%.3f", p->getFinalB());
   finalBEdit->setText(aux);
   sprintf(aux,"%.3f", p->getAlpha());
   alphaEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultCenterX());
   mCntXEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultCenterY());
   mCntYEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultCenterZ());
   mCntZEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumCenterX());
   sCntXEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumCenterY());
   sCntYEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumCenterZ());
   sCntZEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultPosX());
   mPosXEdit->setText(aux);  
   sprintf(aux,"%.3f", p->getDMultPosY());
   mPosYEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultPosZ());
   mPosZEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumPosX());
   sPosXEdit->setText(aux);  
   sprintf(aux,"%.3f", p->getDSumPosY());
   sPosYEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumPosZ());
   sPosZEdit->setText(aux);

   /**********************Edit*2*Window*****************************/ 
   sprintf(aux,"%.3f", p->getDMultColorR());
   dMColorREdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultColorG());
   dMColorGEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultColorB());
   dMColorBEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumColorR());
   dSColorREdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumColorG());
   dSColorGEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumColorB());
   dSColorBEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultVelX());
   dMVelXEdit->setText(aux); 
   sprintf(aux,"%.3f", p->getDMultVelY());
   dMVelYEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDMultVelZ());
   dMVelZEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumVelX());
   dSVelXEdit->setText(aux); 
   sprintf(aux,"%.3f", p->getDSumVelY());
   dSVelYEdit->setText(aux);
   sprintf(aux,"%.3f", p->getDSumVelZ());
   dSVelZEdit->setText(aux);
   sprintf(aux,"%.3f", p->getInitVelX());
   initVelXEdit->setText(aux); 
   sprintf(aux,"%.3f", p->getInitVelY());
   initVelYEdit->setText(aux);
   sprintf(aux,"%.3f", p->getInitVelZ());
   initVelZEdit->setText(aux);
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
            deleteParticle();
            createParticle(type);
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
      else if(typeWindow != NULL)
      {
         if(obj == waterButton)
         {
            createParticle(1);
            gui->closeWindow(typeWindow);
         }
         else if(obj == fireButton)
         {
            createParticle(2);
            gui->closeWindow(typeWindow);
         }
         else if(obj == smokeButton)
         {
            createParticle(4);
            gui->closeWindow(typeWindow);
         }
         else if(obj == bloodButton)
         {
            createParticle(5);
            gui->closeWindow(typeWindow);
         }
         else if(obj == lightningButton)
         {
            createParticle(6);
            gui->closeWindow(typeWindow);
         }
         else if(obj == snowButton)
         {
            createParticle(7);
            gui->closeWindow(typeWindow);
         }
      }
   }
   /* Text Bars Change Events */
   else if(eventInfo == WROTE_TEXT_BAR)
   {
      treatTextBars(obj);
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
               /* Get Particle Type */
               openTypeWindow();
            }
            else
            {
               /* Save the Particle with desired fileName */
               if(p)
               {
                  warning warn;
                  if(p->save(curFileName))
                  {
                     warn.show("Message", "File was saved!", gui);
                  }
                  else
                  {
                     warn.show("Error", "Can't save file!", gui);
                  }
               }
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
   glDisable(GL_FOG);
   glDisable(GL_LIGHTING);

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
   Uint32 time = 0;

   while(!done)
   {
      time = SDL_GetTicks();
      varTime = (time - lastUpdate);
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
      else
      {
         int t = SDL_GetTicks();
         if(UPDATE_RATE - (t - time) > 5)
            SDL_Delay(UPDATE_RATE - (t - time));
      }
   }
}

