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

#include "xp.h"

/*! Amount of XP needed per Level */
int xpPerLevelTable[20] =
{100, 1100, 3100, 6100, 10100, 15100, 21100, 28100, 36100, 45100,
 55100, 66100, 78100, 91100, 105100, 121100, 136100, 153100, 171100, 
 190100
};

/*! The XP table for CR (x->CR y->actorLevel)*/
int xpTable[20][20] = 
{
/* 1st Level */
{30,60,90,135,180,270,360,540,720,1080,
1420,2200,2700,3650,4560,4780,5000,5460,5900,6800},
/* 2nd Level */
{30,60,90,135,180,270,360,540,720,1080,
1420,2200,2700,3650,4560,4780,5000,5460,5900,6800},
/* 3rd Level */
{30,60,90,135,180,270,360,540,720,1080,
1420,2200,2700,3650,4560,4780,5000,5460,5900,6800},
/* 4th Level */
{30,60,80,120,160,240,320,480,640, 960,
1280,2000,2520,3540,4420,4510,4900,5280,5850,6500},
/* 5th Level */
{30,50,75,100,150,225,300,450,600, 900,
1200,1800,2350,3200,4210,4400,4800,5120,5750,6200},
/* 6th Level */
{30,45,60, 90,120,180,270,360,540, 720,
1080,1680,2160,2780,3780,3950,4600,4780,5650,5975},
/* 7th Level */
{26,39,52, 70,105,140,210,315,420, 630,
 840,1260,1680,2520,3450,3650,4400,4650,5450,5650},
/* 8th Level */
{20,30,45, 60, 80,120,160,240,360, 480,
 720, 960,1440,1920,2880,3470,4200,4445,5200,5425},
/* 9th Level */
{18,22,33, 50, 67, 90,135,180,270, 405,
 540, 810,1080,1620,2160,3240,3800,4320,4950,5322},
/* 10th Level */
{13,18,25, 37, 56, 75,100,150,200, 300,
 450, 600, 900,1200,1800,2400,3600,4210,4780,5256},
/* 11th Level */
{10,15,20, 27, 41, 61, 82,110,165, 220,
 330, 495, 660, 990,1320,1980,2640,3960,4500,5035},
/* 12th Level */
{ 8,12,17, 22, 30, 45, 67, 90,120, 180,
 240, 360, 540, 720,1080,1440,2160,2880,4320,4890},
/* 13th Level */
{ 7,10,15, 16, 23, 32, 48, 73, 97, 130,
 195, 260, 390, 585, 780,1170,1560,2340,3120,4680},
/* 14th Level */
{ 6, 8,13, 14, 17, 25, 35, 52, 78, 105,
140, 210, 280, 420, 630, 840,1260,1680,2520,3360},
/* 15th Level */
{ 5, 7,10, 11, 13, 20, 28, 37, 56,  84,
112, 150, 225, 300, 450, 675, 900,1350,1800,2700},
/* 16th Level */
{ 4, 6, 8,  9, 10, 14, 22, 29, 40,  69,
  90, 120, 160, 240, 320, 480, 720, 960,1440,1920},
/* 17th Level */
{ 3, 5, 7,  7,  8, 11, 16, 23, 33,  42,
  63,  95, 127, 170, 255, 340, 510, 765,1020,1530},
/* 18th Level */
{ 2, 4, 6,  6,  7,  9, 14, 17, 27,  30,
  45,  67, 101, 135, 180, 270, 360, 540, 810,1080},
/* 19th Level */
{ 1, 2, 4,  5,  6,  7, 10, 13, 20,  24,
  32,  47,  71, 106, 142, 190, 285, 380, 570, 855},
/* 20th Level */
{ 1, 1, 2,  3,  4,  5,  8, 10, 14,  18,
  25,  32,  50,  75, 100, 150, 200, 300, 400, 600}
};

/************************************************************
 *                        getXP                             *
 ************************************************************/
int getXP(character* actualActor, float targetCR)
{
   int i;
   int level = 0;

   /* CR < 0, means no XP! */
   if(targetCR <= 0)
   {
      return(0);
   }

   /* Calculate the divider */
   if(targetCR < 1)
   {
      targetCR = 1;
   }


   /* Calculate the actor level */
   for(i=0; i<MAX_DISTINCT_CLASSES; i++)
   {
      if(actualActor->actualClass[i] != NULL)
      {
         level += actualActor->classLevels[i];
      }
   }

   if( (level >= 1) && (level <= 20))
   {
      if( (targetCR >= 1) && (targetCR <= 20) )
      {
         /* Get directly from table */
         int tablePos = (int)targetCR-1;
         return(xpTable[level-1][tablePos]);
      }
      else if(targetCR > 20)
      {
         /* TODO Calculate */
      }
   
   }
   else if(level > 20)
   {
      /* TODO epical levels! */
   }

   return(0);
}

/************************************************************
 *                     nextLevelXP                          *
 ************************************************************/
int nextLevelXP(int curXP)
{
   int i = 0;
   while( (curXP >= xpPerLevelTable[i]) && (i < 19) )
   {
      i++;
   }

   if(i < 19)
   {
      return(xpPerLevelTable[i]);
   }
   else
   {
      /* TODO epical levels! */
   }

   return(0);

}

/************************************************************
 *                       getXPLevel                         *
 ************************************************************/
int getXPLevel(int curXP)
{
   int i = 0;
   while( (curXP >= xpPerLevelTable[i]) && (i < 19) )
   {
      i++;
   }

   return(i+1);
}

