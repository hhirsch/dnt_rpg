#include "iastack.h"

#define IA_STACK_TOKEN_BEGIN         "begin"
#define IA_STACK_TOKEN_END           "end"
#define IA_STACK_TOKEN_LINE_NUMBER   "lineNumber"
#define IA_STACK_TOKEN_COMMAND       "command"

#define IA_STACK_TOKEN_END_STACK     "endStack"

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
iaStack::iaStack()
{
   top = NULL;
   bottom = NULL;
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

   /* Update bottom pointer */
   if(top == bottom)
   {
      bottom = NULL;
   }

   /* Update Top Pointer */
   if(top)
   {
      top = top->previous;
   }
   else
   {
      top = NULL;
   }

   /* Update Total Counter */
   if(total > 0)
   {
      total--;
   }

   /* Return the top */
   return(jmp);
}

/***********************************************************
 *                           push                          *
 ***********************************************************/
void iaStack::push(iaJumpPos* jmp)
{
   jmp->previous = top;
   top = jmp;

   if(bottom == NULL)
   {
      bottom = jmp;
   }

   total++;
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
 *                       insertAtBottom                    *
 ***********************************************************/
void iaStack::insertAtBottom(iaJumpPos* jmp)
{
   /* Set the top, if needed */
   if(total == 0)
   {
      top = jmp;
   }

   /* Set the previous, if needed */
   if(bottom)
   {
      bottom->previous = jmp;
   }

   /* Insert at bottom and inc total */
   bottom = jmp;
   jmp->previous = NULL;
   total++;
}

/***********************************************************
 *                          save                           *
 ***********************************************************/
void iaStack::save(ofstream* file)
{
   int i;
   iaJumpPos* jmp = top;
   for(i=0; i < total; i++)
   {
      *file << IA_STACK_TOKEN_BEGIN << " = " << jmp->begin << endl;
      *file << IA_STACK_TOKEN_END << " = " << jmp->end << endl;
      *file << IA_STACK_TOKEN_LINE_NUMBER << " = " << jmp->lineNumber << endl;
      *file << IA_STACK_TOKEN_COMMAND << " = " << jmp->command << endl;

      jmp = jmp->previous;
   }
   *file << IA_STACK_TOKEN_END_STACK << " = " << "END" << endl;
}

/***********************************************************
 *                          load                           *
 ***********************************************************/
void iaStack::load(defParser* def)
{
   string value="", key="";
   iaJumpPos* jmp = NULL;
   int pos=0;

   while(def->getNextTuple(key, value))
   {
      if(key == IA_STACK_TOKEN_END_STACK)
      {
         /* Done with the stack */
         return;
      }
      else if(key == IA_STACK_TOKEN_BEGIN)
      {
         /* The begin position of a jump. It also mark the
          * local to put a new jump on the stack */
         jmp = new iaJumpPos();
         insertAtBottom(jmp);
         sscanf(value.c_str(),"%d",&pos);
         jmp->begin = pos;
      }
      else if(key == IA_STACK_TOKEN_END)
      {
         /* The end position of a jump */
         if(jmp)
         {
            sscanf(value.c_str(),"%d",&pos);
            jmp->end = pos;
         }
         else
         {
            cerr << "Warning: No iaJumpPos for jump at loading stack!" << endl;
         }
      }
      else if(key == IA_STACK_TOKEN_LINE_NUMBER)
      {
         /* The line Number of the jump */
         if(jmp)
         {
            sscanf(value.c_str(),"%d",&jmp->lineNumber);
         }
         else
         {
            cerr << "Warning: No iaJumpPos for jump at loading stack!" << endl;
         }
      }
      else if(key == IA_STACK_TOKEN_COMMAND)
      {
         /* The jump command */
         if(jmp)
         {
            jmp->command = value;
         }
         else
         {
            cerr << "Warning: No iaJumpPos for jump at loading stack!" << endl;
         }
      }
      else
      {
         cerr << "Warning: Unknow token '" << key << "' at stack load!"<< endl;
      }
   }
}

