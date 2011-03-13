/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "weapon.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include "../sound/sound.h"

#include <iostream>
using namespace std;

/************************************************************
 *                        Constructor                       *
 ************************************************************/
wInfo::wInfo()
{
   name = "";
   index = -1;
   title = "";
};

/************************************************************
 *                          Assign                          *
 ************************************************************/
void wInfo::operator=(wInfo& v)
{
   name = v.name;
   index = v.index;
   title = v.title;
};

/************************************************************
 *                        Constructor                       *
 ************************************************************/
weapon::weapon(string path): object(path)
{
   weaponTypes wTypes;
   dirs dir;
   string key="", value="";

   /* Empty Values */
   cleanValues();
   categoryType = NULL;
   rangeType = NULL;
   sizeType = NULL;
   weightType = NULL;
   damageType[0] = NULL;
   damageType[1] = NULL;

   type = OBJECT_TYPE_WEAPON;

   /* Load Parser */
   defParser parser;
   if(!parser.load(path))
   {
       cerr << "Error openning object " <<  dir.getRealFile(path) << endl;
       return;
   }
   fileName = path;

   while(parser.getNextTuple(key,value))
   {
      /* Tokenize the first key */
      if(key == "name")
      {
         name = translateDataString(value);
      }
      else if(key == "description")
      {
         description = translateDataString(value);
      }
      else if(key == "cal3d")
      {
         /* Add the model. The weapon is never a scenery. */
         cal3dName = value;
         scNode = NULL;
         createSceneNode(0,0,0,0,0,0);
      }
      else if( (key == "inventory_sizes") || (key == "sizeValue"))
      {
         sscanf(value.c_str(),"%d %d",&inventSizeX, &inventSizeY);
      }
      else if(key == "inventory_texture")
      {
         model2dName = value;
         model2d = IMG_Load(dir.getRealFile(model2dName).c_str());
         if(!model2d)
         {
            cerr << "Couldn't open inventory image: " 
               << dir.getRealFile(model2dName) 
               << "! Will crash soon!\n" << endl;
         }
      }
      else if(key == "life_points")
      {
         sscanf(value.c_str(),"%d",&maxLifePoints);
         lifePoints = maxLifePoints;
      }
      else if(key == "fortitude")
      {
         sscanf(value.c_str(),"%d",&curBonusAndSaves.fortitude);
      }
      else if( (key == "reflex") || (key == "reflexes"))
      {
         sscanf(value.c_str(),"%d",&curBonusAndSaves.reflexes);
      }
      else if( (key == "will") || (key == "i_am_not_a_fool") )
      {
         sscanf(value.c_str(),"%d",&curBonusAndSaves.iAmNotAFool);
      }
      else if(key == "displacement")
      {
         sscanf(value.c_str(),"%d",&displacement);
      }
      else if(key == "armature_class")
      {
         sscanf(value.c_str(),"%d",&armatureClass);
      }
      else if(key == "size_modifier")
      {
         sscanf(value.c_str(),"%d",&sizeModifier);
      }
      else if(key =="cost") 
      {
         sscanf(value.c_str(),"%f",&cost);
      }
      else if(key == "eqAngles")
      {
         sscanf(value.c_str(),"%f,%f,%f",&eqAngleX, &eqAngleY, &eqAngleZ);
      }
      else if(key == "eqTrans1")
      {
         sscanf(value.c_str(),"%f,%f,%f",
               &eqTrans1[0], &eqTrans1[1], &eqTrans1[2]);
      }
      else if(key == "eqTrans2")
      {
         sscanf(value.c_str(),"%f,%f,%f",
               &eqTrans2[0], &eqTrans2[1], &eqTrans2[2]);
      }


      /* The weapons definitions */
      else if( (key == "category") || (key == "category_type"))
      {
         categoryType = wTypes.getCategory(value);
      }
      else if(key == "range_type")
      {
         rangeType = wTypes.getRange(value);
      }
      else if(key == "range_value")
      {
         sscanf(value.c_str(),"%d",&rangeValue);
      }
      else if( (key == "size_type") || (key == "size"))
      {
         sizeType = wTypes.getSize(value);
      }
      else if(key == "weight_type")
      {
         weightType = wTypes.getWeight(value);
      }
      else if((key == "weight_value") || (key == "weight"))
      {
         sscanf(value.c_str(),"%f",&weight);
      }
      else if(key == "munition_type")
      {
         munitionType = wTypes.getMunition(value);
      }
      else if(key == "munition_capacity")
      {
         sscanf(value.c_str(),"%d",&munitionCapacity);
         state = munitionCapacity;
      }
      else if(key == "main_damage_type")
      {
         damageType[0] = wTypes.getDamage(value);
      }
      else if(key == "main_attack_sound")
      {
         attackSound[0] = value;
      }
      else if(key == "second_damage_type")
      {
         damageType[1] = wTypes.getDamage(value);
      }
      else if(key == "second_attack_sound")
      {
         attackSound[1] = value;
      }
      else if(key == "damage_dice")
      {
         int numberOfDices=0, diceID=0, sumNumber=0, criticalMultiplier=0;

         /* Read base dice information */
         sscanf(value.c_str(), "%d*d%d+%dx%d",
                &numberOfDices, &diceID, &sumNumber, &criticalMultiplier);
         damageDice.baseDice.setType(diceID);
         damageDice.baseDice.setNumberOfDices(numberOfDices);
         damageDice.baseDice.setSumNumber(sumNumber);
         damageDice.baseDice.setCriticalMultiplier(criticalMultiplier);

         /* Aditional Dice not used */
         damageDice.aditionalDice.setNumberOfDices(0);
         damageDice.aditionalDice.setType(DICE_D2);
         damageDice.aditionalDice.setSumNumber(0);
         damageDice.aditionalDice.setCriticalMultiplier(0);
         damageDice.initialLevel = 1;
         damageDice.aditionalLevels = 0;
      }
      else if(key == "instant_kill")
      {
         sscanf(value.c_str(), "%d", &damageDice.instantKill);
      }
      else
      {
         cerr << "Warning: Unknow key '" << key << "' at " << path << endl;
      }
   }

}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
weapon::~weapon()
{
   //Do not need to dec the model3D, cause it is already done at the
   //destructor of the object. Also, do not need to free the model2D, since
   //is is done at the object destructor.
}

