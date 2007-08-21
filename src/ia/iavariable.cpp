#include "iavariable.h"

/***********************************************************
 *                      Constructor                        *
 ***********************************************************/
iaVariable::iaVariable(string varType, string varName)
{
   type = varType;
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
      if((type == IA_TYPE_BOOL) && (v.type == IA_TYPE_BOOL))
      {
         bool* ba = (bool*) value;
         bool* bb = (bool*) v.value;
         return( (*ba) == (*bb) );
      }
      else if( (type == IA_TYPE_INT) && (v.type == IA_TYPE_INT))
      {
         int* ia = (int*) value;
         int* ib = (int*) v.value;
         return( (*ia) == (*ib) );
      }
      else if( (type == IA_TYPE_INT) && (v.type == IA_TYPE_FLOAT))
      {
         int* ia = (int*) value;
         float* ib = (float*) v.value;
         return( (*ia) == (*ib) );
      }
      else if( (type == IA_TYPE_FLOAT) && (v.type == IA_TYPE_INT))
      {
         float* fa = (float*) value;
         int* fb = (int*) v.value;
         return( (*fa) == (*fb) );
      }
      else if( (type == IA_TYPE_FLOAT) && (v.type == IA_TYPE_FLOAT))
      {
         float* fa = (float*) value;
         float* fb = (float*) v.value;
         return( (*fa) == (*fb) );
      }
      else if( (type == IA_TYPE_STRING) && (v.type == IA_TYPE_STRING))
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
 *                           >=                            *
 ***********************************************************/
bool iaVariable::operator>=(const iaVariable& v)
{
   return( (operator==(v)) || (operator>(v)) );
}


/***********************************************************
 *                           >                             *
 ***********************************************************/
bool iaVariable::operator>(const iaVariable& v)
{
   if(type != v.type)
   {
      return(false);
   }
   else
   {
      if( (type == IA_TYPE_BOOL) && (v.type == IA_TYPE_BOOL) )
      {
         bool* ba = (bool*) value;
         bool* bb = (bool*) v.value;
         return( (*ba) > (*bb) );
      }
      else if( (type == IA_TYPE_INT) && (v.type == IA_TYPE_INT))
      {
         int* ia = (int*) value;
         int* ib = (int*) v.value;
         return( (*ia) > (*ib) );
      }
      else if( (type == IA_TYPE_INT) && (v.type == IA_TYPE_FLOAT))
      {
         int* ia = (int*) value;
         float* ib = (float*) v.value;
         return( (*ia) > (*ib) );
      }
      else if( (type == IA_TYPE_FLOAT) && (v.type == IA_TYPE_INT))
      {
         float* fa = (float*) value;
         int* fb = (int*) v.value;
         return( (*fa) > (*fb) );
      }
      else if( (type == IA_TYPE_FLOAT) && (v.type == IA_TYPE_FLOAT))
      {
         float* fa = (float*) value;
         float* fb = (float*) v.value;
         return( (*fa) > (*fb) );
      }
      else if( (type == IA_TYPE_STRING) && (v.type == IA_TYPE_STRING))
      {
         string* sa = (string*) value;
         string* sb = (string*) v.value;
         return( (*sa) > (*sb) );
      }
      else
      {
         /* the pointer; not so usefull */
         return( (value) > (v.value) ); 
      }
   }
   return(false);
}

/***********************************************************
 *                           <=                            *
 ***********************************************************/
bool iaVariable::operator<=(const iaVariable& v)
{
   return( (operator==(v)) || (operator<(v)) );
}

/***********************************************************
 *                           <                             *
 ***********************************************************/
bool iaVariable::operator<(const iaVariable& v)
{
   return(!(operator>=(v)));
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
 *                  receiveOperation                       *
 ***********************************************************/
void iaVariable::receiveOperation(string operation, iaVariable* v1,
                                  iaVariable* v2)
{
   /* Arithimetic  */
   if( (operation == IA_OPERATOR_ADDITION) ||
       (operation == IA_OPERATOR_SUBTRACTION) ||
       (operation == IA_OPERATOR_MULTIPLICATION) ||
       (operation == IA_OPERATOR_DIVISION) ||
       (operation == IA_OPERATOR_MODULUS) )
   {
      if( (type == IA_TYPE_INT) && (v1->type == IA_TYPE_INT) &&
          (v2->type == IA_TYPE_INT) )
      {
         /* Integer operation */
         int val1 = *(int*)v1->value;
         int val2 = *(int*)v2->value;
         if(operation == IA_OPERATOR_ADDITION)
         {
            *(int*)value = val1 + val2;
         }
         else if(operation == IA_OPERATOR_SUBTRACTION)
         {
            *(int*)value = val1 - val2;
         }
         else if(operation == IA_OPERATOR_MULTIPLICATION)
         {
            *(int*)value = val1 * val2;
         }
         else if(operation == IA_OPERATOR_DIVISION)
         {
            *(int*)value = val1 / val2;
         }
         else if(operation == IA_OPERATOR_MODULUS)
         {
            *(int*)value = val1 % val2;
         }
      }
      else if( (type == IA_TYPE_FLOAT) && (v1->type == IA_TYPE_FLOAT) &&
               (v2->type == IA_TYPE_INT) )
      {
         /* FLOAT operation */
         float val1 = *(float*)v1->value;
         int val2 = *(int*)v2->value;
         if(operation == IA_OPERATOR_ADDITION)
         {
            *(float*)value = val1 + val2;
         }
         else if(operation == IA_OPERATOR_SUBTRACTION)
         {
            *(float*)value = val1 - val2;
         }
         else if(operation == IA_OPERATOR_MULTIPLICATION)
         {
            *(float*)value = val1 * val2;
         }
         else if(operation == IA_OPERATOR_DIVISION)
         {
            *(float*)value = val1 / val2;
         }
         else if(operation == IA_OPERATOR_MODULUS)
         {
            cerr << "Erro: Modulus Operation is not defined for " 
                 << IA_TYPE_FLOAT << endl;
         }
      }
      else if( (type == IA_TYPE_FLOAT) && (v1->type == IA_TYPE_INT) &&
               (v2->type == IA_TYPE_FLOAT) )
      {
         /* FLOAT operation */
         int val1 = *(int*)v1->value;
         float val2 = *(float*)v2->value;
         if(operation == IA_OPERATOR_ADDITION)
         {
            *(float*)value = val1 + val2;
         }
         else if(operation == IA_OPERATOR_SUBTRACTION)
         {
            *(float*)value = val1 - val2;
         }
         else if(operation == IA_OPERATOR_MULTIPLICATION)
         {
            *(float*)value = val1 * val2;
         }
         else if(operation == IA_OPERATOR_DIVISION)
         {
            *(float*)value = val1 / val2;
         }
         else if(operation == IA_OPERATOR_MODULUS)
         {
            cerr << "Erro: Modulus Operation is not defined for " 
                 << IA_TYPE_FLOAT << endl;
         }
      }
      else if( (type == IA_TYPE_FLOAT) && (v1->type == IA_TYPE_FLOAT) &&
               (v2->type == IA_TYPE_FLOAT) )
      {
         /* FLOAT operation */
         float val1 = *(float*)v1->value;
         float val2 = *(float*)v2->value;
         if(operation == IA_OPERATOR_ADDITION)
         {
            *(float*)value = val1 + val2;
         }
         else if(operation == IA_OPERATOR_SUBTRACTION)
         {
            *(float*)value = val1 - val2;
         }
         else if(operation == IA_OPERATOR_MULTIPLICATION)
         {
            *(float*)value = val1 * val2;
         }
         else if(operation == IA_OPERATOR_DIVISION)
         {
            *(float*)value = val1 / val2;
         }
         else if(operation == IA_OPERATOR_MODULUS)
         {
            cerr << "Erro: Modulus Operation is not defined for " 
                 << IA_TYPE_FLOAT << endl;
         }
      }
   }

   /* and && or || operations */
   else if( (operation == IA_OPERATOR_AND) ||
            (operation == IA_OPERATOR_OR) )
   {
      if( (type == IA_TYPE_BOOL) && (v1->type == IA_TYPE_BOOL) &&
          (v2->type == IA_TYPE_BOOL))
      {
         bool val1 = *(bool*)v1->value;
         bool val2 = *(bool*)v2->value;
         if(operation == IA_OPERATOR_AND)
         {
            *(bool*)value = val1 && val2;
         }
         else if(operation == IA_OPERATOR_OR)
         {
            *(bool*)value = val1 || val2;
         }
      }
      else
      {
         cerr << "Error: Operator " << operation << " requires "
              << IA_TYPE_BOOL << " types" << endl;
      }
   }

   /* Comparations */
   else if( (operation == IA_OPERATOR_EQUAL) ||
            (operation == IA_OPERATOR_NOT_EQUAL) ||
            (operation == IA_OPERATOR_LESSER) || 
            (operation == IA_OPERATOR_GREATER) ||
            (operation == IA_OPERATOR_GEQUAL) ||
            (operation == IA_OPERATOR_LEQUAL) )
   {
      if(type == IA_TYPE_BOOL)
      {
         if(operation == IA_OPERATOR_EQUAL)
         {
            *(bool*)value = (*v1) == (*v2);
         }
         else if(operation == IA_OPERATOR_NOT_EQUAL)
         {
            *(bool*)value = (*v1) != (*v2);
         }
         else if(operation == IA_OPERATOR_LESSER)
         {
            *(bool*)value = (*v1) < (*v2);
         }
         else if(operation == IA_OPERATOR_GREATER)
         {
            *(bool*)value = (*v1) > (*v2);
         }
         else if(operation == IA_OPERATOR_LEQUAL)
         {
            *(bool*)value = (*v1) <= (*v2);
         }
         else if(operation == IA_OPERATOR_GEQUAL)
         {
            *(bool*)value = (*v1) >= (*v2);
         }
      }
      else
      {
         cerr << "Error: Operator " << operation << " type is " << 
                 IA_TYPE_BOOL << " not " << type << endl;
      }
   }
   /* ! not operations */
   else if((operation == IA_OPERATOR_NOT))
   {
      if( (type == IA_TYPE_BOOL) && (v1->type == IA_TYPE_BOOL) )
      {
         *(bool*)value = !(*(bool*)v1->value);
      }
      else
      {
         cerr << "Error: Operator " << operation << " is only defined for " 
              << IA_TYPE_BOOL << " type" << endl;
      }
   }
}

/***********************************************************
 *                       toString                          *
 ***********************************************************/
string iaVariable::toString()
{
   char buffer[1024];
   if(type == IA_TYPE_BOOL)
   {
      sprintf(buffer,"%d",*(bool*)value);
   }
   else if(type == IA_TYPE_INT)
   {
      sprintf(buffer,"%d",*(int*)value);  
   }
   else if(type == IA_TYPE_FLOAT)
   {
      sprintf(buffer,"%f",*(float*)value);
   }
   else if(type == IA_TYPE_STRING)
   {
      return(*(string*)value);
   }
   else
   {
      sprintf(buffer,"%p",value);
   }
   return(buffer);
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
      cerr << "IA Symbol's Table Warning: Atempt to remove unkonow symbol: " <<
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



