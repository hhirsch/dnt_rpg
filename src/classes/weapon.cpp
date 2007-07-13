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
   readFile(&categories, &totalCategories, FILE_CATEGORIES);
   /* Read Ranges */
   readFile(&ranges, &totalRanges, FILE_RANGES);
   /* Read Weights */
   readFile(&weights, &totalWeights, FILE_WEIGHTS);
   /* Read Sizes */
   readFile(&sizes, &totalSizes, FILE_SIZES);
   /* Read Damages */
   readFile(&damages, &totalDamages, FILE_DAMAGES);
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
void weaponTypes::readFile(string** names, int* totals, string fileName)
{
   FILE* file;
   int i, index;
   char buffer[255];

   /* Open the file */
   if(!(file=fopen(fileName.c_str(),"r")))
   {
       printf("Error while opening definitions file: %s\n",fileName.c_str());
       return;
   }

   /* Read the total number of itens on the file */
   fscanf(file,"%d", totals);

   /* Alloc the vector */
   *names = new string[*totals];

   /* Read All definitions */
   for(i=0; i < (*totals); i++)
   {
      fscanf(file, "%d %s", &index, &buffer[0]);
      *names[index] = buffer;
   }

   /* Close the file */
   fclose(file);
}

