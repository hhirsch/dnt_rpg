#include "weapon.h"

/************************************************************
 *                        Constructor                       *
 ************************************************************/
weapon::weapon(string fileName): object()
{
   //TODO read from file
   //
   //loadModel(name);
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
weapon::~weapon()
{
}

/************************************************************
 *                           getType                        *
 ************************************************************/
void weapon::getType(int& category, int& range, int& size, int& style,
                     int& damageA, int& damageB)
{
   category = categoryType;
   range = rangeType;
   size = sizeType;
   style = styleType;
   damageA = damageType[0];
   damageB = damageType[1];
}

/************************************************************
 *                          getCost                         *
 ************************************************************/
float weapon::getCost()
{
   return(cost);
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
 *                         getWeight                        *
 ************************************************************/
float weapon::getWeight()
{
   return(weight);
}

/************************************************************
 *                      attackAnimation                     *
 ************************************************************/
void weapon::attackAnimation()
{
   //TODO
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                              WEAPON TYPES                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#define FILE_CATEGORIES "../data/weapons/types/categories.dcl"
#define FILE_RANGES "../data/weapons/types/ranges.dcl"
#define FILE_SIZES "../data/weapons/types/sizes.dcl"
#define FILE_WEIGHTS "../data/weapons/types/weights.dcl"
#define FILE_DAMAGES "../data/weapons/types/damages.dcl"

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
}

/************************************************************
 *                          readFile                        *
 ************************************************************/
void weaponTypes::readFile(string fileName)
{
   FILE* file;
   int i, index;
   char buffer[255];

   int totals;
   wInfo* names = NULL;

   /* Open the file */
   if(!(file=fopen(fileName.c_str(),"r")))
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
   else
   {
      fclose(file);
      printf("Unkown definitions file: %s\n", fileName.c_str());
      return;
   }

   /* Read All definitions */
   for(i=0; i < totals; i++)
   {
      fscanf(file, "%d %s", &index, &buffer[0]);
      names[index].name = buffer;
      names[index].index = index;
   }

   /* Close the file */
   fclose(file);
}

/************************************************************
 *                        getCategory                       *
 ************************************************************/
int weaponTypes::getCategory(string name)
{
   return(getThing(categories, totalCategories, name));
}

/************************************************************
 *                         getSize                          *
 ************************************************************/
int weaponTypes::getSize(string name)
{
   return(getThing(sizes, totalSizes, name));
}

/************************************************************
 *                         getWeight                        *
 ************************************************************/
int weaponTypes::getWeight(string name)
{
   return(getThing(weights, totalWeights, name));
}

/************************************************************
 *                         getDamage                        *
 ************************************************************/
int weaponTypes::getDamage(string name)
{
   return(getThing(damages, totalDamages, name));
}

/************************************************************
 *                          getThing                        *
 ************************************************************/
int weaponTypes::getThing(wInfo* thing, int total, string name)
{
   int i;
   for(i=0; i<total; i++)
   {
      if(thing[i].name == name)
      {
         return(thing[i].index);
      }
   }
   printf("Can't found weapon definition type of: %s\n", name.c_str());
   return(-1);
}

