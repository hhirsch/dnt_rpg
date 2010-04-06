#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
using namespace std;

/***********************************************************************
 *                             printHelp                               *
 ***********************************************************************/
void printHelp(char* progName)
{
   cerr << "DNT sound test application" << endl << endl;
   cerr << "  Usage " << progName << " ogg_file.ogg " << endl;
}

/***********************************************************************
 *                                main                                 *
 ***********************************************************************/
int main(int argc, char* argv[])
{
   sound snd;
   string fileName="";

   /* Get filename from command line */
   if(argv[1] != NULL)
   {
      fileName = argv[1];
   }

   /* Verify it */
   if(fileName.empty())
   {
      printHelp(argv[0]);
      return(0);
   }

   /* Init sound system */
   snd.init();

   /* Load the music */
   if(!snd.loadMusic(fileName))
   {
      cerr << "Couldn't load " << fileName << "!" << endl;
   }
   else
   {
      sleep(10);
   }

   /* Exit the sound */
   snd.finish();

   return(1);
}

