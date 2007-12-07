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
 *                          print                          *
 ***********************************************************/
void iaStack::print()
{
   cerr << "IA Stack Top " << endl;
   int i;
   iaJumpPos* jmp = top;
   for(i=0; i < total; i++)
   {
      cerr << "Command: " << jmp->command << endl;
      jmp = jmp->previous;
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

