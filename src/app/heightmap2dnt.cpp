/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include <unistd.h>
using namespace std;

#include "../map/map.h"
#include "../etc/dirs.h"

/***********************************************************************
 *                            doConversion                             *
 ***********************************************************************/
bool doConversion(string imgFile, string mapFile)
{
   SDL_Surface* img = IMG_Load(imgFile.c_str());
   Map* dntMap;
   int sizeX=0, sizeZ=0;
   int k,l,x,z;
   Uint32 color;
   Uint8 R, G, B;

   /* Verify Opened Image */
   if(!img)
   {
      cerr << "Couldn't load image input file!" << endl;
      return(false);
   }

   /* Create Map, based on image size */
   dntMap = new Map();
   sizeX = img->w / 10;
   sizeZ = img->h / 10;
   dntMap->newMap(img->w / 10, img->h / 10);
   dntMap->setOutdoor(true);

   /* throught all map */
   Square* saux;
   for(k = 0; k < sizeX; k++)
   {
      for(l=0; l < sizeZ; l++)
      {
         saux = dntMap->relativeSquare(k,l);

         /* Set the "un-walkable" flags */
         if( (k < 7) || (l < 7) || (k > sizeX-7) || (l > sizeZ-7) )
         {
            saux->flags = 0;
         }

         /* Set the heigth of square */
         x = k*10;
         z = l*10;

         color = Farso::pixel_Get(img, x, z);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         saux->h1 = (R+G+B) / 2.0;

         color = Farso::pixel_Get(img, x, z+10);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         saux->h2 = (R+G+B) / 2.0;

         color = Farso::pixel_Get(img, x+10, z+10);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         saux->h3 = (R+G+B) / 2.0;

         color = Farso::pixel_Get(img, x+10, z);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         saux->h4 = (R+G+B) / 2.0;
      }
   }
   dntMap->setInitialPosition((sizeX*dntMap->squareSize() / 2.0),
                              (sizeZ*dntMap->squareSize() / 2.0));


   /* Save the Map */
   if(!dntMap->save(mapFile))
   {
      delete(dntMap);
      SDL_FreeSurface(img);

      cerr << "Couldn't save map!" << endl;

      return(false);
   }


   delete(dntMap);
   SDL_FreeSurface(img);
   return(true);
}

/***********************************************************************
 *                              doIndoor                               *
 ***********************************************************************/
