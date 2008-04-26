#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>

#include "filesel.h"
#include "guilist.h"
#include "interface.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
fileSel::fileSel(int x, int y, bool load, string dir, void* list)
{
   x1 = x;
   x2 = x+250;
   y1 = y;
   y2 = y+155;
   type = GUI_FILE_SEL;
   curDir = dir;
   lastAction = FILE_SEL_ACTION_NONE;
   lastDir = -1;
   filter = "";
   loading = load;

   intList = list;

   guiList* l = (guiList*)list;

   /* Create the text selector */
   textFiles = l->insertListText(x, y+17, x+250, y+130);

   /* Create the buttons */
   cancelButton = l->insertButton(x, y+135, x+70, y+154, gettext("Cancel"),1);
   acceptButton = l->insertButton(x+180, y+135, x+250, y+154, 
                                  gettext("Confirm"), 1);

   /* Create the current file text */
   if(loading)
   {
      textCurFile = l->insertTextBox(x,y, x+220, y+16, 1, "");
      editCurFile = NULL;
   }
   else
   {
      textCurFile = NULL;
      editCurFile = l->insertTextBar(x,y,x+220, y+16, "", 0);
   }

   /* Create the current filter text */
   textFilter = l->insertTextBox(x+220, y, x+250, y+16, 1, "*");

   /* Put it at initial dir */
   changeCurDir(dir);

}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
fileSel::~fileSel()
{
}

/***********************************************************************
 *                                draw                                 *
 ***********************************************************************/
void fileSel::draw(SDL_Surface* s)
{
}

/***********************************************************************
 *                              setFilter                              *
 ***********************************************************************/
void fileSel::setFilter(string newFilter)
{
   filter = newFilter;
   textFilter->setText(filter);
   /* Update the current dir with the new filter */
   changeCurDir(curDir);
}

/***********************************************************************
 *                             cmpFunction                             *
 ***********************************************************************/
static int cmpFunction(const void *p1,  const void *p2)
{
   /* Compare Function for the quicksort */
   string* s = (string*) p1;
   string* s2 = (string*) p2;
   return(s->compare(*s2));
}

/***********************************************************************
 *                             changeCurDir                            *
 ***********************************************************************/
void fileSel::changeCurDir(string newDir)
{
   int j, total;

   string aux = "";
   string* s = NULL;
   DIR* dir = NULL;
   struct dirent* dirEnt = NULL;

   /* Clear current displayed */
   textFiles->clear();

   /* Change to dir */
   curDir = newDir;
   if(curDir[curDir.length()-1] != '/')
   {
      curDir += "/";
   }
   dir = opendir(newDir.c_str());

   total = 0;
   if(dir)
   {
      /* first pass to define the size */
      for(dirEnt = readdir(dir); dirEnt != NULL; dirEnt = readdir(dir) )
      {
         total++;
      }

      if(total > 0)
      {
         s = new string[total];
      }

      /* rewind for next pass */
      rewinddir(dir);

      /* now save each directory/file name */
      j = 0;
      for(dirEnt = readdir(dir); dirEnt != NULL; dirEnt = readdir(dir) )
      {
         s[j] = dirEnt->d_name;

         /* Put a first character to group directories and files */
         if(dirEnt->d_type == DT_DIR)
         {
            s[j] = "d" + s[j];
         }
         else
         {
            s[j] = "f" + s[j];
         }
         j++;
      }
   }
   else
   {
      cerr << "Can't open dir: " << newDir << " : ";
      perror("");
   }

   /* Sort the result */
   if(total > 0)
   {
      qsort(&s[0], total, sizeof(string*), cmpFunction);
   }

   /* Put at selectors */
   for (j = 0; j < total; j++)
   {
      /* Directories */
      if(s[j][0] == 'd')
      {
         lastDir = j;
         /* Remove the "group" char */
         s[j].erase(0,1);

         /* Insert at list */
         textFiles->insertText(s[j], 255,20,20);
      }

      /* Files */
      else
      {
         /* Remove the "group" char */
         s[j].erase(0,1);

         if(filter != "")
         {
            /* Verify if pass */
            aux = s[j];
            aux.erase(0,aux.length()-filter.length());
            if(aux == filter)
            {
                /* Passed filter, so Insert at list */
                textFiles->insertText(s[j], 240,240,240);
            }
         }
         else
         {
            /* No filter, so Insert at list */
            textFiles->insertText(s[j], 240,240,240);
         }
      }
   }

   /* clear strings */
   if(s)
   {
      delete[]s;
   }
}

/***********************************************************************
 *                            getLastAction                            *
 ***********************************************************************/
int fileSel::getLastAction()
{
   return(lastAction);
}

/***********************************************************************
 *                             getFileName                             *
 ***********************************************************************/
string fileSel::getFileName()
{
   string fileName = curDir;
   if(loading)
   {
      fileName += textCurFile->getText();
   }
   else
   {
      fileName += editCurFile->getText();
   }
   return(fileName);
}

/***********************************************************************
 *                              eventGot                               *
 ***********************************************************************/
bool fileSel::eventGot(int type, guiObject* object)
{
   lastAction = FILE_SEL_ACTION_NONE;
   switch(type)
   { 
      case PRESSED_BUTTON:
      {
         /* Pressed Accept Button */
         if( (object == acceptButton) && (textCurFile->getText() != "") )
         {
            lastAction = FILE_SEL_ACTION_ACCEPT;
            return(true);
         }
         /* Pressed Cancel Button */
         else if(object == cancelButton)
         {
            lastAction = FILE_SEL_ACTION_CANCEL;
            return(true);
         }
      }
      break;
      case SELECTED_LIST_TEXT:
      {
         /* Selected a file of a directory */
         if(object == textFiles)
         {
            string sel = textFiles->getSelectedText();

            if(textFiles->getSelectedPos() <= lastDir)
            {
               /* It's a dir, change the directory to the new one */
               string newDir = "";
               if(sel == ".")
               {
                  /* The new dir is the same */
                  newDir = curDir;
               }
               else if(sel == "..")
               {
                  /* The new dir is without the last one, if not ../  */
                  if( (curDir.length() >= 3) && 
                      (curDir[curDir.length()-1] == '/') &&
                      (curDir[curDir.length()-2] == '.') &&
                      (curDir[curDir.length()-3] == '.') )
                  {
                     /* Continue to adding ../ */
                     newDir = curDir + "../";
                  }
                  else
                  {
                     /* Is without the last one */
                     newDir = curDir;
                     newDir.erase(newDir.length()-1, 1); //remove the last"/"
                     newDir.erase( newDir.find_last_of("/"));
                  }
               }
               else
               {
                  /* The new one is appended  */
                  newDir = curDir + sel; 
               }
               changeCurDir(newDir);
               /* Change the file text if is loading (since at saving 
                * we hope to keep the one got from user) */
               if(loading)
               {
                  textCurFile->setText("");
               }
            }
            else
            {
               /* It's a file, change the selected file */
               if(loading)
               {
                  textCurFile->setText(sel);
               }
               else
               {
                  editCurFile->setText(sel);
               }
            }
            lastAction = FILE_SEL_ACTION_SELECT;
            return(true);
         }
      }
      break;
   }

   return(false);
}

