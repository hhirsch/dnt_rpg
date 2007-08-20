#include "iastack.h"


/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
iaStack::iaStack()
{
   top = NULL;
   total = 0;
}

/***********************************************************
 *                       Destructor                        *
 ***********************************************************/
iaStack::~iaStack()
{
   iaJumpPos* jmp = pop();
   while(jmp != NULL)
   {
      delete(jmp);
      jmp = pop();
   }
}

/***********************************************************
 *                            pop                          *
 ***********************************************************/
iaJumpPos* iaStack::pop()
{
   iaJumpPos* jmp = top;

   if(top)
   {
      top = top->previous;
   }
   else
   {
      top = NULL;
   }

   if(total > 0)
   {
      total--;
   }

   return(jmp);
}

/***********************************************************
 *                           push                          *
 ***********************************************************/
void iaStack::push(iaJumpPos* jmp)
{
   jmp->previous = top;
   top = jmp;
   total++;
}