/************************************************************
 *                           getType                        *
 ************************************************************/
void weapon::getType(wInfo& category, wInfo& range, wInfo& size, wInfo& weight,
                     wInfo& damageA, wInfo& damageB)
{
   if(categoryType)
   {
      category = *categoryType;
   }

   if(rangeType)
   {
      range = *rangeType;
   }

   if(sizeType)
   {
      size = *sizeType;
   }

   if(weightType)
   {
      weight = *weightType;
   }

   if(damageType[0])
   {
      damageA = *damageType[0];
   }

   if(damageType[1])
   {
      damageB = *damageType[1];
   }
}

/************************************************************
 *                          getDice                         *
 ************************************************************/
diceThing weapon::getDice()
{
   return(damageDice);
}

/************************************************************
 *                          getRange                        *
 ************************************************************/
int weapon::getRange()
{
   return(rangeValue);
}

/************************************************************
 *                        getRangeType                      *
 ************************************************************/
wInfo* weapon::getRangeType()
{
   return(rangeType);
}

/************************************************************
 *                       getMunitionType                    *
 ************************************************************/
wInfo* weapon::getMunitionType()
{
   return(munitionType);
}

/************************************************************
 *                         getWeight                        *
 ************************************************************/
float weapon::getWeight()
{
   return(weight);
}

/************************************************************
 *                    getMunitionCapacity                   *
 ************************************************************/
int weapon::getMunitionCapacity()
{
   return(munitionCapacity);
}

/************************************************************
 *                    getCurrentMunition                    *
 ************************************************************/
int weapon::getCurrentMunition()
{
   return(state);
}

/************************************************************
 *                    setCurrentMunition                    *
 ************************************************************/
void weapon::setCurrentMunition(int v)
{
   state = v;
}

/************************************************************
 *                    playMainAttackSound                   *
 ************************************************************/
