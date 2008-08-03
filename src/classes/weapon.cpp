#include "weapon.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"

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
weapon::weapon(string path, modelList& mdlList, weaponTypes& wTypes): object()
{
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
       printf("Error on open object %s\n",
              dir.getRealFile(path).c_str());
       return;
   }
   fileName = path;

   while(parser.getNextTuple(key,value))
   {
      if(value == "")
      {
         printf("at file: %s\n",path.c_str());
      }

      /* Tokenize the first key */
      if(key == "name")
      {
         name = translateDataString(value);
      }
      else if(key == "cal3d")
      {
         /* Add the model. The weapon is never a scenery. */
         model3D = mdlList.addModel(value,"", false);
         model3D->incUsed();
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
            printf("Can't open image: %s\nWill Crash Soon!\n", 
                   dir.getRealFile(model2dName).c_str());
         }
      }
      else if(key == "life_points")
      {
         sscanf(value.c_str(),"%d",&maxLifePoints);
         lifePoints = maxLifePoints;
      }
      else if(key == "fortitude")
      {
         sscanf(value.c_str(),"%d",&fortitude);
      }
      else if(key == "reflex")
      {
         sscanf(value.c_str(),"%d",&reflex);
      }
      else if( (key == "will") || (key == "i_am_not_a_fool") )
      {
         sscanf(value.c_str(),"%d",&iAmNotAFool);
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
         actualMunition = munitionCapacity;
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
      else
      {
         printf("Warning: Unknow key '%s' at %s\n", key.c_str(), 
                                                      path.c_str());
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
 *                          getRange                        *
 ************************************************************/
wInfo* weapon::getRangeType()
{
   return(rangeType);
}

/************************************************************
 *                         getWeight                        *
 ************************************************************/
float weapon::getWeight()
{
   return(weight);
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
weaponTypes::weaponTypes()
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
weaponTypes::~weaponTypes()
{
   delete[] categories;
   delete[] ranges;
   delete[] weights;
   delete[] sizes;
   delete[] damages;
   delete[] munitions;
}

/************************************************************
 *                          readFile                        *
 ************************************************************/
void weaponTypes::readFile(string fileName)
{
   FILE* file;
   int i, index;
   char buffer[255];
   char buffer2[255];

   int totals;
   wInfo* names = NULL;

   /* Open the file */
   dirs dir;
   if(!(file=fopen(dir.getRealFile(fileName).c_str(),"r")))
   {
       printf("Error while opening definitions file: %s\n",fileName.c_str());
       return;
   }

   /* Read the total number of itens on the file */
   fscanf(file,"%d", &totals);

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
      fclose(file);
      printf("Unkown definitions file: %s\n", fileName.c_str());
      return;
   }

   /* Read All definitions */
   for(i=0; i < totals; i++)
   {
      fscanf(file, "%d %s ", &index, &buffer[0]);
      /* eat up the rest of line */
      fgets(buffer2, sizeof(buffer2), file);

      names[index].name = buffer;
      names[index].index = index;
      names[index].title = translateDataString(buffer2);
   }

   /* Close the file */
   fclose(file);
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
   printf("Can't found weapon definition type of: %s\n", name.c_str());
   return(NULL);
}

