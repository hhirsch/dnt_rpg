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
inventWindow::inventWindow(inventory *invent[INVENTORY_PER_CHARACTER],
                           interface* inter)
{
   int i;

   /* Copy Interface Pointer */
   interf = inter;

   /* Copy Inventories Pointers */
   for(i=0; i < INVENTORY_PER_CHARACTER; i++)
   {
      inventories[i] = invent[i];
   }
   
   /* Add Window */
   window = inter->insertWindow(0,0,263,402,language.INVENTW_TITLE.c_str(),1,1);

   /* Add Character (equip) Tab Button */
   characterTabButton = window->objects->InserirTabButton(4,15,256,256,
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
   inventoryTabButton = window->objects->InserirTabButton(4,271,256,128,
                                   "../data/texturas/inventory/inventory.png");
   inventoryButton = inventoryTabButton->insertButton(4,13,251,127);
   inv1Button = inventoryTabButton->insertButton(4,0,64,13);
   inv2Button = inventoryTabButton->insertButton(64,0,127,13);
   inv3Button = inventoryTabButton->insertButton(127,0,191,13);
   inv4Button = inventoryTabButton->insertButton(191,0,251,13);

   /* Open Window */
   window->ptrExterno = &window;
   inter->openWindow(window);

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
         int inv = 0;
         while( (!inventories[inv]->addObject(activeObject) && 
                (inv < INVENTORY_PER_CHARACTER)))
         {
            inv++;
         }
                            
         if(inv < INVENTORY_PER_CHARACTER)
         {
            activeObject = NULL;
            state = INVENTORY_STATE_NONE;
         }
      }
   }
   if(window)
   {
      interf->closeWindow(window);
   }
}

/**************************************************************
 *                            isOPen                          *
 **************************************************************/
bool inventWindow::isOpen()
{
   return(window != NULL);
}

/**************************************************************
 *                            reDraw                          *
 **************************************************************/
void inventWindow::reDraw()
{
   /*int x1 = 0, 
       x2 = 0;*/
   if(isOpen())
   {
      inventories[currentInventory]->draw(0,0, inventoryTabButton->fig);
      window->Desenhar(0,0);

      //TODO Mark on actual Inventory!
      /*switch(currentInventory)
      {
         case 0: 
            x1 = 4; x2 = 64;
         break;
         case 1:
            x1 = 64; x2 = 127;
         break;
         case 2:
            x1 = 127; x2 = 191;
         break;
         case 3:
            x1 = 191; x2 = 251;
         break;
      }*/
   }
}

/**************************************************************
 *                          openMenu                          *
 **************************************************************/
