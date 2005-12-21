/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


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

   dlg->NPC.Se = "";
   dlg->NPC.Atributo = -1;
   dlg->NPC.Senao = "";
   dlg->NPC.Operador = -1;
   dlg->NPC.SeAcao.id = -1;
   dlg->NPC.SenaoAcao.id = -1;

   int aux;
   for(aux = 0; aux< 5; aux++)
   {
      dlg->Opcoes[aux].Se = "";
      dlg->Opcoes[aux].Atributo = -1;
      dlg->Opcoes[aux].Senao = "";
      dlg->Opcoes[aux].Operador = -1;
      dlg->Opcoes[aux].SeAcao.id = -1;
      dlg->Opcoes[aux].SenaoAcao.id = -1;
   }

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
      printf("Not found on dialog %d\n",num);
   else
   {
      dlg->proximo->anterior = dlg->anterior;
      dlg->anterior->proximo = dlg->proximo;
      total--;
      delete(dlg);
   }
}

void conversa::abrirDialogo(int numDialogo, interface* gui,
                               personagem* pers, 
                               int (*procPres)(SDL_Surface *screen, int texto))
{
   dialogo* dlg = primeiro->proximo;
   while( (dlg != primeiro) && (dlg->id != numDialogo))
   {
      dlg = dlg->proximo;
   }
   if(dlg == primeiro)
   {
      return;
   }

   string NPC;
   string Opcoes[5];
   int aux;
//TODO verificar atributos do se senao
   NPC = dlg->NPC.Se;
   for(aux = 0; aux<5; aux++)
   {
      Opcoes[aux] = dlg->Opcoes[aux].Se; 
   }
 
   janela* jan = gui->ljan->InserirJanela(330,100,585,355,"Dialog",1,1,NULL,NULL);
   jan->objetos->InserirFigura(8,20,pers->retratoConversa.c_str());
   jan->objetos->InserirQuadroTexto(90,20,160,95,1,NPC.c_str());
   jan->objetos->InserirSelTexto(8,100,160,252,Opcoes[0],
                      Opcoes[1], Opcoes[2],
                      Opcoes[3], Opcoes[4], procPres);
   jan->ptrExterno = &jan;
   jan->Abrir(gui->ljan);
}

int conversa::ProcessaAcao(int numDialogo, int opcao,interface* gui,
                           personagem* PC, personagem* NPC)
{

   dialogo* dlg = primeiro->proximo;
   while( (dlg != primeiro) && (dlg->id != numDialogo))
   {
      dlg = dlg->proximo;
   }
   if(dlg == primeiro)
   {
      return(-1);
   }


   int action  = dlg->Opcoes[numDialogo].SeAcao.id;
//TODO verificar atributos se senao   
   switch(action)
   {
      case ACTION_GOTO:
         return(numDialogo);
      break;
      case ACTION_FIGHT:
         NPC->amigavel = false; //brigar
      break;
      case ACTION_CLOSE:
         jan->Fechar(gui->ljan);
      break;
      case ACTION_MODPC:
      break;
      case ACTION_MODNPC:
      break;
   }
   return(-1);
}

