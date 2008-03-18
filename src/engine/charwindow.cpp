#include "charwindow.h"
#include "../classes/xp.h"

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
charWindow::charWindow(interface* interf)
{
   inter = interf;
   intWindow = NULL;
}

/********************************************************************
 *                            Destructor                            *
 ********************************************************************/
charWindow::~charWindow()
{
   close();
}

/********************************************************************
 *                              open                                *
 ********************************************************************/
void charWindow::open(character* pers)
{
   dirs dir;
   /* Set open position */
   int posY = 1;
   int posX = 0;

   char buf[512];
   string tmpStr = "";

   /* Close the window, if it is openned */
   if(isOpen())
   {
      posX = intWindow->getX1();
      posY = intWindow->getY1();
      close();
   }

   /* Create Window */
   intWindow = inter->insertWindow(posX, posY, posX+288, posY+330,
                                   pers->name);

   /* Character Image */
   picture* fig;
   fig = intWindow->getObjectsList()->insertPicture(9,19,
                                                    0,0,
                   dir.getRealFile(pers->getPortraitFileName()).c_str());
   intWindow->getObjectsList()->insertTextBox(5,15,65,75,1,"");

   /* General Things */

      /* name */
   sprintf(buf,"%s %s", gettext("Name:"), pers->name.c_str());
   intWindow->getObjectsList()->insertTextBox(68,15,283,25,0,buf);
      /* age */
   sprintf(buf,"%s %d", gettext("Age:"), pers->age);
   intWindow->getObjectsList()->insertTextBox(68,26,128,36,0,buf);
      /* sex */
   switch(pers->sexType)
   {
      case SEX_FEMALE:
         tmpStr = gettext("Female");
      break;
      case SEX_MALE:
         tmpStr = gettext("Male");
      break;
      default:
         tmpStr = gettext("Undefined");
      break;
   }
   sprintf(buf,"%s %s", gettext("Sex:"), tmpStr.c_str());
   intWindow->getObjectsList()->insertTextBox(130,26,283,36,0,buf);
      /* Race */
   sprintf(buf,"%s %s", gettext("Race:"), pers->actualRace->name.c_str());
   intWindow->getObjectsList()->insertTextBox(68,37,283,47,0,buf);
      /* Alignment */
   sprintf(buf,"%s %s", gettext("Alignment:"), 
                        pers->actualAlign->name.c_str());
   intWindow->getObjectsList()->insertTextBox(68,48,283,58,0,buf);
      /* XP */
   sprintf(buf,"%s %d %s %d", gettext("XP:"), pers->xp,
                              gettext("Next Level:"), nextLevelXP(pers->xp));
   intWindow->getObjectsList()->insertTextBox(68,59,283,69,0,buf);
   intWindow->getObjectsList()->insertTextBox(66,15,283,75,2,"");

   /* Life Points And AC */
   sprintf(buf,"%s %d/%d", gettext("HP:"), pers->lifePoints, pers->maxLifePoints);
   intWindow->getObjectsList()->insertTextBox(8,77,62,87,0,buf);
   sprintf(buf,"%s %d", gettext("AC:"), pers->armatureClass);
   intWindow->getObjectsList()->insertTextBox(8,88,62,98,0,buf);
   intWindow->getObjectsList()->insertTextBox(5,76,65,104,2,"");


   /* Character Classes */


   /* Ok Button */
   okButton = intWindow->getObjectsList()->insertButton(88,188,158,207,
                                                        gettext("Ok"),1);                                   
   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(true,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                             close                                *
 ********************************************************************/
void charWindow::close()
{
   if(intWindow)
   {
      inter->closeWindow(intWindow);
      intWindow = NULL;
   }
}

/********************************************************************
 *                             isOpen                               *
 ********************************************************************/
bool charWindow::isOpen()
{
   return(intWindow != NULL);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int charWindow::treat(guiObject* object, int eventInfo)
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


