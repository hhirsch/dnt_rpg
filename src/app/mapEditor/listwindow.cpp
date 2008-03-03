#include "listwindow.h"
#include "../../etc/dirs.h"

/* NPCs */
#define FILE_CHARACTERS "mapEditor/lists/characters.lst"

/* Objects */
#define FILE_BUILDING   "mapEditor/lists/building.lst"
#define FILE_CARS       "mapEditor/lists/cars.lst" 
#define FILE_COMMON     "mapEditor/lists/common.lst"
#define FILE_DOORS      "mapEditor/lists/doors.lst"
#define FILE_GUNS       "mapEditor/lists/guns.lst"
#define FILE_ICEX       "mapEditor/lists/icex.lst"
#define FILE_NATURE     "mapEditor/lists/nature.lst"
#define FILE_MACABRE    "mapEditor/lists/macabre.lst"
#define FILE_BATH       "mapEditor/lists/bath.lst"

/* Particles */
#define FILE_FIRE       "mapEditor/lists/fire.lst"
#define FILE_GRASS      "mapEditor/lists/grass.lst"
#define FILE_SMOKE      "mapEditor/lists/smoke.lst"
#define FILE_SNOW       "mapEditor/lists/snow.lst"
#define FILE_WATERFALL  "mapEditor/lists/waterfall.lst"

/* Musics */
#define FILE_MUSIC      "mapEditor/lists/music.lst"

/********************************************************************
 *                             Constructor                          *
 ********************************************************************/
listWindow::listWindow(interface* gui)
{
   intGui = gui;
   state = -1;
   list = NULL;
   intWindow = NULL;
   listElements = NULL;
}

/********************************************************************
 *                              Destructor                          *
 ********************************************************************/
listWindow::~listWindow()
{
   if(intWindow)
   {
      intGui->closeWindow(intWindow);
   }
}
/********************************************************************
 *                              getState                            *
 ********************************************************************/
int listWindow::getState()
{
   return(state);
}

/********************************************************************
 *                              setState                            *
 ********************************************************************/
void listWindow::setState(int st)
{
   dirs dir;
   string fileName = "";
   if( (state != st) || (intWindow == NULL))
   {
      if(intWindow == NULL)
      {
         open();
      }
      state = st;
      list->clear();
      /* Define the file */
      switch(state)
      {
         case STATE_CHARACTERS:
            fileName = FILE_CHARACTERS;
         break;
         case STATE_BUILDING:
            fileName = FILE_BUILDING;
         break;
         case STATE_CARS:
            fileName = FILE_CARS;
         break;
         case STATE_COMMON:
            fileName = FILE_COMMON;
         break;
         case STATE_DOORS:
            fileName = FILE_DOORS;
         break;
         case STATE_GUNS:
            fileName = FILE_GUNS;
         break;
         case STATE_ICEX:
            fileName = FILE_ICEX;
         break;
         case STATE_NATURE:
            fileName = FILE_NATURE;
         break;
         case STATE_MACABRE:
            fileName = FILE_MACABRE;
         break;
         case STATE_BATH:
            fileName = FILE_BATH;
         break;
         case STATE_FIRE:
            fileName = FILE_FIRE;
         break;
         case STATE_GRASS:
            fileName = FILE_GRASS;
         break;
         case STATE_SMOKE:
            fileName = FILE_SMOKE;
         break;
         case STATE_SNOW:
            fileName = FILE_SNOW;
         break;
         case STATE_WATERFALL:
            fileName = FILE_WATERFALL;
         break;
         case STATE_MUSIC:
            fileName = FILE_MUSIC;
         break;
      }
      /* Load file */
      if(!fileName.empty())
      {
         loadFromFile(dir.getRealFile(fileName));
      }
      intWindow->draw(0,0);
   }
}

/********************************************************************
 *                               open                               *
 ********************************************************************/
void listWindow::open()
{
   intWindow = intGui->insertWindow(153,599-248,404,599-37,"Select");
   list = intWindow->getObjectsList()->insertListText(5,15,245,190,
                                                      intWindow->getSurface());
   intWindow->setExternPointer(&intWindow);
   intGui->openWindow(intWindow);
}

/********************************************************************
 *                           loadFromFile                           *
 ********************************************************************/
void listWindow::loadFromFile(string fileName)
{
   FILE* arq;
   char buffer[1024];
   char buf2[1024];
   char buf3[1024];
   total = 0;
   int i;

   if(listElements != NULL)
   {
      delete [] listElements;
      listElements = NULL;
   }

   if(!(arq=fopen(fileName.c_str(), "r")))
   {
      printf("Can't open List file: %s\n", fileName.c_str());
      return;
   }

   fgets(buffer, sizeof(buffer), arq);
   sscanf(buffer, "%d", &total);

   if(total > 0)
   {
      listElements = new listElement[total];
   }

   for(i = 0; i < total; i++)
   {
      fgets(buffer, sizeof(buffer), arq);
      sscanf(buffer, "%s %s", buf2, buf3);
      list->insertText(buf2);
      listElements[i].title = buf2;
      listElements[i].fileName = buf3;
   }
   fclose(arq);
}

/********************************************************************
 *                           getFileName                            *
 ********************************************************************/
string listWindow::getFileName()
{
   return(selFileName);
}

/********************************************************************
 *                       getFileNameWithTitle                       *
 ********************************************************************/
string listWindow::getFileNameWithTitle(string title)
{
   int i;
   for(i = 0; i < total; i++)
   {
      if(listElements[i].title == title)
      {
         return(listElements[i].fileName);
      }
   }
   return("");
}

/********************************************************************
 *                             eventGot                             *
 ********************************************************************/
bool listWindow::eventGot(int type, guiObject* object)
{
   selFileName = "";
   if(type == SELECTED_LIST_TEXT)
   {
      if(object == list)
      {
         selFileName = getFileNameWithTitle(list->getSelectedText());
         return(!selFileName.empty());
      }
   }
   return(false);
}

