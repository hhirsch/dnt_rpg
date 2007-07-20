/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "inventwindow.h"

#define INVENTORY_STATE_NONE   0
#define INVENTORY_STATE_OBJECT 1
#define INVENTORY_STATE_MENU   2

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
inventWindow::inventWindow(inventory *invent, interface* inter)
{
   objectMenu = NULL;
   previousCursor = NULL;

   /* Copy Interface Pointer */
   interf = inter;

   /* Copy Inventories Pointers */
   inventories = invent;

   /* Add Window */
   intWindow = inter->insertWindow(0,0,263,402,language.INVENTW_TITLE.c_str());

   /* Add Character (equip) Tab Button */
   characterTabButton = intWindow->getObjectsList()->insertTabButton(4,15,
                                                                     256,256,
                                        "../data/texturas/inventory/equip.png");
   headButton = characterTabButton->insertButton(109,3,147,41);
   footButton = characterTabButton->insertButton(109,214,147,252);
   bodyButton = characterTabButton->insertButton(99,100,156,176);
   rightHandButton = characterTabButton->insertButton(58,112,96,188);
   leftHandButton = characterTabButton->insertButton(159,112,197,188);
   neckButton = characterTabButton->insertButton(159,77,197,96);
   rightFingerButton = characterTabButton->insertButton(36,169,55,188);
   leftFingerButton = characterTabButton->insertButton(200,169,219,188);

   /* Add Inventory Tab Button */
   inventoryTabButton = intWindow->getObjectsList()->insertTabButton(4,271,
                                                                     256,128,
                                   "../data/texturas/inventory/inventory.png");
   inventoryButton = inventoryTabButton->insertButton(4,13,251,127);
   inv1Button = inventoryTabButton->insertButton(4,0,64,13);
   inv2Button = inventoryTabButton->insertButton(64,0,127,13);
   inv3Button = inventoryTabButton->insertButton(127,0,191,13);
   inv4Button = inventoryTabButton->insertButton(191,0,251,13);

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   inter->openWindow(intWindow);

   currentInventory = 0;
   reDraw();

   state = INVENTORY_STATE_NONE;
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
inventWindow::~inventWindow()
{
   if(activeObject)
   {
      if(state == INVENTORY_STATE_OBJECT)
      {
         /* Reput the object to the inventory */
         if(!inventories->addObject(activeObject))
         {
            activeObject = NULL;
            state = INVENTORY_STATE_NONE;
         }
      }
   }
   if(intWindow)
   {
      interf->closeWindow(intWindow);
   }
}

/**************************************************************
 *                            isOpen                          *
 **************************************************************/
bool inventWindow::isOpen()
{
   return(intWindow != NULL);
}

/**************************************************************
 *                            reDraw                          *
 **************************************************************/
void inventWindow::reDraw()
{
   if(isOpen())
   {
      inventories->draw(0,0, inventoryTabButton->get(), currentInventory);
      inventories->drawEquiped(0,0,characterTabButton->get());
      intWindow->draw(0,0);

      //TODO Mark on actual Inventory!
   }
}

/**************************************************************
 *                          openMenu                          *
 **************************************************************/
void inventWindow::openMenu(int x, int y, int type)
{
   int xSize;
   menuType = type;
   objectMenu = (menu*) intWindow->getObjectsList()->addMenu();
   objectMenu->insertItem(language.INVENTW_DROP,0);
   objectMenu->insertItem("-",0);
   objectMenu->insertItem(language.INVENTW_SELL,0);
   if(menuType == MENU_TYPE_INVENTORY)
   {
      objectMenu->insertItem(language.INVENTW_USE,1);
   }
   else
   {
      objectMenu->insertItem("Unequip",1);
   }
   objectMenu->insertItem("-",0);
   objectMenu->insertItem("Info",0);
   objectMenu->insertItem("-",0);
   objectMenu->insertItem(language.INVENTW_GET,1);

   defineFont(FFARSO,ALIGN_LEFT,1);

   xSize = objectMenu->getMaxCharac()*(font_incCP()+1)+6;

   /* Make Sure all Menu is in Window */
   if(y+94 >= intWindow->getY2())
   {
       y = intWindow->getY2()-94;
   }

   if(x+xSize >= intWindow->getX2())
   {
      x = intWindow->getX2() - xSize;
   }
   
   objectMenu->setPosition(x,y);
   
   state = INVENTORY_STATE_MENU;
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
bool inventWindow::treat(guiObject* guiObj, int eventInfo, cursor* mouseCursor)
{
   if(!isOpen())
   {
      /* Window is no more opened */
      return(false);
   }
   int x,y;
   Uint8 Mbotao = SDL_GetMouseState(&x,&y);

   if((state == INVENTORY_STATE_OBJECT) && (activeObject))
   {
      if(mouseCursor->getActual() != activeObject->get2dModel())
      {
         previousCursor = mouseCursor->getActual();
      }
      mouseCursor->setActual(activeObject->get2dModel());

      if( x < intWindow->getX1())
      {
         x = intWindow->getX1();
      }
      else if(x > (intWindow->getX2() - activeObject->get2dModel()->w))
      {
         x = intWindow->getX2() - activeObject->get2dModel()->w;
      }
      if( y < intWindow->getY1())
      {
         y = intWindow->getY1();
      }
      else if(y > (intWindow->getY2() -activeObject->get2dModel()->h) )
      {
         y = intWindow->getY2() - activeObject->get2dModel()->h;
      }
      SDL_WarpMouse(x,y);
   }
   else
   {
      if(previousCursor)
      {
         mouseCursor->setActual(previousCursor);
         previousCursor = NULL;
      }
   }


   int posX = (int) floor((x - (8 + intWindow->getX1())) / (19.0));
   int posY = (int) floor((y - (284 + intWindow->getY1())) / (19.0));

   if(Mbotao & SDL_BUTTON(3))
   {
      if( (state == INVENTORY_STATE_OBJECT) && (activeObject) )
      {
         /* Reput the object to the inventory */
         if(inventories->addObject(activeObject)) 
         {
            activeObject = NULL;
            state = INVENTORY_STATE_NONE;
            reDraw();
            return(true);
         }
      }
   }
   
   switch(eventInfo)
   {
      case PRESSED_TAB_BUTTON:
      {
         /* Inventory Spaces Selected */
         if(guiObj == (guiObject*) inventoryButton)
         {
            if(state == INVENTORY_STATE_NONE)
            {
               /* Open Menu For Object if one is avaible */
               if(inventories->getFromPosition(posX, posY,currentInventory))
               {
                  activeObject = inventories->getFromPosition(posX, posY,
                                                              currentInventory);
                  objX = posX;
                  objY = posY;
                  objWhere = INVENTORY_INVENTORY;
                  openMenu((x-intWindow->getX1()),(y-intWindow->getY1()),
                           MENU_TYPE_INVENTORY); 
               }
               return(true);
            }
            else if(state == INVENTORY_STATE_OBJECT)
            {
               /* Put Object on  */
               if(inventories->addObject(activeObject, posX, posY, 
                                         currentInventory))
               {
                  activeObject = NULL;
                  state = INVENTORY_STATE_NONE;
                  reDraw();
               }
               return(true);
            }
         }
         
         /* Change to Inventory 1 */
         else if(guiObj == (guiObject*) inv1Button)
         {
            currentInventory = 0;
            reDraw();
            return(true);
         }
         /* Change to Inventory 2 */
         else if(guiObj == (guiObject*) inv2Button)
         {
            currentInventory = 1;
            reDraw();
            return(true);
         }
         /* Change to Inventory 3 */
         else if(guiObj == (guiObject*) inv3Button)
         {
            currentInventory = 2;
            reDraw();
            return(true);
         }
         /* Change to Inventory 4 */
         else if(guiObj == (guiObject*) inv4Button)
         {
            currentInventory = 3;
            reDraw();
            return(true);
         } 
         
         if(state == INVENTORY_STATE_NONE)
         {
            object* aObject = NULL;
            if(guiObj == (guiObject*) headButton)
            {
               aObject = inventories->getFromPlace(INVENTORY_HEAD);
               objWhere = INVENTORY_HEAD;
            }
            else if(guiObj == (guiObject*) leftHandButton)
            {
               aObject = inventories->getFromPlace(INVENTORY_LEFT_HAND);
               objWhere = INVENTORY_LEFT_HAND;
            }
            else if(guiObj == (guiObject*) rightHandButton)
            {
               aObject = inventories->getFromPlace(INVENTORY_RIGHT_HAND);
               objWhere = INVENTORY_RIGHT_HAND;
            }
            else if(guiObj == (guiObject*) leftFingerButton)
            {
               aObject = inventories->getFromPlace(INVENTORY_LEFT_FINGER);
               objWhere = INVENTORY_LEFT_FINGER;
            }
            else if(guiObj == (guiObject*) rightFingerButton)
            {
               aObject = inventories->getFromPlace(INVENTORY_RIGHT_FINGER);
               objWhere = INVENTORY_RIGHT_FINGER;
            }
            else if(guiObj == (guiObject*) neckButton)
            {
               aObject = inventories->getFromPlace(INVENTORY_NECK);
               objWhere = INVENTORY_NECK;
            }
            else if(guiObj == (guiObject*) footButton)
            {
               aObject = inventories->getFromPlace(INVENTORY_FOOT);
               objWhere = INVENTORY_FOOT;
            }
            else if(guiObj == (guiObject*) bodyButton)
            {
               aObject = inventories->getFromPlace(INVENTORY_BODY);
               objWhere = INVENTORY_BODY;
            }

            if(aObject)
            {
               activeObject = aObject;
               openMenu((x-intWindow->getX1()),(y-intWindow->getY1()),
                        MENU_TYPE_EQUIPED);
               state = INVENTORY_STATE_MENU;
               return(true);
            }
         }
         /* State Object */
         else if(state == INVENTORY_STATE_OBJECT)
         {
            if(guiObj == (guiObject*) headButton)
            {
               if(inventories->equipObject(activeObject, INVENTORY_HEAD))
               {
                  activeObject = NULL;
               }
            }
            else if(guiObj == (guiObject*) leftHandButton)
            {
               if(inventories->equipObject(activeObject,INVENTORY_LEFT_HAND))
               {
                  activeObject = NULL;
               }

            }
            else if(guiObj == (guiObject*) rightHandButton)
            {
               if(inventories->equipObject(activeObject,INVENTORY_RIGHT_HAND))
               {
                  activeObject = NULL;
               }
            }
            else if(guiObj == (guiObject*) leftFingerButton)
            {
               if(inventories->equipObject(activeObject,INVENTORY_LEFT_FINGER))
               {
                  activeObject = NULL;
               }
            }
            else if(guiObj == (guiObject*) rightFingerButton)
            {
               if(inventories->equipObject(activeObject,INVENTORY_RIGHT_FINGER))
               {
                  activeObject = NULL;
               }
            }
            else if(guiObj == (guiObject*) neckButton)
            {
               if(inventories->equipObject(activeObject, INVENTORY_NECK))
               {
                  activeObject = NULL;
               }
            }
            else if(guiObj == (guiObject*) footButton)
            {
               if(inventories->equipObject(activeObject, INVENTORY_FOOT))
               {
                  activeObject = NULL;
               }
            }
            else if(guiObj == (guiObject*) bodyButton)
            {
               if(inventories->equipObject(activeObject, INVENTORY_BODY))
               {
                  activeObject = NULL;
               }
            }

            /* if no more getObjectsList(), done */
            if(!activeObject)
            {
               state = INVENTORY_STATE_NONE;
               reDraw();
               return(true);
            }
         }
      }
      break;
      
      case SELECTED_MENU:
      {
         if(objectMenu)
         {
            state = INVENTORY_STATE_NONE;
            switch(objectMenu->getActualItem())
            {
               case 1: /* Get */
                  if(objWhere == INVENTORY_INVENTORY)
                  {
                    inventories->removeFromInventory(objX,objY, 
                                                     currentInventory);
                    reDraw();
                  }
                  else
                  {
                     inventories->removeFromPlace(objWhere);
                     reDraw();
                  }
                  state = INVENTORY_STATE_OBJECT;
               break;
               case 3: /* Info */
               break;
               case 5: /* Use */
               {
                  if(objWhere == INVENTORY_INVENTORY)
                  {
                     if(activeObject->getType() == OBJECT_TYPE_WEAPON)
                     {
                        /* Equip Weapon */
                        inventories->removeFromInventory(objX,objY, 
                                                         currentInventory);
                        if(!inventories->equipObject(activeObject, 
                                                     INVENTORY_RIGHT_HAND))
                        {
                           /* Can't equip the weapon, so back on inventory */
                           inventories->addObject(activeObject,
                                                  objX,objY,currentInventory);
                        }
                        activeObject = NULL;
                        reDraw();
                     }
                  }
                  else
                  {
                     /* Unequip */
                     inventories->removeFromPlace(objWhere);
                     if(!inventories->addObject(activeObject))
                     {
                        /* Inventory is Full, so not remove from used place */
                        inventories->equipObject(activeObject, objWhere);
                     }
                     activeObject = NULL;
                     reDraw();
                  }
                  if(!activeObject)
                  {
                     state = INVENTORY_STATE_NONE;
                  }
               }
               break;
               case 6: /* Sell */
                  //TODO
               break;
               case 8: /* Drop */
                  //TODO
               break;
            }
            intWindow->getObjectsList()->removeMenu();
            objectMenu = NULL;
            return(true);
         }
      }
      break;
   }
   return(false);
}


