/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
  
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/


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

