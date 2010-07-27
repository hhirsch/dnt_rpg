/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "inventwindow.h"
#include "barterwindow.h"
#include "splitwindow.h"

#include "dccnit.h"

#include "../cbook/comicbook.h"
#include "../classes/actions.h"
#include "../classes/money.h"
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
                           itemWindow* infoW, void* usedEngine)
{
   init(xa,ya,title,invent,inter, infoW, usedEngine);
}

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
inventWindow::inventWindow(character *invent, guiInterface* inter,
                           itemWindow* infoW, void* usedEngine)
{
   init(0,1, gettext("Inventory"), invent, inter, infoW, usedEngine);
}

/**************************************************************
 *                              init                          *
 **************************************************************/
void inventWindow::init(int xa, int ya, string title, 
                       character *invent,guiInterface* inter,
                       itemWindow* infoW, void* usedEngine)
{
   int i;
   char buf[8];

   dirs dir;
   objectMenu = NULL;
   previousCursor = -1;

   /* Set engine */
   curEngine = usedEngine;

   /* Copy Interface Pointer */
   interf = inter;

   /* Copy ItemWindow Pointer */
   infoWindow = infoW;

   /* Copy Inventories Pointers */
   owner = invent;
   inventories = owner->inventories;
   inventories->setOpenedWindow(this);

   /* Add Window */
   intWindow = inter->insertWindow(xa,ya,xa+266,ya+415,title.c_str());

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

   /* Add Inventory Tab Box */
   inventoryTabBox = (tabBox*)intWindow->getObjectsList()->defineTabBox(6,272,
                                                                       258,409);

   /* Add Each Options */
   guiList* list;
   for(i = 4; i > 0; i--)
   {
      sprintf(buf, "%d", i);
      list = inventoryTabBox->insertOption(buf);
      inventoryTabButton[i-1] = list->insertTabButton(8,292,0,0,
                   dir.getRealFile("texturas/inventory/inventory.png").c_str());
      inventoryButton[i-1] = inventoryTabButton[i-1]->insertButton(0,0,
                                                                   247,114);
   }

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
   if(isOpen())
   {
      inventories->draw(0,0, inventoryTabButton[currentInventory]->get(), 
                        currentInventory);
      inventories->drawEquiped(0,0,characterTabButton->get());

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
   string text="";
   menuType = type;
   objectMenu = (menu*) intWindow->getObjectsList()->addMenu();
 
   /* Insert all menu items */
   objectMenu->insertItem(gettext("Get"),
                          dir.getRealFile("icons/get.png"), !seller);
   objectMenu->insertItem("-",0);
   objectMenu->insertItem(gettext("Info"),
                          dir.getRealFile("icons/info.png"), 1);
   objectMenu->insertItem("-",0);
   if(menuType == MENU_TYPE_INVENTORY)
   {
      switch(activeObject->getType())
      {
         case OBJECT_TYPE_BOOK:
            text = gettext("Read");
         break;
         case OBJECT_TYPE_AMMO:
            text = gettext("Reload Weapon");
         break;
         default:
            text = gettext("Use");
         break;
      }
      objectMenu->insertItem(text, dir.getRealFile("icons/use.png"), 
                             (!seller) && (activeObject->isUsable()) );
   }
   else
   {
      objectMenu->insertItem(gettext("Remove"),
                             dir.getRealFile("icons/remove.png"), !seller);
   }
   objectMenu->insertItem(seller ? gettext("Buy") : gettext("Sell"), 
                          dir.getRealFile("icons/sell.png"),
                          tradeWindow.isOpen() && 
                          (menuType == MENU_TYPE_INVENTORY));
   objectMenu->insertItem("-",0);
   objectMenu->insertItem(gettext("Drop"),
                          dir.getRealFile("icons/drop.png"),
                          (menuType == MENU_TYPE_INVENTORY) && 
                          (!seller) );

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
 *                      verifyUseObject                       *
 **************************************************************/
void inventWindow::verifyUseObject()
{
   int i; 
   char buf[1024];
   briefing brief;

   if(objWhere == INVENTORY_INVENTORY)
   {
      /* Equip an weapon (if possible) */
      if(activeObject->getType() == OBJECT_TYPE_WEAPON)
      {
         /* Equip Weapon */         
         if(inventories->equipObject(activeObject, INVENTORY_RIGHT_HAND))
         {
            /* Equipped, so must remove from inventory */
            inventories->removeFromInventory(objX,objY, currentInventory);
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
         factor healFactor;
         healFactor.id = "HEAL";
         healFactor.type = MOD_TYPE_SKILL;
         doHealOrAttack(*owner, target, 
               activeObject->getDiceInfo(),
               &healFactor, 20, true);

         /* And discard the object */
         inventories->removeFromInventory(objX,objY, 
               currentInventory);
         delete(activeObject);
         activeObject = NULL;
         reDraw();
      }

      /* Read a comic book */
      else if(activeObject->getType() == OBJECT_TYPE_BOOK)
      {
         /* Do the comic book run */
         comicBook *cb = new comicBook();
         if(cb->load(activeObject->getRelatedInfo()))
         {
            cb->run();
         }
         /* Clear it and done */
         delete(cb);
         activeObject = NULL;
      }

      /* Reload an weapon with ammo */
      else if(activeObject->getType() == OBJECT_TYPE_AMMO)
      {
         /* Verify if the ammo type is compatible with any in use weapon */
         bool used = false;
         wInfo* ammoType;
         weapon* wp = (weapon*)inventories->getFromPlace(INVENTORY_RIGHT_HAND);
         for(i=0; ((i < 2) && (activeObject != NULL)); i++)
         {
            if(wp != NULL)
            {
               ammoType = wp->getMunitionType();
               if(ammoType->name == activeObject->getRelatedInfo())
               {
                  /* Weapon and ammo of the same type, so reload it! */
                  used = true;
                  int needs = wp->getMunitionCapacity() - 
                              wp->getCurrentMunition();

                  sprintf(buf, gettext("%s reloaded %s with %s ammo."),
                          owner->name.c_str(), wp->name.c_str(), 
                          ammoType->title.c_str());
                  brief.addText(buf);

                  if(activeObject->getState() > needs)
                  {
                     /* Fully recharge */
                     wp->setCurrentMunition(wp->getMunitionCapacity());
                     activeObject->setState(activeObject->getState()-needs);
                  }
                  else
                  {
                     /* Recharge with the quantity we current have */
                     wp->setCurrentMunition(activeObject->getState());
                     /* And remove the object (it isn't used anymore) */
                     inventories->removeFromInventory(objX,objY, 
                           currentInventory);
                     delete(activeObject);
                     activeObject = NULL;
                     reDraw();
                  }
               }
            }
 
            /* Next, verify the left hand weapon too */
            wp = (weapon*)inventories->getFromPlace(INVENTORY_LEFT_HAND);
         }
         activeObject = NULL;
         
         if(!used)
         {
            /* If can't use the ammo, better show a message */
            warning warn;
            warn.show(gettext("Warning"), 
                      gettext("No equipped weapons are compatible with this "
                              "munition type."), interf);
         }
      }

      /* narcotic objects */
      else if(activeObject->getType() == OBJECT_TYPE_NARCOTIC)
      {
         /* TODO -> get the narcotic target (currently 
          * doing always on the inventory owner) */
         character* target = owner;

         /* Create the script */
         iaScript* useScript = new iaScript(activeObject->getRelatedInfo(), 
               curEngine); 

         /* Define params and owner */
         useScript->defineObjectOwner(activeObject);
         useScript->setParameter("target", target);
         useScript->setParameter("difficulty", 
               &activeObject->curBonusAndSaves.iAmNotAFool);
         useScript->setParameter("drunkLevel", 
               &activeObject->curBonusAndSaves.fortitude);

         /* Run it to the end! */
         useScript->run(0);

         /* Done, close it */
         useScript->close();
         delete(useScript);

         /* And discard the object */
         inventories->removeFromInventory(objX,objY, 
               currentInventory);
         delete(activeObject);
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


/**************************************************************
 *                             treat                          *
 **************************************************************/
int inventWindow::treat(guiObject* guiObj, int eventInfo, cursor* mouseCursor,
                        Map* actualMap, GLfloat X, GLfloat Z, bool seller)
{
   if(!isOpen())
   {
      /* Window is no more opened */
      return(INVENTORY_ACTION_NONE);
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


   /* Get the position at "inventory coordinates" */
   int posX = (int) floor((x - (8 + intWindow->getX1())) / (19.0));
   int posY = (int) floor((y - (292 + intWindow->getY1())) / (19.0));

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
            return(INVENTORY_ACTION_REMOVE_ITEM);
         }
      }
   }
   
   switch(eventInfo)
   {
      /* Changes to the Current Inventory */
      case FARSO_EVENT_TAB_BOX_CHANGED:
      {
         if(guiObj == (guiObject*)inventoryTabBox)
         {
            int inv;
            sscanf(inventoryTabBox->getActiveTitle().c_str(),"%d",&inv);
            if((inv >= 1) && (inv <= 4))
            {
               currentInventory = inv-1;
               reDraw();
            }
         }
      }
      break;

      /* Accessing inventory or equipment slots! */
      case FARSO_EVENT_PRESSED_TAB_BUTTON:
      {
         /* Inventory Spaces Selected */
         if( (guiObj == (guiObject*) inventoryButton[0]) ||
             (guiObj == (guiObject*) inventoryButton[1]) ||
             (guiObj == (guiObject*) inventoryButton[2]) ||
             (guiObj == (guiObject*) inventoryButton[3]))
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
               return(INVENTORY_ACTION_INTERNAL);
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
               return(INVENTORY_ACTION_CHANGE_ITEM);
            }
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
               return(INVENTORY_ACTION_INTERNAL);
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
               return(INVENTORY_ACTION_EQUIPED);
            }
         }
      }
      break;
     
      /* Verify Menu Options */
      case FARSO_EVENT_SELECTED_MENU:
      {
         if(objectMenu)
         {
            int act = INVENTORY_ACTION_NONE;
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
               {
                  if(infoWindow)
                  {
                     infoWindow->open(activeObject);
                  }
               }
               break;
               case 5: /* Use */
               {
                 verifyUseObject();
                 act = INVENTORY_ACTION_USE_ITEM;
               }
               break;
               case 6: /* Buy /  Sell */
               {
                  barterWindow tradeWindow;

                  if(activeObject)
                  {
                     /* FIXME
                     if(activeObject->getType() == OBJECT_TYPE_MONEY)
                     {
                        money* m = (money*)activeObject;
                        quantityWindow qty;
                        qty.setRange(0.0f, m->quantity());
                        qty.setValue(m->quantity());
                        qty.setDelta(1.0f);
                        qty.show(gettext("Money"), 
                                 gettext("Define quantity:"), interf); 
                     }*/

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
                  #if 0
                     if(activeObject->getType() == OBJECT_TYPE_MONEY)
                     {
                        splitWindow spWindow;
                        spWindow.open(interf, activeObject, curEngine, owner); 
                     }
                     else
                   #endif
                   //  {
                        /* drop the object */
                        inventories->dropObject(activeObject, objX, objY, 
                              currentInventory, actualMap, X, Z);
                   //  }

                     /* Return to the NONE state */
                     activeObject = NULL;
                     state = INVENTORY_STATE_NONE;
                     
                     act = INVENTORY_ACTION_REMOVE_ITEM;
                  }
               break;
            }
            intWindow->getObjectsList()->removeMenu();
            objectMenu = NULL;
            return(act);
         }
      }
      break;
   }
   return(INVENTORY_ACTION_NONE);
}


