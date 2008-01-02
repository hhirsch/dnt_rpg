
#include "itemwindow.h"
#include "../classes/weapon.h"


/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
itemWindow::itemWindow(interface* interf)
{
   inter = interf;
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

   char buf[256];

   /* Close the window, if it is openned */
   if(isOpen())
   {
      centerX = intWindow->getX1()+128;
      centerY = intWindow->getY1()+128;
      close();
   }

   /* Create Window */
   intWindow = inter->insertWindow(centerX-128,centerY-128,
                                   centerX+128,centerY+128,
                                   item->getName());

   /***********************
    * Common Object Things 
    ***********************/
   
   /* Name */
   sprintf(buf,gettext("Name: %s"), item->getName().c_str());
   intWindow->getObjectsList()->insertTextBox(70,15,250,32,1,buf);

   /* Cost */
   sprintf(buf,gettext("Cost: %.2f"), item->cost);
   intWindow->getObjectsList()->insertTextBox(70,33,250,51,1,buf);

   /* Weight */
   sprintf(buf,gettext("Weight: %.2f Kg"), item->weight);
   intWindow->getObjectsList()->insertTextBox(70,52,250,69,1,buf);

   /* Item Image */
   int remainX = (64 - item->get2dModel()->w) / 2;
   int remainY = (77 - item->get2dModel()->h) / 2;
   fig = intWindow->getObjectsList()->insertPicture(8+remainX,16+remainY,
                                                    0,0,NULL);
   fig->set(item->get2dModel());
   intWindow->getObjectsList()->insertTextBox(5,15,69,92,1,"");

   /* Ok Button */
   okButton = intWindow->getObjectsList()->insertButton(88,228,158,247,
                                                        gettext("Ok"),1);

   curY = 70;

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
      sprintf(buf,"%s %dd%d+%d (x%d)", gettext("Damage Dice: "),  
              dam.baseDice.numberOfDices, dam.baseDice.diceID, 
              dam.baseDice.sumNumber, dam.baseDice.criticalMultiplier);
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
      curY += 18;

      /* Primary Damage */
      sprintf(buf,"%s %s", gettext("Primary Damage: "), damageA.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
      curY += 18;

      /* Secondary Damage */
      if(damageB.index != -1)
      {
         sprintf(buf,"%s %s", gettext("Secondary Damage: "), 
                 damageB.title.c_str());
         intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
         curY += 18;
      }

      /* Range */
      sprintf(buf,"%s %s", gettext("Range: "), range.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
      curY += 18;

      /* Size */
      sprintf(buf,"%s %s", gettext("Size: "), size.title.c_str());
      intWindow->getObjectsList()->insertTextBox(70,curY,250,curY+17,1,buf);
      curY += 18;

   }


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
         fig->set(NULL); //to not delete item image
         inter->closeWindow(intWindow);
         intWindow = NULL;
         return(1);
      }
   }
   return(0);
}

