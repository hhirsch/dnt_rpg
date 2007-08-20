#include "iavariable.h"

/***********************************************************
 *                      Constructor                        *
 ***********************************************************/
iaVariable::iaVariable(string varType, string varName)
{
   type = type;
   name = varName;
   value = NULL;

   if(type == IA_TYPE_VOID)
   {
      /* Can't define VOID variables */
      cerr << "Script Error! Tried to create a void variable!" << endl;
      value = NULL;
   }
   else if(type == IA_TYPE_BOOL)
   {
      value = new bool;
   }
   else if(type == IA_TYPE_INT)
   {
      value = new int;
   }
   else if(type == IA_TYPE_FLOAT)
   {
      value = new float;
   }
   else if(type == IA_TYPE_STRING)
   {
      value = new string;
   }
   else if( (type == IA_TYPE_CHARACTER) || (type == IA_TYPE_OBJECT) ||
            (type == IA_TYPE_SKILL) || (type == IA_TYPE_FEAT))
   {
      /* It'll be only a pointer, so NULL for now. */
      value = NULL;
   }
   else
   {
      cerr << "Unknow Type: " << type << endl;
   }
}

/***********************************************************
 *                       Destructor                        *
 ***********************************************************/
iaVariable::~iaVariable()
{
   if(value)
   {
      if(type == IA_TYPE_BOOL)
      {
         bool* b = (bool*)value;
         delete(b);
      }
      else if(type == IA_TYPE_INT)
      {
         int* i = (int*) value;
         delete(i);
      }
      else if(type == IA_TYPE_FLOAT)
      {
         float* f = (float*)value;
         delete(f);
      }
      else if(type == IA_TYPE_STRING)
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
      if(type == IA_TYPE_BOOL)
      {
         bool* ba = (bool*) value;
         bool* bb = (bool*) v.value;
         return( (*ba) == (*bb) );
      }
      else if(type == IA_TYPE_INT)
      {
         int* ia = (int*) value;
         int* ib = (int*) v.value;
         return( (*ia) == (*ib) );
      }
      else if(type == IA_TYPE_FLOAT)
      {
         float* fa = (float*) value;
         float* fb = (float*) v.value;
         return( (*fa) == (*fb) );
      }
      else if(type == IA_TYPE_STRING)
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
      if(type == IA_TYPE_BOOL)
      {
         bool* va = (bool*)value;
         bool* vb = (bool*)v.value;
         *va = *vb;
      }
      else if(type == IA_TYPE_INT)
      {
         int* ia = (int*)value;
         int* ib = (int*)v.value;
         *ia = *ib;
      }
      else if(type == IA_TYPE_FLOAT)
      {
         float* fa = (float*)value;
         float* fb = (float*)v.value;
         *fa = *fb;
      }
      else if(type == IA_TYPE_STRING)
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



/***********************************************************
 *                      Constructor                        *
 ***********************************************************/
iaSymbolsTable::iaSymbolsTable()
{
   first = NULL;
   total = 0;
}

/***********************************************************
 *                       Destructor                        *
 ***********************************************************/
iaSymbolsTable::~iaSymbolsTable()
{
   while(first)
   {
      removeSymbol(first);
   }
}

/***********************************************************
 *                        addSymbol                        *
 ***********************************************************/
void iaSymbolsTable::addSymbol(string type, string name)
{
   iaVariable* iv = new iaVariable(type, name);

   if(first)
   {
      iv->next = first;
      iv->previous = first->previous;
      first->previous = iv;
      iv->previous->next = iv;
   }
   else
   {
      iv->next = iv;
      iv->previous = iv;
   }
   first = iv;
   total++;
}

/***********************************************************
 *                      removeSymbol                       *
 ***********************************************************/
void iaSymbolsTable::removeSymbol(string name)
{
   iaVariable* iv = getSymbol(name);
   if(iv)
   {
      removeSymbol(iv);
   }
   else
   {
      cerr << "IA Symbol's Table Warning: Atempt to remove unkonow symbol:" <<
              name << endl;
   }
}

/***********************************************************
 *                      removeSymbol                       *
 ***********************************************************/
void iaSymbolsTable::removeSymbol(iaVariable* symbol)
{
   if(symbol)
   {
      if(symbol == first)
      {
         first = first->next;
      }
      symbol->next->previous = symbol->previous;
      symbol->previous->next = symbol->next;
      delete(symbol);
      total--;
      if(total == 0)
      {
         first = NULL;
      }
   }
   else
   {
      cerr << "IA Symbol's Table Warning: Atempt to remove NULL symbol" << endl;
   }
}

/***********************************************************
 *                       getSymbol                         *
 ***********************************************************/
iaVariable* iaSymbolsTable::getSymbol(string name)
{
   iaVariable* iv = first;
   int i;
   for(i=0; i<total; i++)
   {
      if(iv->name == name)
      {
         return(iv);
      }
      iv = iv->next;
   }
   return(NULL);
}