bool doIndoor(string imgFile, string mapFile)
{
   SDL_Surface* img = IMG_Load(imgFile.c_str());
   Map* dntMap;
   int sizeX=0, sizeZ=0;
   int k,l;
   Uint32 color;
   Uint8 R, G, B;
   float wx1 = -1;
   float wz1 = -1;
   float wx2 = -1;
   float wz2 = -1;

   /* Verify Opened Image */
   if(!img)
   {
      cerr << "Couldn't load image input file!" << endl;
      return(false);
   }

   /* Create Map, based on image size */
   dntMap = new Map();
   sizeX = img->w / 10;
   sizeZ = img->h / 10;
   dntMap->newMap(sizeX, sizeZ);
   dntMap->setOutdoor(false);

   /* Add vertical walls */
   for(k = 0; k < img->w; k++)
   {
      for(l=0; l < img->h; l++)
      {
         color = Farso::pixel_Get(img, k, l);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         if(R == 0)
         {
            /* Wall */
            if(wx1 == -1)
            {
               wx1 = k*INDOOR_SQUARE_SIZE/10.0f;
               wz1 = l*INDOOR_SQUARE_SIZE/10.0f;
            }
            else
            {
               wx2 = k*INDOOR_SQUARE_SIZE/10.0f;
               wz2 = l*INDOOR_SQUARE_SIZE/10.0f;
            }
         }
         else
         {
            /* No Wall */
            if((wx1 != -1) && (wx2 != -1))
            {
               /* Add it */
               dntMap->addWall(wx1, wz1, wx1+10, wz2+10);
            }

            /* Empty buffer */
            wx1 = -1; wz1 = -1;
            wx2 = -1; wz2 = -1;
         }
      }

      /* If have wall at end of line, must add it */
      if((wx1 != -1) && (wx2 != -1))
      {
         /* Add it */
         dntMap->addWall(wx1, wz1, wx1+10, wz2+10);
      }

      /* Empty buffer */
      wx1 = -1; wz1 = -1;
      wx2 = -1; wz2 = -1;
   }

   /* Add horizontal walls */
   for(l = 0; l < img->h; l++)
   {
      for(k=0; k < img->w; k++)
      {
         color = Farso::pixel_Get(img, k, l);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         if(R == 0)
         {
            /* Wall */
            if(wx1 == -1)
            {
               wx1 = k*INDOOR_SQUARE_SIZE/10.0f;
               wz1 = l*INDOOR_SQUARE_SIZE/10.0f;
            }
            else
            {
               wx2 = k*INDOOR_SQUARE_SIZE/10.0f;
               wz2 = l*INDOOR_SQUARE_SIZE/10.0f;
            }
         }
         else
         {
            /* No Wall */
            if((wx1 != -1) && (wx2 != -1))
            {
               /* Add it */
               dntMap->addWall(wx1, wz1, wx2+10, wz1+10);
            }

            /* Empty buffer */
            wx1 = -1; wz1 = -1;
            wx2 = -1; wz2 = -1;
         }
      }

      /* If have wall at end of line, must add it */
      if((wx1 != -1) && (wx2 != -1))
      {
         /* Add it */
         dntMap->addWall(wx1, wz1, wx2+10, wz1+10);
      }

      /* Empty buffer */
      wx1 = -1; wz1 = -1;
      wx2 = -1; wz2 = -1;
   }

   dntMap->setInitialPosition((sizeX*dntMap->squareSize() / 2.0),
                              (sizeZ*dntMap->squareSize() / 2.0));


   /* Save the Map */
   if(!dntMap->save(mapFile))
   {
      delete(dntMap);
      SDL_FreeSurface(img);

      cerr << "Couldn't save map!" << endl;

      return(false);
   }

   cout << "saved as: " << mapFile << endl;
   delete(dntMap);
   SDL_FreeSurface(img);
   return(true);
}

/***********************************************************************
 *                              showHelp                               *
 ***********************************************************************/
void showHelp()
{
   cout << "heightmap2dnt - create a dnt map from an heightmap image" << endl
        << "Usage:" << endl
        << "\theightmap2dnt -i image -o mapname" << endl
        << "Description:" << endl
        << "\t-i\tDefine image to use as heightmap" << endl
        << "\t-o\tDefine output map filename to create" << endl
        << "\t-w\tTo set the map as indoor" << endl;
   exit(-1);
}

/***********************************************************************
 *                                main                                 *
 ***********************************************************************/
int main(int argc, char** argv)
{
   char option;
   string mapFile="", imgFile="";
   bool indoorMap = false;

   /* get the parameters  */
   while( (option = getopt(argc,argv,"i:o:w") ) != -1)
   {
      switch(option)
      {
         case 'i': /* Input Image */
            if(optarg)
            {
               imgFile = optarg;
            }
            else
            {
               showHelp();
            }
         break;

         case 'o': /* Output map file */
            if(optarg)
            {
               mapFile = optarg;
            }
            else
            {
               showHelp();
            }
         break;
         
         case 'w': /* Indoor Map */
         {
            indoorMap = true;
         }
         break;

         default:  /* Unknow */
            showHelp();
         break;
      }
   }

   /* Verify if all defined */
   if( (mapFile.empty()) || (imgFile.empty()) )
   {
      showHelp();
   }

   /* Init the SDL */
   if( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
   {
      cerr << "Argh! Couldn't init SDL!" << endl;
      exit(-2);
   }
   atexit(SDL_Quit);

   /* Init the screen */
   SDL_Surface* screen = SDL_SetVideoMode(16, 16, 32, SDL_DOUBLEBUF | SDL_OPENGL);
   if(!screen)
   {
      cerr << "Couldn't init screen" << endl;
      exit(-3);
   }

   /* Define data dir */
   dirs dir;
   dir.findDataDirectories();

   if(indoorMap)
   {
      if(doIndoor(imgFile, mapFile))
      {
         cout << "Done." << endl;
      }
      else
      {
         cout << "Failed." << endl;
      }
   }
   else if(doConversion(imgFile, mapFile))
   {
      cout << "Done." << endl;
   }
   else
   {
      cout << "Failed." << endl;
   }


   return(0);
}

