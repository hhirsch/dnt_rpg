#include "charwindow.h"
#include "levelup.h"
#include "../classes/xp.h"

//TODO -> TREAT FEATS BUTTON!


/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
charWindow::charWindow(guiInterface* interf)
{
   /* set interface */
   inter = interf;

   /* Nullify pointers */
   intWindow = NULL;
   current = NULL;
   skWindow = NULL;
   okButton = NULL;
   levelUpButton = NULL;
   skillsButton = NULL;
   infoBar = NULL;
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
   int i;

   /* Set open position */
   int posY = 1;
   int posX = 0;

   char buf[512];
   string tmpStr = "";

   /* Ignore, if persnot defined */
   if(!pers)
   {
      return;
   }

   /* Close the window, if it is openned */
   if(isOpen())
   {
      if(pers == current)
      {
         /* Called to open again with the same character, so close window */
         close();
         return;
      }
      posX = intWindow->getX1();
      posY = intWindow->getY1();
      close();
   }

   /* Set Current */
   current = pers;

   /* Create Window */
   intWindow = inter->insertWindow(posX, posY, posX+340, posY+278,
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
   intWindow->getObjectsList()->insertTextBox(68,15,335,25,0,buf);
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
   intWindow->getObjectsList()->insertTextBox(130,26,335,36,0,buf);
      /* Race */
   sprintf(buf,"%s %s", gettext("Race:"), pers->actualRace->name.c_str());
   intWindow->getObjectsList()->insertTextBox(68,37,335,47,0,buf);
      /* Alignment */
   sprintf(buf,"%s %s", gettext("Alignment:"), 
                        pers->actualAlign->name.c_str());
   intWindow->getObjectsList()->insertTextBox(68,48,335,58,0,buf);
      /* XP */
   sprintf(buf,"%s: %d      %s: %d", gettext("XP"), pers->getXP(),
                                     gettext("Next Level"), 
                                     nextLevelXP(pers->getXP()));
   intWindow->getObjectsList()->insertTextBox(68,59,335,69,0,buf);
   intWindow->getObjectsList()->insertTextBox(66,15,335,75,2,"");

   /* Life Points And AC */
   sprintf(buf,"%s: %d/%d", gettext("HP"), pers->getLifePoints(), 
                            pers->getMaxLifePoints());
   intWindow->getObjectsList()->insertTextBox(8,77,62,87,0,buf);
   sprintf(buf,"%s: %d", gettext("AC"), pers->armatureClass);
   intWindow->getObjectsList()->insertTextBox(8,88,62,98,0,buf);
   intWindow->getObjectsList()->insertTextBox(5,76,65,116,2,"");


   /* Character Classes */
   posY = 77;
   for(i=0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if(pers->actualClass[i])
      {
         sprintf(buf,"%s(%d)", pers->actualClass[i]->name.c_str(),
                               pers->classLevels[i]);
         intWindow->getObjectsList()->insertTextBox(68,posY,335,posY+10,0,buf);
         posY += 11;
      }
   }
   intWindow->getObjectsList()->insertTextBox(65,76,335,116,2,"");

   /* Character Attributes */
   skill* sk;
      /* Strength */
   sk = pers->sk.getSkillByInt(ATT_STRENGTH);
   sprintf(buf,"%s: %.2d (%d)", gettext("Strength"), sk->points,
           (int)floor((sk->points-10) / 2.0));
   intWindow->getObjectsList()->insertTextBox(8,117,125,127,0,buf);
      /* Dexterity */
   sk = pers->sk.getSkillByInt(ATT_DEXTERITY);
   sprintf(buf,"%s: %.2d (%d)", gettext("Dexterity"), sk->points, 
           (int)floor((sk->points-10) / 2.0));
   intWindow->getObjectsList()->insertTextBox(8,128,125,138,0,buf);
      /* Constitution */
   sk = pers->sk.getSkillByInt(ATT_CONSTITUTION);
   sprintf(buf,"%s: %.2d (%d)", gettext("Constitution"), sk->points,
           (int)floor((sk->points-10) / 2.0));
   intWindow->getObjectsList()->insertTextBox(8,139,125,149,0,buf);
      /* Intelligence */
   sk = pers->sk.getSkillByInt(ATT_INTELLIGENCE);
   sprintf(buf,"%s: %.2d (%d)", gettext("Intelligence"), sk->points,
           (int)floor((sk->points-10) / 2.0));
   intWindow->getObjectsList()->insertTextBox(8,150,125,160,0,buf);
      /* Wisdow */
   sk = pers->sk.getSkillByInt(ATT_WISDOM);
   sprintf(buf,"%s: %.2d (%d)", gettext("Wisdom"), sk->points,
           (int)floor((sk->points-10) / 2.0));
   intWindow->getObjectsList()->insertTextBox(8,161,125,171,0,buf);
      /* Charism */
   sk = pers->sk.getSkillByInt(ATT_CHARISM);
   sprintf(buf,"%s: %.2d (%d)", gettext("Charisma"), sk->points,
           (int)floor((sk->points-10) / 2.0));
   intWindow->getObjectsList()->insertTextBox(8,172,125,182,0,buf);
      /* Contorn */
   intWindow->getObjectsList()->insertTextBox(5,117,125,191,2,"");

   /* Infos at RolBar */
   infoBar = intWindow->getObjectsList()->insertRolBar(126,117,335,271,"");
      /* Main Weapon */
      writeAboutWeapon(INVENTORY_RIGHT_HAND);
      writeAboutWeapon(INVENTORY_LEFT_HAND);

      /* Fortitude */
      sprintf(buf,"%s: %d", gettext("Fortitude"), 
              pers->curBonusAndSaves.fortitude);
      infoBar->addText(buf);

      /* Reflex */
      sprintf(buf,"%s: %d", gettext("Reflex"), pers->curBonusAndSaves.reflex);
      infoBar->addText(buf);

      /* IAmNotAFool */
      sprintf(buf,"%s: %d", gettext("I am Not a Fool"), 
              pers->curBonusAndSaves.iAmNotAFool);
      infoBar->addText(buf);

      /* Base Attack Modifier */
      sprintf(buf,"%s: %s", gettext("Base Attack Modifier"), 
              pers->curBonusAndSaves.baseAttackBonus.toString().c_str());
      infoBar->addText(buf);

      /* Size Modifier */
      sprintf(buf,"%s: %d", gettext("Size Modifier"), pers->sizeModifier);
      infoBar->addText(buf);

      /* TODO List all Effects affecting the character */

      /* Put bar at init */
      infoBar->setFirstLine(0);

   /* Level Up Button */
   levelUpButton = intWindow->getObjectsList()->insertButton(5,192,125,211,
                                                            gettext("Level Up"),
                                                             1);
   levelUpButton->setAvaible(pers->getUpLevels() > 0);

   /* Feats Button */
   intWindow->getObjectsList()->insertButton(5,212,125,231,
                                             gettext("Talents"),1);

   /* Skill Button */
   skillsButton = intWindow->getObjectsList()->insertButton(5,232,125,251,
                                                           gettext("Skills"),1);

   /* Close Button */
   okButton = intWindow->getObjectsList()->insertButton(5,252,125,271,
                                                        gettext("Close"),1); 

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
   /* Close Character Window */
   if(intWindow)
   {
      inter->closeWindow(intWindow);
      intWindow = NULL;
      current = NULL;
   }

   /* Close and delete Skill Window */
   if(skWindow)
   {
     if(skWindow->isOpened())
     {
        skWindow->close(inter);
     }
     delete(skWindow);
     skWindow = NULL;
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
 *                      hasChildrenWindows                          *
 ********************************************************************/
bool charWindow::hasChildrenWindows()
{
   return(skWindow != NULL);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int charWindow::treat(guiObject* object, int eventInfo,
                      GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   if(!isOpen())
   {
      /* No more open, must close children windows */
      if(hasChildrenWindows())
      {
         close();
      }
      return(1);
   }

   /* Verify Skill Window */
   if(skWindow)
   {
      int res = skWindow->treat(object, eventInfo, inter);

      if( (res == SKILLW_CONFIRM) || (res == SKILLW_CANCEL) )
      {
         /* Done with skill window */
         delete(skWindow);
         skWindow = NULL;
      }

      /* If event treated by Skill Window, must return */
      if(res != SKILLW_OTHER)
      {
         return(res);
      }
   }

   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      /* Ok Button */
      if(object == (guiObject*)okButton)
      {
         close();
         return(1);
      }
      /* Skills Button */
      else if(object == (guiObject*)skillsButton)
      {
         if(skWindow == NULL)
         {
            skWindow = new skillWindow(&current->sk,
                                       inter, current->getLevel(), true );
         }
      }
      /* Level Up Button */
      else if(object == (guiObject*)levelUpButton)
      {
         /* Call Level Up for Character */
         levelUp* lvlUp = new levelUp(current);
         lvlUp->doLevelUp(proj,modl,viewPort);
         delete(lvlUp);

         /* Close the character window */
         close();

         return(1);
      }
   }
   return(0);
}

/********************************************************************
 *                        Write About Weapon                        *
 ********************************************************************/
void charWindow::writeAboutWeapon(int inventoryPlace)
{
   weapon* wp;
   char buf[512];

   wp = (weapon*)current->inventories->getFromPlace(inventoryPlace);

   /* Define of what type of weapon talking */
   if(inventoryPlace == INVENTORY_RIGHT_HAND)
   {
      sprintf(buf, "%s", gettext("Main Weapon"));
   }
   else if(inventoryPlace == INVENTORY_LEFT_HAND)
   {
      sprintf(buf, "%s", gettext("Secondary Weapon"));
   }
   else
   {
      sprintf(buf, "%s", gettext("Weapon"));
   }

   if(wp)
   {
      /* Write about the weapon */
      sprintf(buf, "%s: %s", buf, wp->getName().c_str());
      infoBar->addText(buf);
   }
   else if(inventoryPlace == INVENTORY_RIGHT_HAND)
   {
      /* Write about unarmed */
      sprintf(buf, "%s: %s", buf, gettext("Unarmed"));
      infoBar->addText(buf);
   }
   else
   {
      /* Not Applicable */
      sprintf(buf, "%s: %s", buf, gettext("Not Applicable"));
      infoBar->addText(buf);
   }

}


