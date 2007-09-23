/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "aspectwindow.h"

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
aspectWindow::aspectWindow(character* dude, interface* inter)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   curImage = 0;
   images = NULL;
   loadImages();

   usedCharacter = dude;
   intWindow = inter->insertWindow(275,206,532,315,gettext("Aspect"));

   /* Portrait Figure */
   figurePortrait = intWindow->getObjectsList()->insertPicture(5,18,0,0,NULL);
   figurePortrait->set(images[curImage].image);

   /* Previous Image Button */
   buttonPreviousImage = intWindow->getObjectsList()->insertButton(5,83,
                                                                  19,101,"<",0);
   /* Next Image Button */
   buttonNextImage = intWindow->getObjectsList()->insertButton(55,83,69,101,
                                                                 ">",0);
   /* Contorn to the previous and next buttons */
   intWindow->getObjectsList()->insertTextBox(20,83,54,101,1,"");

   /* Name Text */
   intWindow->getObjectsList()->insertTextBox(72,20,112,34,1,gettext("Name:"));
   textName = intWindow->getObjectsList()->insertTextBar(113,20,251,34,
                                                                  "",0);

   /* Age Text */
   intWindow->getObjectsList()->insertTextBox(72,35,112,49,1,gettext("Age:"));
   textAge = intWindow->getObjectsList()->insertTextBar(113,35,143,49,"20",0);

   /* Sex Selectors */
   intWindow->getObjectsList()->insertTextBox(72,50,112,64,1,gettext("Sex:"));
   cxSelSexF = intWindow->getObjectsList()->insertCxSel(113, 52, false);
   intWindow->getObjectsList()->insertPicture(125,50,0,0,
                                  "../data/texturas/aspectw/sex_f.png");
   cxSelSexM = intWindow->getObjectsList()->insertCxSel(143, 52, true);
   intWindow->getObjectsList()->insertPicture(155,50,0,0,
                                  "../data/texturas/aspectw/sex_m.png");
   cxSelSexO = intWindow->getObjectsList()->insertCxSel(173, 52, false);
   intWindow->getObjectsList()->insertPicture(185,50,0,0,
                                  "../data/texturas/aspectw/sex_o.png");

   /* Contorns */
   intWindow->getObjectsList()->insertTextBox(70,18,254,74,2,"");
   intWindow->getObjectsList()->insertTextBox(70,75,254,101,2,"");

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(181,79,251,97, 
                                                 gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(72,79,140,97, 
                                                  gettext("Cancel"),1);

   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,false, false, false);
   inter->openWindow(intWindow);
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
aspectWindow::~aspectWindow()
{
   /* Delete all images */
   if(images)
   {
      delete[] images;
   }
}

/**************************************************************
 *                          loadImages                        *
 **************************************************************/
void aspectWindow::loadImages()
{
   FILE* f;
   int i;
   char buffer[256];

   totalImages = 0;

   if( !(f = fopen("../data/characters/portraits/portraits.lst", "r")) )
   {
      printf(gettext("Can't open the list with portraits: portraits.lst!\n"));
      return;
   }

   fscanf(f,"%d", &totalImages);
   images = new portraitImage[totalImages];

   for(i = 0; i < totalImages; i++)
   {
      fscanf(f,"%s", &buffer[0]);
      images[i].imageFile = buffer;
      images[i].image = IMG_Load(images[i].imageFile.c_str());
   }
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
int aspectWindow::treat(guiObject* object, int eventInfo, interface* inter)
{
   if(eventInfo == PRESSED_BUTTON)
   {
      /* Confirm */
      if(object == (guiObject*) buttonConfirm)
      {
         /* Save Things to the character */
         usedCharacter->definePortrait(images[curImage].imageFile);
         /* Define Sex */
         if(cxSelSexF->isSelected())
         {
            usedCharacter->sexType = SEX_FEMALE;
         }
         else if(cxSelSexM->isSelected())
         {
            usedCharacter->sexType = SEX_MALE;
         }
         else
         {
            usedCharacter->sexType = SEX_OTHER;
         }
         /* Define Name */
         usedCharacter->nome = textName->getText();
         /* Define Age */
         sscanf(textAge->getText().c_str(),"%d", &usedCharacter->age);

         /* Close Window */
         figurePortrait->set(NULL);
         inter->closeWindow(intWindow);
         intWindow = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ASPECTW_CONFIRM);
      }
      /* Cancel */
      else if(object == (guiObject*) buttonCancel) 
      {
         figurePortrait->set(NULL);
         inter->closeWindow(intWindow);
         intWindow = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ASPECTW_CANCEL);
      }
      /* Previous Image */
      else if(object == (guiObject*) buttonPreviousImage)
      {
         curImage--;
         if(curImage < 0)
         {
            curImage = totalImages-1;
         }
         figurePortrait->set(images[curImage].image);
      }
      /* Next Image */
      else if(object == (guiObject*) buttonNextImage)
      {
         curImage++;
         if(curImage >= totalImages)
         {
            curImage = 0;
         }
         figurePortrait->set(images[curImage].image);
      }
   }
   else if(eventInfo == MODIFIED_CX_SEL)
   {
      /* Female */
      if(object == (guiObject*) cxSelSexF)
      {
         if(cxSelSexF->isSelected())
         {
            cxSelSexM->setSelection(false);
         }
         else
         {
            cxSelSexM->setSelection(true);
         }
         cxSelSexO->setSelection(false);
      }
      /* Male */
      if(object == (guiObject*) cxSelSexM)
      {
         if(cxSelSexM->isSelected())
         {
            cxSelSexF->setSelection(false);
         }
         else
         {
            cxSelSexF->setSelection(true);
         }
         cxSelSexO->setSelection(false);
      }
      /* Other */
      if(object == (guiObject*) cxSelSexO)
      {
         if(cxSelSexO->isSelected())
         {
            cxSelSexF->setSelection(false);
         }
         else
         {
            cxSelSexF->setSelection(true);
         }
         cxSelSexM->setSelection(false);
      }
   }
   intWindow->draw(0,0);
   return(ASPECTW_OTHER);
}

