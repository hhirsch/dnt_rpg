#include "classes.h"


classe::classe(string arq)
{
   FILE* file;
   int nAjustes = 0;
   int aux;
   char buffer[1024];
   ajuste* ajus;

   ajustes = new(ajuste);
   ajustes->proximo = NULL;

   if(!(file=fopen(arq.c_str(),"r")))
   {
       printf("Error while opening character class: %s\n",arq.c_str());
       return;
   }

   fgets(buffer,sizeof(buffer),file);
   descricao = buffer;

   fscanf(file,"%d",&nAjustes);

   for(aux = 0; aux < nAjustes; aux++)
   {
      ajus = new(ajuste);
      ajus->proximo = ajustes->proximo;
      ajustes->proximo = ajus;
      fscanf(file,"%d %d %d",&ajus->variacao,&ajus->onde,&ajust->custoMod);
   }

   fclose(file);

}

classe::~classe()
{
   ajuste* ajus;
   while(ajustes->proximo)
   {
      ajus = ajustes->proximo;
      ajustes->proximo = ajus->proximo;
      delete(ajus);
   }
   delete(ajustes);
}

