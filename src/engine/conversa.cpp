#include "conversa.h"

conversa::conversa()
{
   primeiro = new(dialogo);
   primeiro->proximo = primeiro;
   primeiro->anterior = primeiro;
   primeiro->id = 0;
   total = 0;
}

conversa::~conversa()
{
   dialogo* dlg;
   while(total>0)
   {
       dlg = primeiro->proximo;
       retirarDialogo(dlg->id);
   }
   delete(primeiro);
   total = -1;
}

int conversa::carregarArquivo(char* nome)
{
  dialogo* dlg;
  int aux;

  FILE* arq= fopen(nome,"r");
  if(!arq) return(0);
  fread(&total,sizeof(int),1,arq);
  for(aux=0;aux<total;aux++)
  {
     fread(dlg,sizeof(dialogo),1,arq);
     dlg->proximo = primeiro->proximo;
     dlg->anterior = primeiro;
     dlg->proximo->anterior = dlg;
     primeiro->proximo = dlg;
  }
  fclose(arq);
  return(1);
}


int conversa::salvarArquivo(char* nome)
{
  int aux;
  dialogo* dlg= primeiro->proximo;

  FILE *arq = fopen(nome,"w");
  if(!arq) return(0);
  fwrite(&total,sizeof(int),1,arq); //Escreve o Total de Dialogos
  for(aux=0;aux<total;aux++)
  {
     fwrite(dlg,sizeof(dialogo),1,arq);
     dlg = dlg->proximo;
  }
  fclose(arq);
  return(1);
}

dialogo* conversa::inserirDialogo()
{
   dialogo* dlg = new (dialogo);
   dlg->proximo = primeiro->proximo;
   dlg->anterior = primeiro;
   dlg->proximo->anterior = dlg;
   dlg->id = dlg->proximo->id+1;
   primeiro->proximo = dlg;
   total++;
   return(dlg);
}

void conversa::retirarDialogo(int num)
{
   dialogo *dlg = primeiro->proximo;
   while((dlg != primeiro) && (dlg->id != num))
   {
       dlg = dlg->proximo;
   }
   if(dlg == primeiro)
      printf("Nao encontrei para retirar dialogo %d\n",num);
   else
   {
      dlg->proximo->anterior = dlg->anterior;
      dlg->anterior->proximo = dlg->proximo;
      total--;
      delete(dlg);
   }
}

void conversa::abrirDialogo(int numDialogo)
{
}

