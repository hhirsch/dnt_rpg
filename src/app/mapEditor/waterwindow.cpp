#include "waterwindow.h"


/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
waterWindow::waterWindow(interface* actualGui)
{
   gui = actualGui;
   activeWater = NULL;
   activePlane = NULL;
   intWindow = NULL;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
waterWindow::~waterWindow()
{
}

/***********************************************************************
 *                              setWater                               *
 ***********************************************************************/
void waterWindow::setWater(part1* water)
{
   activeWater = water;
   if(!intWindow)
   {
      openWindow();
   }
}

/***********************************************************************
 *                              setPlane                               *
 ***********************************************************************/
void waterWindow::setPlane(interPlane* pl)
{
   activePlane = pl;
   if(!intWindow)
   {
      openWindow();
   }
}

/***********************************************************************
 *                              setPlane                               *
 ***********************************************************************/
void waterWindow::setPartSystem(partSystem* pS) 
{
   pSystem = pS;
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void waterWindow::openWindow()
{
   intWindow = gui->insertWindow(403,599-247,653,599-37,"Waterfall");

   /* Waterfall Navigators */
   intWindow->getObjectsList()->insertTextBox(15,17,77,36,0,"Waterfall");
   waterTab = intWindow->getObjectsList()->insertTabButton(6,30,0,0,
                                                 "../data/mapEditor/nav2.png");
   waterMoveTab = intWindow->getObjectsList()->insertTabButton(67,30,0,0,
                                           "../data/mapEditor/water_move.png");
   buttonPreviousWater = waterTab->insertButton(0,0,19,19);
   buttonDestroyWater = waterTab->insertButton(20,0,39,19);
   buttonNextWater = waterTab->insertButton(40,0,59,19);
   buttonMoveWaterUp = waterMoveTab->insertButton(0,0,19,9);
   buttonMoveWaterDown = waterMoveTab->insertButton(0,10,19,19);
   intWindow->getObjectsList()->insertTextBox(4,16,90,52,1,"");

   /* Plane Navigators */
   intWindow->getObjectsList()->insertTextBox(112,17,152,36,0,"Planes");
   planesTab = intWindow->getObjectsList()->insertTabButton(94,30,0,0,
                                           "../data/mapEditor/plane_nav.png");
   buttonAddPlane = planesTab->insertButton(0,0,19,19);
   buttonPreviousPlane = planesTab->insertButton(20,0,39,19);
   buttonDestroyPlane = planesTab->insertButton(40,0,59,19);
   buttonNextPlane = planesTab->insertButton(60,0,79,19);
   intWindow->getObjectsList()->insertTextBox(91,16,177,52,1,"");

   /* Plane Inclination */
   intWindow->getObjectsList()->insertTextBox(181,17,245,36,0,"Plane Inc");
   incTypeTab = intWindow->getObjectsList()->insertTabButton(181,30,0,0,
                                     "../data/mapEditor/plane_inclination.png");
   buttonNoInclination = incTypeTab->insertButton(0,0,19,19);
   buttonInclinationX = incTypeTab->insertButton(20,0,39,19);
   buttonInclinationZ = incTypeTab->insertButton(40,0,59,19);
   intWindow->getObjectsList()->insertTextBox(178,16,246,52,1,"");

   /* Plane Move X_Z */
   intWindow->getObjectsList()->insertTextBox(5,55,45,68,0,"Plane  Move");
   movXZTab = intWindow->getObjectsList()->insertTabButton(5,78,0,0,
                                          "../data/mapEditor/plane_move.png");
   buttonMoveZMore = movXZTab->insertButton(14,0,26,14);
   buttonMoveZLess = movXZTab->insertButton(14,24,26,39);
   buttonMoveXLess = movXZTab->insertButton(2,12,15,26);
   buttonMoveXMore = movXZTab->insertButton(25,12,38,26);
   intWindow->getObjectsList()->insertTextBox(3,53,48,120,1,"");

   /* Plane Move Y */
   intWindow->getObjectsList()->insertTextBox(10,123,65,136,0,"Plane y");
   movYTab = intWindow->getObjectsList()->insertTabButton(6,136,0,0,
                                          "../data/mapEditor/plane_move_y.png");
   buttonMoveYMore = movYTab->insertButton(5,1,15,15);
   buttonMoveYLess = movYTab->insertButton(5,24,15,38);
   buttonMoveY1More = movYTab->insertButton(20,1,30,15);
   buttonMoveY1Less = movYTab->insertButton(20,24,30,38);
   buttonMoveY2More = movYTab->insertButton(47,1,57,15);
   buttonMoveY2Less = movYTab->insertButton(47,24,57,38);
   intWindow->getObjectsList()->insertTextBox(3,121,69,180,1,"");

   /* Plane Scale */
   intWindow->getObjectsList()->insertTextBox(75,123,130,136,0,"P Scale");
   scaleTab = intWindow->getObjectsList()->insertTabButton(73,136,0,0,
                                          "../data/mapEditor/plane_scale.png");
   buttonScaleXMore = scaleTab->insertButton(0,0,19,19);
   buttonScaleXLess = scaleTab->insertButton(0,20,19,39);
   buttonScaleYMore = scaleTab->insertButton(20,0,39,19);
   buttonScaleYLess = scaleTab->insertButton(20,20,39,39);
   buttonScaleZMore = scaleTab->insertButton(40,0,59,19);
   buttonScaleZLess = scaleTab->insertButton(40,20,59,39);
   intWindow->getObjectsList()->insertTextBox(70,121,138,180,1,"");

   /* Waterflow Direction */
   intWindow->getObjectsList()->insertTextBox(110,55,245,68,0,
                                              "Plane Waterflow");
   intWindow->getObjectsList()->insertTextBox(50,73,100,88,0,"deltaX:");
   dxWaterflowLess = intWindow->getObjectsList()->insertButton(101,71,111,88,
                                                               "<",0);
   dxWaterflowText = intWindow->getObjectsList()->insertTextBox(112,71,162,88,
                                                                 1,"1.0");
   dxWaterflowMore = intWindow->getObjectsList()->insertButton(163,71,173,88,
                                                               ">",0);
   intWindow->getObjectsList()->insertTextBox(50,93,100,108,0,"deltaZ:");
   dzWaterflowLess = intWindow->getObjectsList()->insertButton(101,91,111,108,
                                                               "<",0);
   dzWaterflowText = intWindow->getObjectsList()->insertTextBox(112,91,162,108,
                                                                 1,"1.0");
   dzWaterflowMore = intWindow->getObjectsList()->insertButton(163,91,173,108,
                                                               ">",0);
   intWindow->getObjectsList()->insertTextBox(49,53,246,120,1,"");

   /* Unused Space */
   intWindow->getObjectsList()->insertTextBox(139,121,246,180,1,"");

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   gui->openWindow(intWindow);
}

/***********************************************************************
 *                              eventGot                               *
 ***********************************************************************/
bool waterWindow::eventGot(int type, guiObject* object)
{
   //TODO
   return(false);
}

