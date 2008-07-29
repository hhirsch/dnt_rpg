
#include "itemwindow.h"
#include "../classes/weapon.h"


/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
itemWindow::itemWindow(guiInterface* interf)
{
   inter = interf;
   intWindow = NULL;
}

/********************************************************************
 *                            Destructor                            *
 ********************************************************************/
itemWindow::~itemWindow()
{
   close();
}

/********************************************************************
 *                              open                                *
 ********************************************************************/
void itemWindow::open(object* item)
{
   /* Set open position */
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   int curY = 0;
   int prvY = 0;

   char buf[256];

   /* Close the window, if it is openned */
   if(isOpen())
   {
      centerX = intWindow->getX1()+128;
      centerY = intWindow->getY1()+108;
      close();
   }

   /* Create Window */
   intWindow = inter->insertWindow(centerX-128,centerY-108,
                                   centerX+128,centerY+108,
                                   item->getName());

   /***********************
    * Common Object Things 
    ***********************/
   
   curY = 15;
   prvY = curY;

   /* Name */
   sprintf(buf,gettext("Name: %s"), item->getName().c_str());
   intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
   curY += 18;

   /* Cost */
   sprintf(buf,gettext("Cost: %.2f"), item->cost);
   intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
   curY += 18;

   /* Weight */
   sprintf(buf,gettext("Weight: %.2f Kg"), item->weight);
   intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
   curY += 18;

   /* General Contorn */
   intWindow->getObjectsList()->insertTextBox(70,prvY,250,curY-1,1,"");

   /***********************
    * Weapons Things 
    ***********************/
   if(item->getType() == OBJECT_TYPE_WEAPON)
   {
      /* Get All Info from the weapon */
      wInfo category, range, size, weight,
            damageA, damageB;
      weapon* wp = (weapon*)item;
      wp->getType(category, range, size, weight, damageA, damageB);
      diceThing dam = wp->getDice();

      /* Category */
      sprintf(buf,"%s %s", gettext("Category: "), category.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
      curY += 18;

      /* Damage Dice */
      prvY = curY;
      sprintf(buf,"%s %dd%d+%d (x%d)", gettext("Damage Dice: "),  
              dam.baseDice.getNumberOfDices(), dam.baseDice.getType(), 
              dam.baseDice.getSumNumber(), 
              dam.baseDice.getCriticalMultiplier());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Primary Damage */
      sprintf(buf,"%s %s", gettext("Primary Damage: "), damageA.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Secondary Damage */
      if(damageB.index != -1)
      {
         sprintf(buf,"%s %s", gettext("Secondary Damage: "), 
                 damageB.title.c_str());
         intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
         curY += 18;
      }

      /* Damage Contorn */
      intWindow->getObjectsList()->insertTextBox(70,prvY,250,curY-1,1,"");


      prvY = curY;
      /* Range */
      sprintf(buf,"%s %s", gettext("Range: "), range.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Size */
      sprintf(buf,"%s %s", gettext("Size: "), size.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,0,buf);
      curY += 18;

      /* Contorn to range and size */
      intWindow->getObjectsList()->insertTextBox(70,prvY,250,curY-1,1,"");
   }

   /* Item Image */
   int remainX = (64 - item->get2dModel()->w) / 2;
   int remainY = ((curY-15) - item->get2dModel()->h) / 2;
   fig = intWindow->getObjectsList()->insertPicture(8+remainX,16+remainY,
                                                    0,0,NULL);
   fig->set(item->get2dModel());
   intWindow->getObjectsList()->insertTextBox(5,15,69,curY-1,1,"");

   /* Ok Button */
   okButton = intWindow->getObjectsList()->insertButton(88,188,158,207,
                                                        gettext("Ok"),1);

   /* Remaining Contorn */
   intWindow->getObjectsList()->insertTextBox(5,curY,250,211,1,"");
   

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(true,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                             close                                *
 ********************************************************************/
void itemWindow::close()
{
   if(intWindow)
   {
      if(fig)
      {
         /* To avoid delete the item image */
         fig->set(NULL);
      }
      inter->closeWindow(intWindow);
      intWindow = NULL;
   }
}

/********************************************************************
 *                             isOpen                               *
 ********************************************************************/
bool itemWindow::isOpen()
{
   return(intWindow != NULL);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int itemWindow::treat(guiObject* object, int eventInfo)
{
   if(eventInfo == PRESSED_BUTTON)
   {
      if(object == (guiObject*) okButton)
      {
         close();
         return(1);
      }
   }
   return(0);
}

