#include "guiIO.h"
#include "../../engine/util.h"
#include "message.h"

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
guiIO::guiIO()
{
   gui = new interface(NULL);

   state = GUI_IO_STATE_INITIAL;
   tool = TOOL_NONE;

   /* Open Windows */
   openFileWindow();
   openNavWindow();
   openMessageWindow();
   openObjectsWindow();
   openPlayControlWindow();
   openTp3Window();
   
   /* Camera Things */
   theta = 35;
   phi = 0;
   d = 280;
   centerX = 20;
   centerZ = 80;
   centerY = 30;
   deltaY = 0;
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));

}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
guiIO::~guiIO()
{
   delete(gui);
}

/****************************************************************
 *                       Open File Window                       *
 ****************************************************************/
void guiIO::openFileWindow()
{
   fileWindow = gui->ljan->InserirJanela(0,0,255,63,"File",1,1,NULL,NULL);
   newButton = fileWindow->objects->InserirBotao(45,37,85,55,
                                                  fileWindow->Cores.corBot.R,
                                                  fileWindow->Cores.corBot.G,
                                                  fileWindow->Cores.corBot.B,
                                                  "New",1,NULL);
   openButton = fileWindow->objects->InserirBotao(86,37,126,55,
                                                  fileWindow->Cores.corBot.R,
                                                  fileWindow->Cores.corBot.G,
                                                  fileWindow->Cores.corBot.B,
                                                  "Open",1,NULL);
   saveButton = fileWindow->objects->InserirBotao(127,37,167,55,
                                                  fileWindow->Cores.corBot.R,
                                                  fileWindow->Cores.corBot.G,
                                                  fileWindow->Cores.corBot.B,
                                                  "Save",1,NULL);
   exitButton = fileWindow->objects->InserirBotao(168,37,208,55,
                                                  fileWindow->Cores.corBot.R,
                                                  fileWindow->Cores.corBot.G,
                                                  fileWindow->Cores.corBot.B,
                                                  "Exit",1,NULL);
   fileText = fileWindow->objects->InserirBarraTexto(10,17,245,33,
                                                     "../data/ia/",0,NULL);
   fileWindow->fechavel = 0;
   fileWindow->ptrExterno = &fileWindow;
   fileWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Nav Window                        *
 ****************************************************************/
void guiIO::openNavWindow()
{
   navWindow = gui->ljan->InserirJanela(799-63,599-63,799,599,"Nav",1,1,
                                        NULL,NULL);
   navTabButton = navWindow->objects->InserirTabButton(6,15,0,0,
                                                    "../data/iaEditor/nav.png");
   moreZoomButton = navTabButton->insertButton(0,0,8,8);    /* More Zoom */
   lessZoomButton = navTabButton->insertButton(9,0,17,8);   /* Less Zoom */
   upButton = navTabButton->insertButton(19,0,31,17);       /* Up */
   downButton = navTabButton->insertButton(19,27,31,44);    /* Down */
   leftButton = navTabButton->insertButton(6,16,23,28);     /* Left */
   rightButton = navTabButton->insertButton(27,16,44,28);   /* Right */
   rotUpButton = navTabButton->insertButton(45,0,53,11);    /* Rotation Up */
   rotDownButton = navTabButton->insertButton(45,13,53,24); /* Rotation Down */
   rotLeftButton = navTabButton->insertButton(40,36,50,50); /* Rotation Left */
   rotRightButton = navTabButton->insertButton(0,36,10,50); /* Rotation Right */
   navWindow->fechavel = 0;
   navWindow->ptrExterno = &navWindow;
   navWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Message Window                    *
 ****************************************************************/
void guiIO::openMessageWindow()
{
   messageWindow = gui->ljan->InserirJanela(0,599-31,511,599,
                                            "Messages",1,1,NULL,NULL);
   messageText = messageWindow->objects->InserirQuadroTexto(7,16,610,31,0,
                 "Welcome to DccNiTghtmare IA Editor / Tester!");
   messageWindow->fechavel = 0;
   messageWindow->ptrExterno = &messageWindow;
   messageWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open Objects Window                    *
 ****************************************************************/
void guiIO::openObjectsWindow()
{
   objectsWindow=gui->ljan->InserirJanela(0,599-95,127,599-32,"Objects",
                                          1,1,NULL,NULL);
   objectsTabButton = objectsWindow->objects->InserirTabButton(7,17,0,0,
                                                 "../data/iaEditor/objects.png");
   potentialButton = objectsTabButton->insertButton(0,0,19,19);
   patternButton = objectsTabButton->insertButton(20,0,39,19);
   goalButton = objectsTabButton->insertButton(0,20,19,39);
   objectButton = objectsTabButton->insertButton(20,20,39,39);
   objectsWindow->fechavel = 0;
   objectsWindow->ptrExterno = &objectsWindow;
   objectsWindow->Abrir(gui->ljan);
}

/****************************************************************
 *                       Open TP3 Window                        *
 ****************************************************************/
void guiIO::openTp3Window()
{
   tp3Window=gui->ljan->InserirJanela(0,599-222,127,599-158,"TP3",
                                          1,1,NULL,NULL);
   tp3TabButton = tp3Window->objects->InserirTabButton(7,17,0,0,
                                                 "../data/iaEditor/tp3.png");
   tp3Window->fechavel = 0;
   tp3Window->ptrExterno = &objectsWindow;
   tp3Window->Abrir(gui->ljan);
}

/****************************************************************
 *                     Open PlayControl Window                  *
 ****************************************************************/
void guiIO::openPlayControlWindow()
{
   playControlWindow=gui->ljan->InserirJanela(0,599-159,127,599-96,"Sim",
                                          1,1,NULL,NULL);
   playControlTabButton = playControlWindow->objects->InserirTabButton(27,17,0,0,
                                             "../data/iaEditor/playControl.png");
   playButton = playControlTabButton->insertButton(5,5,21,34);
   pauseButton = playControlTabButton->insertButton(29,7,38,32);
   stopButton = playControlTabButton->insertButton(46,9,65,30);
   playControlWindow->fechavel = 0;
   playControlWindow->ptrExterno = &playControlWindow;
   playControlWindow->Abrir(gui->ljan);
}


/****************************************************************
 *                           getState                           *
 ****************************************************************/
int guiIO::getState()
{
   return(state);
}

/****************************************************************
 *                           getTool                            *
 ****************************************************************/
int guiIO::getTool()
{
   return(tool);
}


/****************************************************************
 *                             Draw                             *
 ****************************************************************/
void guiIO::draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);
   gui->draw(proj,modl,viewPort);
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
}


/****************************************************************
 *                             DoIO                             *
 ****************************************************************/
int guiIO::doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys)
{
   int eventInfo;
   Tobjeto* object;
   object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, &eventInfo);
   switch(eventInfo)
   {
      case TABBOTAOPRESSIONADO:
      {
         /*  Navigation Buttons  */
         if(object == (Tobjeto*) upButton)
         {
            centerX -= 4.0 * sin(deg2Rad(phi));
            centerZ -= 4.0 * cos(deg2Rad(phi));
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) downButton)
         {
            centerX += 4.0 * sin(deg2Rad(phi));
            centerZ += 4.0 * cos(deg2Rad(phi));
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) leftButton)
         {
            centerX -= 4.0 * sin(deg2Rad(phi)+deg2Rad(90));
            centerZ -= 4.0 * cos(deg2Rad(phi)+deg2Rad(90));
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rightButton)
         {
            centerX += 4.0 * sin(deg2Rad(phi)+deg2Rad(90));
            centerZ += 4.0 * cos(deg2Rad(phi)+deg2Rad(90));
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rotUpButton)
         {
            theta += 1;
            if(theta > 89) 
               theta = 89;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rotDownButton)
         {
            theta -= 1;
            if(theta < 0)
               theta = 0;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rotLeftButton)
         {
            phi += 1;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) rotRightButton)
         {
            phi -= 1;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) moreZoomButton)
         {
            d -= 1;
            if (d<1) d = 1;
            return(GUI_IO_NEW_POSITION);
         }
         else if (object == (Tobjeto*) lessZoomButton)
         {
            d += 1;
            if (d>300) d = 300;
            return(GUI_IO_NEW_POSITION);
         }
         /* PlayControl Events */
         else if (object == (Tobjeto*) playButton)
         {
            tool = TOOL_SIM_PLAY;
            return(GUI_IO_NEW_STATE);
         }
         else if (object == (Tobjeto*) pauseButton)
         {
            tool = TOOL_SIM_PAUSE;
            return(GUI_IO_NEW_STATE);
         }
         else if (object == (Tobjeto*) stopButton)
         {
            tool = TOOL_SIM_STOP;
            return(GUI_IO_NEW_STATE);
         }
         /* Objects Window Events */
         else if (object == (Tobjeto*) potentialButton)
         {
            tool = TOOL_POTENTIAL_ADD;
            return(GUI_IO_NEW_STATE);
         }
         else if (object == (Tobjeto*) patternButton)
         {
            tool = TOOL_PATTERN_ADD;
            return(GUI_IO_NEW_STATE);
         }
         else if (object == (Tobjeto*) goalButton)
         {
            tool = TOOL_GOAL_ADD;
            return(GUI_IO_NEW_STATE);
         }
         else if (object == (Tobjeto*) objectButton)
         {
            tool = TOOL_OBSTACLE_ADD;
            return(GUI_IO_NEW_STATE);
         }
      }
      case BOTAOPRESSIONADO:
      {
         if(object == (Tobjeto*) exitButton)
         {
            return(GUI_IO_EXIT);
         }
         else if(object == (Tobjeto*) newButton)
         {
            return(GUI_IO_NEW_IAFILE);
         }
         else if(object == (Tobjeto*) openButton)
         {
            return(GUI_IO_OPEN_IAFILE);
         }
         else if(object == (Tobjeto*) saveButton)
         {
            return(GUI_IO_SAVE_IAFILE);
         }
         
         break;
      }
   }
   if(eventInfo == NADA)
   {
      return(GUI_IO_NOTHING);
   }
   else
   {
      return(GUI_IO_OTHER);
   }
}

/****************************************************************
 *                             camera                           *
 ****************************************************************/
void guiIO::camera()
{
   cameraX = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
   cameraY = centerY + deltaY + (float) d * sin(deg2Rad(theta));
   cameraZ = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
   gluLookAt(cameraX,cameraY,cameraZ, centerX,centerY,centerZ,0,1,0);
}

/****************************************************************
 *                        getFileName                           *
 ****************************************************************/
string guiIO::getFileName()
{
   return(fileText->texto);
}

/****************************************************************
 *                        Show Message                          *
 ****************************************************************/
void guiIO::showMessage(string msg)
{
   messageText->texto = msg;
   messageWindow->Desenhar(0,0);
}

