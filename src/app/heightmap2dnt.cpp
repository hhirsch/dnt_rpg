
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include <string>
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
      printf("Can't load image input file!\n");
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

         color = pixel_Get(img, x, z);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         saux->h1 = (R+G+B) / 2.0;

         color = pixel_Get(img, x, z+10);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         saux->h2 = (R+G+B) / 2.0;

         color = pixel_Get(img, x+10, z+10);
         SDL_GetRGB(color, img->format, &R, &G, &B);
         saux->h3 = (R+G+B) / 2.0;

         color = pixel_Get(img, x+10, z);
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

      printf("Can't save map!\n");

      return(false);
   }


   delete(dntMap);
   SDL_FreeSurface(img);
   return(true);
}

/***********************************************************************
 *                              showHelp                               *
 ***********************************************************************/
void showHelp()
{
   printf("heightmap2dnt - create a dnt map from an heightmap image\n"
          "Usage:\n"
          "\theightmap2dnt -i image -o mapname\n"
          "Description:\n"
          "\t-i\tDefine image to use as heightmap\n"
          "\t-o\tDefine output map filename to create\n");
   exit(-1);
}

/***********************************************************************
 *                                main                                 *
 ***********************************************************************/
int main(int argc, char** argv)
{
   char option;
   string mapFile="", imgFile="";

   /* get the parameters  */
   while( (option = getopt(argc,argv,"i:o:") ) != -1)
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
      printf("Argh! Can't init SDL!\n");
      exit(-2);
   }
   atexit(SDL_Quit);

   /* Init the screen */
   SDL_Surface* screen = SDL_SetVideoMode(16, 16, 32, SDL_DOUBLEBUF | SDL_OPENGL);
   if(!screen)
   {
      printf("Can't init screen\n");
      exit(-3);
   }

   /* Define data dir */
   dirs dir;
   dir.findDataDirectories();


   if(doConversion(imgFile, mapFile))
   {
      printf("Conversion Done.\n");
   }
   else
   {
      printf("Conversion Failed\n");
   }


   return(0);
}

