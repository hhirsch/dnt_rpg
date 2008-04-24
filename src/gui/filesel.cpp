#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>

#include "filesel.h"
#include "guilist.h"
#include "interface.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
fileSel::fileSel(int x, int y, string dir, void* list)
{
   x1 = x;
   x2 = x+250;
   y1 = y;
   y2 = y+155;
   type = GUI_SEL_FILE;
   curDir = dir;

   intList = list;

   guiList* l = (guiList*)list;

   /* Create the text selector */
   textFiles = l->insertListText(x, y+17, x+250, y+130);

   /* Create the buttons */
   cancelButton = l->insertButton(x, y+135, x+70, y+154, gettext("Cancel"),1);
   acceptButton = l->insertButton(x+180, y+135, x+250, y+154, 
                                  gettext("Confirm"), 1);

   /* Create the current file text */
   textCurFile = l->insertTextBox(x,y, x+220, y+16, 1, "");

}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
fileSel::~fileSel()
{
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

   string* s = NULL;
   DIR* dir = NULL;
   struct dirent* dirEnt = NULL;

   /* Clear current displayed */
   textFiles->clear();

   /* Change to dir */
   curDir = newDir;
   opendir(newDir.c_str());

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

   /* Sort the result */
   if(total > 0)
   {
      qsort(&s[0], total, sizeof(string*), cmpFunction);
   }

   /* Put at selectors */
   for (j = 0; j < total; j++)
   {
      /* Remove the "group" char */
      s[j].erase(0,1);

      /* Insert at list */
      textFiles->insertText(s[j]);
   }

   /* clear strings */
   if(s)
   {
      delete[]s;
   }
}


/***********************************************************************
 *                                eventGot                             *
 ***********************************************************************/
bool fileSel::eventGot(int type, guiObject* object)
{
   switch(type)
   { 
      case PRESSED_BUTTON:
      {
         if(object == acceptButton)
         {
            //TODO done!
         }
         else if(object == cancelButton)
         {
            //TODO cancel!
         }
      }
      break;
      case SELECTED_LIST_TEXT:
      {
         //if(is dir)
         /* Change the directory to the new one */
         //else if(is file)
         /* Change the selected file */
      }
      break;
   }

   return(false);
}

