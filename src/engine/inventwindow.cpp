/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "inventwindow.h"
#include "barterwindow.h"

#include "../classes/actions.h"
#include "../etc/dirs.h"
#include "../gui/dntfont.h"
#include "../sound/sound.h"



#define INVENTORY_STATE_NONE   0
#define INVENTORY_STATE_OBJECT 1
#define INVENTORY_STATE_MENU   2

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
inventWindow::inventWindow(int xa, int ya, string title, 
                           character* invent, guiInterface* inter,
                           itemWindow* infoW)
{
   init(xa,ya,title,invent,inter, infoW);
}

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
inventWindow::inventWindow(character *invent, guiInterface* inter,
                           itemWindow* infoW)
{
   init(0,1, gettext("Inventory"), invent, inter, infoW);
}

/**************************************************************
 *                              init                          *
 **************************************************************/
void inventWindow::init(int xa, int ya, string title, 
                       character *invent,guiInterface* inter,
                       itemWindow* infoW)
{
   dirs dir;
   objectMenu = NULL;
   previousCursor = -1;

   /* Copy Interface Pointer */
   interf = inter;

   /* Copy ItemWindow Pointer */
   infoWindow = infoW;

   /* Copy Inventories Pointers */
   owner = invent;
   inventories = owner->inventories;
   inventories->setOpenedWindow(this);

   /* Add Window */
   intWindow = inter->insertWindow(xa,ya,xa+266,ya+407,title.c_str());

   /* Add Character (equip) Tab Button */
   characterTabButton = intWindow->getObjectsList()->insertTabButton(4,15,
                                                                     256,256,
                       dir.getRealFile("texturas/inventory/equip.png").c_str());
   headButton = characterTabButton->insertButton(109,3,147,41);
   footButton = characterTabButton->insertButton(109,214,147,252);
   bodyButton = characterTabButton->insertButton(99,100,156,176);
   rightHandButton = characterTabButton->insertButton(58,112,96,188);
   leftHandButton = characterTabButton->insertButton(159,112,197,188);
   neckButton = characterTabButton->insertButton(159,77,197,96);
   rightFingerButton = characterTabButton->insertButton(36,169,55,188);
   leftFingerButton = characterTabButton->insertButton(200,169,219,188);

   /* Add Inventory Tab Button */
   inventoryTabButton = intWindow->getObjectsList()->insertTabButton(4,272,
                                                                     256,128,
                   dir.getRealFile("texturas/inventory/inventory.png").c_str());
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
   inventories->setOpenedWindow(NULL);
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
   int i;
   dntFont fnt;
   int x = 32;
   char buf[8];
   farso_colors colors;
   fnt.defineFont(DNT_FONT_TIMES, 10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
   if(isOpen())
   {
      inventories->draw(0,0, inventoryTabButton->get(), currentInventory);
      inventories->drawEquiped(0,0,characterTabButton->get());

      /* Write Inventory Numbers */
      for(i=0; i < 4; i++)
      {
         if(currentInventory == i)
         {
            color_Set(colors.colorText.R,
                      colors.colorText.G,
                      colors.colorText.B,
                      colors.colorText.A);
         }
         else
         {
            color_Set(colors.colorCont[1].R,
                      colors.colorCont[1].G,
                      colors.colorCont[1].B,
                      colors.colorCont[1].A);
         }
         sprintf(buf,"%d", i);
         fnt.write(inventoryTabButton->get(), x, 1, buf, true);
         x += 61;
      }
      intWindow->draw(0,0);

   }
}

/**************************************************************
 *                          openMenu                          *
 **************************************************************/
void inventWindow::openMenu(int x, int y, int type, bool seller)
{
   barterWindow tradeWindow;
   dntFont fnt;
   dirs dir;
   int xSize;
   menuType = type;
   objectMenu = (menu*) intWindow->getObjectsList()->addMenu();
   objectMenu->insertItem(gettext("Drop"),
                          dir.getRealFile("icons/drop.png"),
                          (menuType == MENU_TYPE_INVENTORY) && 
                          (!seller) );

   objectMenu->insertItem("-",0);

   objectMenu->insertItem(seller ? gettext("Buy") : gettext("Sell"), 
                          dir.getRealFile("icons/sell.png"),
                          tradeWindow.isOpen() && 
                          (menuType == MENU_TYPE_INVENTORY));

   if(menuType == MENU_TYPE_INVENTORY)
   {
      objectMenu->insertItem(gettext("Use"),
                             dir.getRealFile("icons/use.png"), 
                             (!seller) && (activeObject->isUsable()) );
   }
   else
   {
      objectMenu->insertItem(gettext("Remove"),
                             dir.getRealFile("icons/remove.png"), !seller);
   }
   objectMenu->insertItem("-",0);
   objectMenu->insertItem(gettext("Info"),
                          dir.getRealFile("icons/info.png"), 1);
   objectMenu->insertItem("-",0);

   objectMenu->insertItem(gettext("Get"),
                          dir.getRealFile("icons/get.png"), !seller);

   fnt.defineFont(DNT_FONT_ARIAL,12);

   xSize = objectMenu->getMaxCharac()*(fnt.getIncCP()+1)+6;

   /* Make Sure all Menu is in Window */
   int menuHeight = 8*(MENU_ITEM_HEIGHT+1);
   if( (y + menuHeight) >= (intWindow->getY2() - intWindow->getY1()) )
   {
       y = intWindow->getY2()-intWindow->getY1()-(menuHeight);
   }

   if( (x + xSize) >= (intWindow->getX2() - intWindow->getX1()) )
   {
      x = intWindow->getX2() - intWindow->getX1()  - xSize;
   }
   
   objectMenu->setPosition(x,y);
   
   state = INVENTORY_STATE_MENU;
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
bool inventWindow::treat(guiObject* guiObj, int eventInfo, cursor* mouseCursor,
                         Map* actualMap, GLfloat X, GLfloat Z, bool seller)
{
   modState modifState;

   sound snd;
   if(!isOpen())
   {
      /* Window is no more opened */
      return(false);
   }
   int x,y;
   Uint8 Mbotao = SDL_GetMouseState(&x,&y);

   if((state == INVENTORY_STATE_OBJECT) && (activeObject))
   {
      if(mouseCursor->get() != CURSOR_USER_IMAGE)
      {
         previousCursor = mouseCursor->get();
      }
      mouseCursor->set(activeObject->get2dModel());

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
      if(previousCursor != -1)
      {
         mouseCursor->set(previousCursor);
         previousCursor = -1;
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
                           MENU_TYPE_INVENTORY, seller); 
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
                        MENU_TYPE_EQUIPED, seller);
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
     
      /* Verify Menu Options */
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
                  if(infoWindow)
                  {
                     infoWindow->open(activeObject);
                  }
               break;
               case 5: /* Use */
               {
                  if(objWhere == INVENTORY_INVENTORY)
                  {
                     /* Equip an weapon (if possible) */
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

                     /* Use a heal object */
                     else if(activeObject->getType() == OBJECT_TYPE_HEAL)
                     {
                        /* TODO -> get the heal target (currently 
                         * doing always on the inventory owner) */
                        character* target = owner;
                        
                        /* Apply the heal to the target 
                         * FIXME: set the range! */
                        doHealOrAttack(*owner, *target, 
                                       activeObject->getDiceInfo(),
                                       NULL, 20, NULL, true);

                        /* And discard the object */
                        inventories->removeFromInventory(objX,objY, 
                                                         currentInventory);
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
               case 6: /* Buy /  Sell */
               {
                  barterWindow tradeWindow;

                  if(activeObject)
                  {
                     if(seller)
                     {
                        /* The Pc is buying from the seller */
                        tradeWindow.addBuyItem(activeObject);
                     }
                     else
                     {
                        /* The PC is selling to the seller */
                        tradeWindow.addSellItem(activeObject);
                     }
                     inventories->removeFromInventory(objX,objY, 
                           currentInventory);
                     reDraw();
                     activeObject = NULL;
                  }
               }
               break;
               case 8: /* Drop */
                  if(objWhere == INVENTORY_INVENTORY)
                  {
                     /* Only can drop if it is on the inventory */
                     inventories->removeFromInventory(objX,objY, 
                                                      currentInventory);
                     /* Add it to the map */
                     actualMap->insertObject(X, actualMap->getHeight(X,Z), Z,
                                             0, activeObject, 0);
                     modifState.mapObjectAddAction(MODSTATE_ACTION_OBJECT_ADD,
                                                   activeObject->getFileName(),
                                                   actualMap->getFileName(),
                                                   X,Z);
                     /* Return to the NONE state */
                     activeObject = NULL;
                     state = INVENTORY_STATE_NONE;
                     reDraw();
                     /* Play Drop Sound */
                     snd.addSoundEffect(X, actualMap->getHeight(X,Z), Z,
                                        SOUND_NO_LOOP,
                                        "sndfx/objects/drop_item.ogg");
                  }
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


