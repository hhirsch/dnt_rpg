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
   window = inter->insertWindow(0,0,263,402,"Inventory",1,1);

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
   inventories[currentInventory]->draw(0,0, inventoryTabButton->fig);
   window->Desenhar(0,0);

   state = INVENTORY_STATE_NONE;
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
inventWindow::~inventWindow()
{
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
   if(isOpen())
   {
      inventories[0]->draw(0,0, inventoryTabButton->fig);
      window->Desenhar(0,0);
   }
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
bool inventWindow::treat(object** activeObject, Tobjeto* guiObject, 
                         int eventInfo)
{
   int x,y;
   Uint8 Mbotao = SDL_GetMouseState(&x,&y);
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
               int posX = (int) floor((x - (8 + window->x1)) / (20.0));
               int posY = (int) floor((y - (284 + window->y1)) / (20.0));
               if(inventories[currentInventory]->getFromPosition(posX, posY))
               {
                  *activeObject = inventories[currentInventory]->
                                   getFromPosition(posX, posY);
                  objectMenu = (menu*) window->objects->addMenu();
                  objectMenu->InserirMenuItem("Drop",1);
                  objectMenu->InserirMenuItem("-",0);
                  objectMenu->InserirMenuItem("Sell    ",0);
                  objectMenu->InserirMenuItem("Use",0);
                  objectMenu->InserirMenuItem("-",0);
                  objectMenu->InserirMenuItem("Get",1);
                  objectMenu->Coordenada(x,y);
                  state = INVENTORY_STATE_MENU;
               }
               return(true);
            }
            else if(state == INVENTORY_STATE_OBJECT)
            {
               /* TODO Put Object on  */
               return(true);
            }
         }
         /* Change to Inventory 1 */
         else if(guiObject == (Tobjeto*) inv1Button)
         {
            currentInventory = 0;
            inventories[currentInventory]->draw(0,0, inventoryTabButton->fig);
            window->Desenhar(0,0);
            return(true);
         }
         /* Change to Inventory 2 */
         else if(guiObject == (Tobjeto*) inv2Button)
         {
            currentInventory = 1;
            inventories[currentInventory]->draw(0,0, inventoryTabButton->fig);
            window->Desenhar(0,0);
            return(true);
         }
         /* Change to Inventory 3 */
         else if(guiObject == (Tobjeto*) inv3Button)
         {
            currentInventory = 2;
            inventories[currentInventory]->draw(0,0, inventoryTabButton->fig);
            window->Desenhar(0,0);
            return(true);
         }
         /* Change to Inventory 4 */
         else if(guiObject == (Tobjeto*) inv4Button)
         {
            currentInventory = 3;
            inventories[currentInventory]->draw(0,0, inventoryTabButton->fig);
            window->Desenhar(0,0);
            return(true);
         } 

         /* Verify Character's Buttons */
         if(state == INVENTORY_STATE_OBJECT)
         {
            if(guiObject == (Tobjeto*) headButton)
            {
               return(true);
            }
            else if(guiObject == (Tobjeto*) leftHandButton)
            {
               return(true);
            }
            else if(guiObject == (Tobjeto*) rightHandButton)
            {
               return(true);
            }
            else if(guiObject == (Tobjeto*) leftFingerButton)
            {
               return(true);
            }
            else if(guiObject == (Tobjeto*) rightFingerButton)
            {
               return(true);
            }
            else if(guiObject == (Tobjeto*) neckButton)
            {
               return(true);
            }
            else if(guiObject == (Tobjeto*) footButton)
            {
               return(true);
            }
            else if(guiObject == (Tobjeto*) bodyButton)
            {
               return(true);
            }
         }
      }
      break;
      
      case MENUSELECIONADO:
      {
         objectMenu = NULL;
         //TODO get return of menu
         state = INVENTORY_STATE_NONE;
         window->objects->removeMenu();         
         return(true);
      }
      break;
   }
   return(false);
}


