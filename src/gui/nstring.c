/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "nstring.h"

/* Deleta n caracteres em st a partir de pos */
char* string_Deletar(char *st,int pos, int n)
{
   char* depois = &st[pos+n];
   if(pos>0) 
   {  /*GAMBIARRRAAAAA!!!!!!!!!!!!!!!!!!!!!****************************/
      char* antes = (char*) malloc(255*sizeof(char));
      int aux;
      for(aux=0;aux<pos;aux++)
         antes[aux] = st[aux];
      int aux2=pos+n;
      while(st[aux2]!='\0')
      {
         antes[aux] = st[aux2];
         aux++;aux2++;
      }
      antes[aux] = '\0';
      //free(st); 
      return(antes);
   }
   else
      return(depois);
}

char* string_Colocar(char* st,char c,int pos)
{  /*MAIS GAMBIARRA!!!!!!!***************************/
   char* result= (char*) malloc(sizeof(char)*255);
   int aux; 
   for (aux=0;aux<pos;aux++)
      result[aux] = st[aux];
   result[pos] = c;
   while(st[aux]!='\0')
   {
      result[aux+1] = st[aux];
      aux++;
   }
   result[aux+1] = '\0';
   return(result);
}