void weapon::playMainAttackSound(float x, float y, float z)
{
   if(!attackSound[0].empty())
   {
      sound snd;
      snd.addSoundEffect(x, y, z, SOUND_NO_LOOP, attackSound[0]);
   }
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                              WEAPON TYPES                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#define FILE_CATEGORIES "weapons/types/categories.dcl"
#define FILE_RANGES "weapons/types/ranges.dcl"
#define FILE_SIZES "weapons/types/sizes.dcl"
#define FILE_WEIGHTS "weapons/types/weights.dcl"
#define FILE_DAMAGES "weapons/types/damages.dcl"
#define FILE_MUNITIONS "weapons/types/munitions.dcl"

/************************************************************
 *                        Constructor                       *
 ************************************************************/
void weaponTypes::init()
{
   /* Read Categories */
   readFile(FILE_CATEGORIES);
   /* Read Ranges */
   readFile(FILE_RANGES);
   /* Read Weights */
   readFile(FILE_WEIGHTS);
   /* Read Sizes */
   readFile(FILE_SIZES);
   /* Read Damages */
   readFile(FILE_DAMAGES);
   /* Read Munitions */
   readFile(FILE_MUNITIONS);
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
void weaponTypes::finish()
{
   if(categories)
   {
      delete[] categories;
   }
   if(ranges)
   {
      delete[] ranges;
   }
   if(weights)
   {
      delete[] weights;
   }
   if(sizes)
   {
      delete[] sizes;
   }
   if(damages)
   {
      delete[] damages;
   }
   if(munitions)
   {
      delete[] munitions;
   }
}

/************************************************************
 *                          readFile                        *
 ************************************************************/
void weaponTypes::readFile(string fileName)
{
   ifstream file;
   int i, index;
   string strBuffer;
   string::size_type pos;

   int totals;
   wInfo* names = NULL;

   /* Open the file */
   dirs dir;
   file.open(dir.getRealFile(fileName).c_str(), ios::in | ios::binary);
   if(!file)
   {
       cerr << "Error while opening definitions file: " 
            << fileName << endl;
       return;
   }

   /* Read the total number of itens on the file */
   getline(file, strBuffer);
   sscanf(strBuffer.c_str(),"%d", &totals);

   /* Alloc the vector */
   names = new wInfo[totals];

   /* Define the correct pointers */
   if(fileName == FILE_CATEGORIES)
   {
      categories = names;
      totalCategories = totals;
   }
   else if(fileName == FILE_RANGES)
   {
      ranges = names;
      totalRanges = totals;
   } 
   else if(fileName == FILE_SIZES)
   {
      sizes = names;
      totalSizes = totals;
   }
   else if(fileName == FILE_WEIGHTS)
   {
      weights = names;
      totalWeights = totals;
   }
   else if(fileName == FILE_DAMAGES)
   {
      damages = names;
      totalDamages = totals;
   }
   else if(fileName == FILE_MUNITIONS)
   {
      munitions = names;
      totalMunitions = totals;
   }
   else
   {
      file.close();
      cerr << "Error: Unkown definitions file: " << fileName << endl;
      return;
   }

   /* Read All definitions */
   for(i=0; i < totals; i++)
   {
      getline(file, strBuffer);

      /* Get the index */
      sscanf(strBuffer.c_str(), "%d ", &index);
      names[index].index = index;

      /* Get the name */
      pos = strBuffer.find_first_not_of(" \t");
      pos = strBuffer.find_first_not_of("0123456789", pos);
      pos = strBuffer.find_first_not_of(" \t", pos);
      if((pos != string::npos) && (strBuffer[pos] != '\n'))
      {
         names[index].name = strBuffer.substr(pos, 
                                 strBuffer.find_first_of(" \t\n\r", pos) - pos);
         pos += names[index].name.size();
      }

      /* Get the title */
      pos = strBuffer.find_first_not_of(" \t\n\r",pos); 
      if((pos != string::npos) && (strBuffer[pos] != '\n'))
      {
         names[index].title = translateDataString(strBuffer.substr(pos, 
                               strBuffer.find_first_of("\t\n\r\0", pos) - pos));
      }
      
   }

   /* Close the file */
   file.close();
}

/************************************************************
 *                        getCategory                       *
 ************************************************************/
wInfo* weaponTypes::getCategory(string name)
{
   return(getThing(categories, totalCategories, name));
}

/************************************************************
 *                         getSize                          *
 ************************************************************/
wInfo* weaponTypes::getSize(string name)
{
   return(getThing(sizes, totalSizes, name));
}

/************************************************************
 *                         getRage                          *
 ************************************************************/
wInfo* weaponTypes::getRange(string name)
{
   return(getThing(ranges, totalRanges, name));
}


/************************************************************
 *                         getWeight                        *
 ************************************************************/
wInfo* weaponTypes::getWeight(string name)
{
   return(getThing(weights, totalWeights, name));
}

/************************************************************
 *                         getDamage                        *
 ************************************************************/
wInfo* weaponTypes::getDamage(string name)
{
   return(getThing(damages, totalDamages, name));
}

/************************************************************
 *                        getMunition                       *
 ************************************************************/
wInfo* weaponTypes::getMunition(string name)
{
   return(getThing(munitions, totalMunitions, name));
}


/************************************************************
 *                          getThing                        *
 ************************************************************/
wInfo* weaponTypes::getThing(wInfo* thing, int total, string name)
{
   int i;
   for(i=0; i<total; i++)
   {
      if(thing[i].name == name)
      {
         return(&thing[i]);
      }
   }
   cerr << "Couldn't find weapon definition type of " << name << endl;
   return(NULL);
}


/************************************************************
 *                     Static Members                       *
 ************************************************************/
int weaponTypes::totalCategories = 0;
wInfo* weaponTypes::categories = NULL;
int weaponTypes::totalRanges = 0;
wInfo* weaponTypes::ranges = NULL;
int weaponTypes::totalSizes = 0;
wInfo* weaponTypes::sizes = NULL;
int weaponTypes::totalWeights = 0;
wInfo* weaponTypes::weights = NULL;
int weaponTypes::totalDamages = 0;
wInfo* weaponTypes::damages = NULL;
int weaponTypes::totalMunitions = 0;
wInfo* weaponTypes::munitions = NULL;