void inventWindow::openMenu(int x, int y)
{
   int xSize;
   objectMenu = (menu*) window->objects->addMenu();
   objectMenu->InserirMenuItem(language.INVENTW_DROP,1);
   objectMenu->InserirMenuItem("-",0);
   objectMenu->InserirMenuItem(language.INVENTW_SELL,0);
   objectMenu->InserirMenuItem(language.INVENTW_USE,0);
   objectMenu->InserirMenuItem("-",0);
   objectMenu->InserirMenuItem(language.INVENTW_GET,1);

   selFonte(FFARSO,ESQUERDA,1);

   xSize = objectMenu->maxCarac*(fonte_incCP()+1)+6;

   /* Make Sure all Menu is in Window */
   if(y+70 >= window->y2)
   {
       y = window->y2-70;
   }

   if(x+xSize >= window->x2)
   {
      x = window->x2 - xSize;
   }
   
   objectMenu->Coordenada(x,y);
   
   state = INVENTORY_STATE_MENU;
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
bool inventWindow::treat(Tobjeto* guiObject, int eventInfo)
{
   if(!isOpen())
   {
      /* Window is no more opened */
      return(false);
   }
   int x,y;
   Uint8 Mbotao = SDL_GetMouseState(&x,&y);

   int posX = (int) floor((x - (8 + window->x1)) / (19.0));
   int posY = (int) floor((y - (284 + window->y1)) / (19.0));

   if(Mbotao & SDL_BUTTON(3))
   {
      if( (state == INVENTORY_STATE_OBJECT) && (activeObject) )
      {
         /* Reput the object to the inventory */
         int inv = 0;
         while( (!inventories[inv]->addObject(activeObject) && 
                (inv < INVENTORY_PER_CHARACTER)))
         {
            inv++;
         }
                            
         if(inv < INVENTORY_PER_CHARACTER)
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
      case TABBOTAOPRESSIONADO:
      {
         /* Inventory Spaces Selected */
         if(guiObject == (Tobjeto*) inventoryButton)
         {
            if(state == INVENTORY_STATE_NONE)
            {
               /* Open Menu For Object if one is avaible */
               if(inventories[currentInventory]->getFromPosition(posX, posY))
               {
                  activeObject = inventories[currentInventory]->
                                   getFromPosition(posX, posY);
                  objX = posX;
                  objY = posY;
                  objWhere = INVENTORY_INVENTORY;
                  openMenu(x,y); 
               }
               return(true);
            }
            else if(state == INVENTORY_STATE_OBJECT)
            {
               /* Put Object on  */
               if(inventories[currentInventory]->addObject(activeObject,
                                                           posX, posY))
               {
                  delete(activeObject);
                  activeObject = NULL;
                  state = INVENTORY_STATE_NONE;
                  reDraw();
               }
               return(true);
            }
         }
         
         /* Change to Inventory 1 */
         else if(guiObject == (Tobjeto*) inv1Button)
         {
            currentInventory = 0;
            reDraw();
            return(true);
         }
         /* Change to Inventory 2 */
         else if(guiObject == (Tobjeto*) inv2Button)
         {
            currentInventory = 1;
            reDraw();
            return(true);
         }
         /* Change to Inventory 3 */
         else if(guiObject == (Tobjeto*) inv3Button)
         {
            currentInventory = 2;
            reDraw();
            return(true);
         }
         /* Change to Inventory 4 */
         else if(guiObject == (Tobjeto*) inv4Button)
         {
            currentInventory = 3;
            reDraw();
            return(true);
         } 
         
         if(state == INVENTORY_STATE_NONE)
         {
            object* aObject = NULL;
            if(guiObject == (Tobjeto*) headButton)
            {
               aObject = inventories[0]->getFromPlace(INVENTORY_HEAD);
               objWhere = INVENTORY_HEAD;
            }
            else if(guiObject == (Tobjeto*) leftHandButton)
            {
               aObject = inventories[0]->getFromPlace(INVENTORY_LEFT_HAND);
               objWhere = INVENTORY_LEFT_HAND;
            }
            else if(guiObject == (Tobjeto*) rightHandButton)
            {
               aObject = inventories[0]->getFromPlace(INVENTORY_RIGHT_HAND);
               objWhere = INVENTORY_RIGHT_HAND;
            }
            else if(guiObject == (Tobjeto*) leftFingerButton)
            {
               aObject = inventories[0]->getFromPlace(INVENTORY_LEFT_FINGER);
               objWhere = INVENTORY_LEFT_FINGER;
            }
            else if(guiObject == (Tobjeto*) rightFingerButton)
            {
               aObject = inventories[0]->getFromPlace(INVENTORY_RIGHT_FINGER);
               objWhere = INVENTORY_RIGHT_FINGER;
            }
            else if(guiObject == (Tobjeto*) neckButton)
            {
               aObject = inventories[0]->getFromPlace(INVENTORY_NECK);
               objWhere = INVENTORY_NECK;
            }
            else if(guiObject == (Tobjeto*) footButton)
            {
               aObject = inventories[0]->getFromPlace(INVENTORY_FOOT);
               objWhere = INVENTORY_FOOT;
            }
            else if(guiObject == (Tobjeto*) bodyButton)
            {
               aObject = inventories[0]->getFromPlace(INVENTORY_BODY);
               objWhere = INVENTORY_BODY;
            }

            if(aObject)
            {
               activeObject = aObject;
               openMenu(x,y);
               state = INVENTORY_STATE_MENU;
               return(true);
            }
         }
         /* State Object */
         else if(state == INVENTORY_STATE_OBJECT)
         {
            if(guiObject == (Tobjeto*) headButton)
            {
               if(inventories[0]->equipObject(activeObject, INVENTORY_HEAD))
               {
                  delete(activeObject);
                  activeObject = NULL;
               }
            }
            else if(guiObject == (Tobjeto*) leftHandButton)
            {
               if(inventories[0]->equipObject(activeObject, 
                                              INVENTORY_LEFT_HAND))
               {
                  delete(activeObject);
                  activeObject = NULL;
               }

            }
            else if(guiObject == (Tobjeto*) rightHandButton)
            {
               if(inventories[0]->equipObject(activeObject, 
                                              INVENTORY_RIGHT_HAND))
               {
                  delete(activeObject);
                  activeObject = NULL;
               }
            }
            else if(guiObject == (Tobjeto*) leftFingerButton)
            {
               if(inventories[0]->equipObject(activeObject,
                                              INVENTORY_LEFT_FINGER))
               {
                  delete(activeObject);
                  activeObject = NULL;
               }
            }
            else if(guiObject == (Tobjeto*) rightFingerButton)
            {
               if(inventories[0]->equipObject(activeObject,
                                              INVENTORY_RIGHT_FINGER))
               {
                  delete(activeObject);
                  activeObject = NULL;
               }
            }
            else if(guiObject == (Tobjeto*) neckButton)
            {
               if(inventories[0]->equipObject(activeObject, INVENTORY_NECK))
               {
                  delete(activeObject);
                  activeObject = NULL;
               }
            }
            else if(guiObject == (Tobjeto*) footButton)
            {
               if(inventories[0]->equipObject(activeObject, INVENTORY_FOOT))
               {
                  delete(activeObject);
                  activeObject = NULL;
               }
            }
            else if(guiObject == (Tobjeto*) bodyButton)
            {
               if(inventories[0]->equipObject(activeObject, INVENTORY_BODY))
               {
                  delete(activeObject);
                  activeObject = NULL;
               }
            }

            /* if no more object, done */
            if(!activeObject)
            {
               state = INVENTORY_STATE_NONE;
               return(true);
            }
            
         }
      }
      break;
      
      case MENUSELECIONADO:
      {
         //TODO get return of menu
         state = INVENTORY_STATE_NONE;
         switch(objectMenu->itemAtual)
         {
            case 1: /* Get */
               if(objWhere == INVENTORY_INVENTORY)
               {
                 /* Recreate the object, since it will be deleted when removed
                  * from Inventory. */
                 activeObject = new object(activeObject);
                 inventories[currentInventory]->removeFromInventory(objX, objY);
                 reDraw();
               }
               else
               {
                  inventories[0]->removeFromPlace(objWhere);
               }
               state = INVENTORY_STATE_OBJECT;
            break;
         }
         window->objects->removeMenu();
         objectMenu = NULL;
         return(true);
      }
      break;
   }
   return(false);
}


