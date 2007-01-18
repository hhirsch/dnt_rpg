/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/


#include "inventwindow.h"


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
   window = inter->ljan->InserirJanela(0,0,263,402,"Inventory",
                                       1,1,NULL,NULL);

   /* Add Character (equip) Tab Button */
   characterTabButton = window->objects->InserirTabButton(4,15,256,256,
                                         "../data/texturas/inventory/equip.png");
   headButton = characterTabButton->insertButton(109,3,147,41);
   footButton = characterTabButton->insertButton(109,214,157,252);
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
   window->Abrir(inter->ljan);
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
inventWindow::~inventWindow()
{
   if(window)
   {
      window->Fechar(interf->ljan);
   }
}

/**************************************************************
 *                            isOPen                          *
 **************************************************************/
bool inventWindow::isOpen()
{
   return(window != NULL);
}

