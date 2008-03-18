#include "weapon.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"

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
   FILE* file;
   char buffer[512];
   string token, token2;

   cleanValues();
   categoryType = NULL;
   rangeType = NULL;
   sizeType = NULL;
   weightType = NULL;
   damageType[0] = NULL;
   damageType[1] = NULL;

   type = OBJECT_TYPE_WEAPON;

   if(!(file=fopen(dir.getRealFile(path).c_str(),"r")))
   {
       printf("Error on open object %s\n",
              dir.getRealFile(path).c_str());
       return;
   }

   fileName = path;

   while(fscanf(file, "%s", buffer) != EOF)
   {
      token = buffer;

      /* eat up the rest of line */
      fgets(buffer, sizeof(buffer), file);
      token2 = getAfterEqual(buffer);

      if(token2 == "")
      {
         printf("at file: %s\n",path.c_str());
      }

      /* Tokenize the first token */
      if(token == "name")
      {
         name = translateDataString(token2);
      }
      else if(token == "cal3d")
      {
         model3D = mdlList.addModel(token2,"");
         model3D->incUsed();
      }
      else if( (token == "inventory_sizes") || (token == "sizeValue"))
      {
         sscanf(token2.c_str(),"%d %d",&inventSizeX, &inventSizeY);
      }
      else if(token == "inventory_texture")
      {
         model2dName = token2;
         model2d = IMG_Load(dir.getRealFile(model2dName).c_str());
         if(!model2d)
         {
            printf("Can't open image: %s\nWill Crash Soon!\n", 
                   dir.getRealFile(model2dName).c_str());
         }
      }
      else if(token == "life_points")
      {
         sscanf(token2.c_str(),"%d",&maxLifePoints);
         lifePoints = maxLifePoints;
      }
      else if(token == "fortitude")
      {
         sscanf(token2.c_str(),"%d",&fortitude);
      }
      else if(token == "reflex")
      {
         sscanf(token2.c_str(),"%d",&reflex);
      }
      else if(token == "will")
      {
         sscanf(token2.c_str(),"%d",&will);
      }
      else if(token == "displacement")
      {
         sscanf(token2.c_str(),"%d",&displacement);
      }
      else if(token == "armature_class")
      {
         sscanf(token2.c_str(),"%d",&armatureClass);
      }
      else if(token == "size_modifier")
      {
         sscanf(token2.c_str(),"%d",&sizeModifier);
      }
      else if(token =="cost") 
      {
         sscanf(token2.c_str(),"%f",&cost);
      }

      /* The weapons definitions */
      else if( (token == "category") || (token == "category_type"))
      {
         categoryType = wTypes.getCategory(token2);
      }
      else if(token == "range_type")
      {
         rangeType = wTypes.getRange(token2);
      }
      else if(token == "range_value")
      {
         sscanf(token2.c_str(),"%d",&rangeValue);
      }
      else if( (token == "size_type") || (token == "size"))
      {
         sizeType = wTypes.getSize(token2);
      }
      else if(token == "weight_type")
      {
         weightType = wTypes.getWeight(token2);
      }
      else if((token == "weight_value") || (token == "weight"))
      {
         sscanf(token2.c_str(),"%f",&weight);
      }
      else if(token == "munition_type")
      {
         munitionType = wTypes.getMunition(token2);
      }
      else if(token == "munition_capacity")
      {
         sscanf(token2.c_str(),"%d",&munitionCapacity);
         actualMunition = munitionCapacity;
      }
      else if(token == "main_damage_type")
      {
         damageType[0] = wTypes.getDamage(token2);
      }
      else if(token == "main_attack_sound")
      {
         attackSound[0] = token2;
      }
      else if(token == "second_damage_type")
      {
         damageType[1] = wTypes.getDamage(token2);
      }
      else if(token == "second_attack_sound")
      {
         attackSound[1] = token2;
      }
      else if(token == "damage_dice")
      {
         sscanf(token2.c_str(),
                "%d*d%d+%dx%d",&damageDice.baseDice.numberOfDices,
                               &damageDice.baseDice.diceID,
                               &damageDice.baseDice.sumNumber,
                               &damageDice.baseDice.criticalMultiplier);
         damageDice.aditionalDice.numberOfDices = 0;
         damageDice.aditionalDice.diceID = 2;
         damageDice.aditionalDice.sumNumber = 0;
         damageDice.aditionalDice.criticalMultiplier = 0;
         damageDice.initialLevel = 1;
         damageDice.aditionalLevels = 0;
      }
      else
      {
         printf("Warning: Unknow token '%s' at %s\n", token.c_str(), 
                                                      path.c_str());
      }
   }

   fclose(file);
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
weapon::~weapon()
{
   //Do not need to dec the model3D, since it is already done at the
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

