#include "iavariable.h"

/***********************************************************
 *                      Constructor                        *
 ***********************************************************/
iaVariable::iaVariable(string varType, string varName)
{
   type = type;
   name = varName;
   value = NULL;

   if(type == IA_VOID)
   {
      /* Can't define VOID variables */
      cerr << "Script Error! Tried to create a void variable!" << endl;
      value = NULL;
   }
   else if(type == IA_BOOL)
   {
      value = new bool;
   }
   else if(type == IA_INT)
   {
      value = new int;
   }
   else if(type == IA_FLOAT)
   {
      value = new float;
   }
   else if(type == IA_STRING)
   {
      value = new string;
   }
   else if( (type == IA_CHARACTER) || (type == IA_OBJECT) ||
            (type == IA_SKILL) || (type == IA_FEAT))
   {
      /* It'll be only a pointer, so NULL for now. */
      value = NULL;
   }
}

/***********************************************************
 *                       Destructor                        *
 ***********************************************************/
iaVariable::~iaVariable()
{
   if(value)
   {
      if(type == IA_BOOL)
      {
         bool* b = (bool*)value;
         delete(b);
      }
      else if(type == IA_INT)
      {
         int* i = (int*) value;
         delete(i);
      }
      else if(type == IA_FLOAT)
      {
         float* f = (float*)value;
         delete(f);
      }
      else if(type == IA_STRING)
      {
         string* s = (string*)value;
         delete(s);
      }
      /* else don't do anything, since it's only a pointer. */
   }
}

/***********************************************************
 *                           ==                            *
 ***********************************************************/
bool iaVariable::operator==(const iaVariable& v)
{
   if(type != v.type)
   {
      return(false);
   }
   else
   {
      if(type == IA_BOOL)
      {
         bool* ba = (bool*) value;
         bool* bb = (bool*) v.value;
         return( (*ba) == (*bb) );
      }
      else if(type == IA_INT)
      {
         int* ia = (int*) value;
         int* ib = (int*) v.value;
         return( (*ia) == (*ib) );
      }
      else if(type == IA_FLOAT)
      {
         float* fa = (float*) value;
         float* fb = (float*) v.value;
         return( (*fa) == (*fb) );
      }
      else if(type == IA_STRING)
      {
         string* sa = (string*) value;
         string* sb = (string*) v.value;
         return( (*sa) == (*sb) );
      }
      else
      {
         /* the pointer must be the same */
         return( (value) == (v.value) ); 
      }
   }
   return(false);
}

/***********************************************************
 *                           !=                            *
 ***********************************************************/
bool iaVariable::operator!=(const iaVariable& v)
{
   return(!operator==(v));
}

/***********************************************************
 *                           =                             *
 ***********************************************************/
void iaVariable::operator=(const iaVariable& v)
{
   if(type == v.type)
   {
      if(type == IA_BOOL)
      {
         bool* va = (bool*)value;
         bool* vb = (bool*)v.value;
         *va = *vb;
      }
      else if(type == IA_INT)
      {
         int* ia = (int*)value;
         int* ib = (int*)v.value;
         *ia = *ib;
      }
      else if(type == IA_FLOAT)
      {
         float* fa = (float*)value;
         float* fb = (float*)v.value;
         *fa = *fb;
      }
      else if(type == IA_STRING)
      {
         string* sa = (string*)value;
         string* sb = (string*)v.value;
         *sa = *sb;
      }
      else
      {  
         /* Just assign pointers */
         value = v.value;
      }

   }
}

