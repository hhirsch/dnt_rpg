#include "weapon.h"

/************************************************************
 *                        Constructor                       *
 ************************************************************/
weapon::weapon(string fileName): aniModel()
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

