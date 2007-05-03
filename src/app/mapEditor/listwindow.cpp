#include "listwindow.h"

/* NPCs */
#define FILE_CHARACTERS "../data/mapEditor/lists/characters.lst"

/* Objects */
#define FILE_BUILDING   "../data/mapEditor/lists/building.lst"
#define FILE_CARS       "../data/mapEditor/lists/cars.lst" 
#define FILE_COMMON     "../data/mapEditor/lists/common.lst"
#define FILE_DOORS      "../data/mapEditor/lists/doors.lst"
#define FILE_GUNS       "../data/mapEditor/lists/guns.lst"
#define FILE_ICEX       "../data/mapEditor/lists/icex.lst"
#define FILE_NATURE     "../data/mapEditor/lists/nature.lst"

/* Particles */
#define FILE_FIRE       "../data/mapEditor/lists/fire.lst"
#define FILE_GRASS      "../data/mapEditor/lists/grass.lst"
#define FILE_SMOKE      "../data/mapEditor/lists/smoke.lst"
#define FILE_SNOW       "../data/mapEditor/lists/snow.lst"
#define FILE_WATERFALL  "../data/mapEditor/lists/waterfall.lst"


/********************************************************************
 *                             Constructor                          *
 ********************************************************************/
listWindow::listWindow(interface* gui)
{
   intGui = gui;
   state = -1;
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
 *                              setState                            *
 ********************************************************************/
void listWindow::setState(int st)
{
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
      }
      /* Load file */
      if(!fileName.empty())
      {
         loadFromFile(fileName);
      }
   }
}

/********************************************************************
 *                               open                               *
 ********************************************************************/
void listWindow::open()
{
   intWindow = intGui->insertWindow(50,50,300,250,"Select",1,1);
   list = intWindow->objects->insertListText(5,15,245,190,intWindow->cara);
   intWindow->ptrExterno = &intWindow;
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
   int total = 0;
   int i;
   if(!(arq=fopen(fileName.c_str(), "r")))
   {
      printf("Can't open List file: %s\n", fileName.c_str());
      return;
   }

   fgets(buffer, sizeof(buffer), arq);
   sscanf(buffer, "%d", &total);

   for(i = 0; i < total; i++)
   {
      fgets(buffer, sizeof(buffer), arq);
      sscanf(buffer, "%s", buf2);
      list->insertText(buf2);
   }
   fclose(arq);
}

