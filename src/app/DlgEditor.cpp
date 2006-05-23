/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "../gui/farso.h"
#include "../engine/conversa.h"
#include "../engine/culling.h"

cores Cores;
int sair;

/* Menus Padroes */
menu* acoes;                 // Menu de acoes possiveis
menu* atributos;             // Menu de atributos possiveis
menu* operComp;              // Menu de operacoes de comparacao
menu* operMod;               // Menu de operacoes de Modificacao

/* Objetos da Janela Principal */
janela* principal;
  botao* botaoSalvarArquivo; 
  botao* botaoAbrirArquivo;
  botao* botaoSair;
  barraTexto* barraAbrir;

/* Objetos da Janela de Edicao de Dialogos */
janela* dialogos;
  cxSel* cxSelNPC;
  cxSel* cxSelPC[5];
  botao* botaoAttSeNPC;
  botao* botaoAttSePC[5];
  botao* botaoOperSeNPC;
  botao* botaoOperSePC[5];
  barraTexto* barraValSeNPC;
  barraTexto* barraValSePC[5];
  barraTexto* barraTextoSeNPC;
  barraTexto* barraTextoSePC[5];
  barraTexto* barraTextoSenaoNPC;
  barraTexto* barraTextoSenaoPC[5];
  botao* botaoAcaoSeNPC;
  botao* botaoAcaoSePC[5];
  botao* botaoAcaoAttSeNPC;
  botao* botaoAcaoAttSePC[5];
  botao* botaoAcaoOperSeNPC;
  botao* botaoAcaoOperSePC[5];
  barraTexto* barraAcaoValSeNPC;
  barraTexto* barraAcaoValSePC[5];
  botao* botaoAcaoSenaoNPC;
  botao* botaoAcaoSenaoPC[5];
  botao* botaoAcaoAttSenaoNPC;
  botao* botaoAcaoAttSenaoPC[5];
  botao* botaoAcaoOperSenaoNPC;
  botao* botaoAcaoOperSenaoPC[5];
  barraTexto* barraAcaoValSenaoNPC;
  barraTexto* barraAcaoValSenaoPC[5];
  botao* botaoAnterior;
  botao* botaoPrimeiro;
  botao* botaoUltimo;
  botao* botaoProximo;
  barraTexto* barraVaiPara;
  botao* botaoVaiPara;
  quadroTexto* quadroID;

int ProcedimentoSair(void* jan, void* ljan, SDL_Surface* screen)
{
   sair = SAIR;
   return(1);
}

void ProcedimentoMenus(void* jan, void* bot,menuItem* item,
                                                        SDL_Surface* screen)
{
   if(item)
   {
      botao* b=(botao*)bot;
      b->texto = item->texto;
      janela* j=(janela*)jan;
      j->Desenhar(0,0);
      j->AtualizaCara();
   }
}


void criaMenus()
{
   acoes     =  new menu();
   acoes->InserirMenuItem("ModifNPC",1);
   acoes->InserirMenuItem("ModifPC",1);
   acoes->InserirMenuItem("-",0);
   acoes->InserirMenuItem("Fechar",1);
   acoes->InserirMenuItem("VaPara",1);
   acoes->InserirMenuItem("-",0);
   acoes->InserirMenuItem("Brigar",1);
   acoes->procSelecionado = &ProcedimentoMenus;
   atributos =  new menu();
   atributos->InserirMenuItem("Intelig",1);
   atributos->InserirMenuItem("Carisma",1);
   atributos->InserirMenuItem("-",0);
   atributos->InserirMenuItem("Forca",1);
   atributos->InserirMenuItem("Agilidade",1);
   atributos->procSelecionado = &ProcedimentoMenus;
   operComp  =  new menu();
   operComp->InserirMenuItem("==",1);
   operComp->InserirMenuItem("!=",1);
   operComp->InserirMenuItem("-",0);
   operComp->InserirMenuItem(">",1);
   operComp->InserirMenuItem("<",1);
   operComp->InserirMenuItem("-",0);
   operComp->InserirMenuItem(">=",1);
   operComp->InserirMenuItem("<=",1);
   operComp->procSelecionado = &ProcedimentoMenus;
   operMod   =  new menu();
   operMod->InserirMenuItem("+",1);
   operMod->InserirMenuItem("-",1);
   operMod->InserirMenuItem("-",0);
   operMod->InserirMenuItem("0",1);
   operMod->procSelecionado = &ProcedimentoMenus;
}

void inserirObjsNPC()
{
   cxSelNPC = dialogos->objetos->InserirCxSel(35,24,0);
   dialogos->objetos->InserirQuadroTexto(47,23,77,36,0,"Se");
   botaoAttSeNPC = dialogos->objetos->InserirBotao(67,21,137,37,Cores.corBot.R,
                             Cores.corBot.G,
                             Cores.corBot.B,"Atributo",0,NULL);
   botaoAttSeNPC->men = atributos;
   botaoOperSeNPC = dialogos->objetos->InserirBotao(139,21,159,37,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,">=",0,NULL);
   botaoOperSeNPC->men = operComp;
   barraValSeNPC = dialogos->objetos->InserirBarraTexto(160,22,180,36,"10",
                                      0,NULL);
   barraTextoSeNPC = dialogos->objetos->InserirBarraTexto(181,22,280,36,
                                        "Texto",0,NULL);
   dialogos->objetos->InserirQuadroTexto(281,22,320,36,0,"Senao");
   barraTextoSenaoNPC = dialogos->objetos->InserirBarraTexto(320,22,420,36,
                                           "Texto",0,NULL);
   botaoAcaoSeNPC = dialogos->objetos->InserirBotao(55,38,125,54,
                              Cores.corBot.R,Cores.corBot.G,
                              Cores.corBot.B,"Acao",0,NULL);
   botaoAcaoSeNPC->men = acoes;
   botaoAcaoAttSeNPC = dialogos->objetos->InserirBotao(127,38,197,54,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,"Atributo",0,NULL);
   botaoAcaoAttSeNPC->men = atributos;
   botaoAcaoOperSeNPC = dialogos->objetos->InserirBotao(199,38,215,54,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,"+",0,NULL);
   botaoAcaoOperSeNPC->men = operMod;
   barraAcaoValSeNPC = dialogos->objetos->InserirBarraTexto(217,39,237,53,
                             "10",0,NULL);
   botaoAcaoSenaoNPC = dialogos->objetos->InserirBotao(255,38,325,54,
                              Cores.corBot.R,Cores.corBot.G,
                              Cores.corBot.B,"Acao",0,NULL);
   botaoAcaoSenaoNPC->men = acoes;
   botaoAcaoAttSenaoNPC = dialogos->objetos->InserirBotao(327,38,397,54,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,"Atributo",0,NULL);
   botaoAcaoAttSenaoNPC->men = atributos;
   botaoAcaoOperSenaoNPC = dialogos->objetos->InserirBotao(399,38,415,54,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,"+",0,NULL);
   botaoAcaoOperSenaoNPC->men = operMod;
   barraAcaoValSenaoNPC = dialogos->objetos->InserirBarraTexto(417,39,437,53,
                             "10",0,NULL);
}

void inserirObjsPC()
{
  int i;
  int y = 71;
  dialogos->objetos->InserirQuadroTexto(7,63,443,272,1,"PC:");
  for(i=0;i<5;i++)
  {
   cxSelPC[5] = dialogos->objetos->InserirCxSel(35,y+3,0);
   dialogos->objetos->InserirQuadroTexto(47,y+2,77,y+15,0,"Se");
   botaoAttSePC[i] = dialogos->objetos->InserirBotao(67,y,137,y+16,Cores.corBot.R,
                             Cores.corBot.G,
                             Cores.corBot.B,"Atributo",0,NULL);
   botaoAttSePC[i]->men = atributos;
   botaoOperSePC[i] = dialogos->objetos->InserirBotao(139,y,159,y+16,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,">=",0,NULL);
   botaoOperSePC[i]->men = operComp;
   barraValSePC[i] = dialogos->objetos->InserirBarraTexto(160,y+1,180,y+15,"10",
                                      0,NULL);
   barraTextoSePC[i] = dialogos->objetos->InserirBarraTexto(181,y+1,280,y+15,
                                        "Texto",0,NULL);
   dialogos->objetos->InserirQuadroTexto(281,y+1,320,y+15,0,"Senao");
   barraTextoSenaoPC[i] = dialogos->objetos->InserirBarraTexto(320,y+1,420,y+15,
                                           "Texto",0,NULL);
   botaoAcaoSePC[i] = dialogos->objetos->InserirBotao(55,y+17,125,y+33,
                              Cores.corBot.R,Cores.corBot.G,
                              Cores.corBot.B,"Acao",0,NULL);
   botaoAcaoSePC[i]->men = acoes;
   botaoAcaoAttSePC[i] = dialogos->objetos->InserirBotao(127,y+17,197,y+33,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,"Atributo",0,NULL);
   botaoAcaoAttSePC[i]->men = atributos;
   botaoAcaoOperSePC[i] = dialogos->objetos->InserirBotao(199,y+17,215,y+33,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,"+",0,NULL);
   botaoAcaoOperSePC[i]->men = operMod;
   barraAcaoValSePC[i] = dialogos->objetos->InserirBarraTexto(217,y+18,237,y+32,
                             "10",0,NULL);
   botaoAcaoSenaoPC[i] = dialogos->objetos->InserirBotao(255,y+17,325,y+33,
                              Cores.corBot.R,Cores.corBot.G,
                              Cores.corBot.B,"Acao",0,NULL);
   botaoAcaoSenaoPC[i]->men = acoes;
   botaoAcaoAttSenaoPC[i] = dialogos->objetos->InserirBotao(327,y+17,397,y+33,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,"Atributo",0,NULL);
   botaoAcaoAttSenaoPC[i]->men = atributos;
   botaoAcaoOperSenaoPC[i] = dialogos->objetos->InserirBotao(399,y+17,415,y+33,
                             Cores.corBot.R,Cores.corBot.G,
                             Cores.corBot.B,"+",0,NULL);
   botaoAcaoOperSenaoPC[i]->men = operMod;
   barraAcaoValSenaoPC[i] = dialogos->objetos->InserirBarraTexto(417,y+18,437,y+32,
                             "10",0,NULL);
   y+=40;

  }
}


void inserirObjsDialogo()
{
   dialogos->objetos->InserirQuadroTexto(7,353,443,425,1,"Dialogo:");
   botaoAnterior = dialogos->objetos->InserirBotao(53,405,138,421,
                                   Cores.corBot.R,Cores.corBot.G,Cores.corBot.G,
                                   "<- Anterior",0,NULL);
   botaoPrimeiro = dialogos->objetos->InserirBotao(139,405,219,421,
                                   Cores.corBot.R,Cores.corBot.G,Cores.corBot.G,
                                   "Primeiro",0,NULL);
   botaoUltimo = dialogos->objetos->InserirBotao(220,405,297,421,
                                   Cores.corBot.R,Cores.corBot.G,Cores.corBot.G,
                                   "Ultimo",0,NULL);
   botaoProximo = dialogos->objetos->InserirBotao(298,405,385,421,
                                   Cores.corBot.R,Cores.corBot.G,Cores.corBot.G,
                                   "Proximo ->",0,NULL);
   barraVaiPara=dialogos->objetos->InserirBarraTexto(233,380,263,394,"",0,NULL);
   botaoVaiPara = dialogos->objetos->InserirBotao(160,379,231,395,
                                   Cores.corBot.R,Cores.corBot.G,Cores.corBot.G,
                                   "Vai Para",0,NULL);
   dialogos->objetos->InserirQuadroTexto(185,360,210,374,0,"id:");
   quadroID = dialogos->objetos->InserirQuadroTexto(211,360,241,374,1,"100");
}

int main(int argc, char *argv[])
{
    //cores_Iniciar();
   SDL_Surface *screen;
   Farso_Iniciar(&screen,"Editor de Diálogos");
   glViewport(0,0,800,600);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrtho(0.0f, 800, 600, 0.0f, -1.0f, 1.0f);
	
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   Cores.Iniciar();

   interface* interf = new interface(NULL);
   
   principal=interf->ljan->InserirJanela(0,0,255,63,"Principal",1,1,NULL,NULL);
   barraAbrir = principal->objetos->InserirBarraTexto(10,23,173,36,"Arquivo",
                                                      0,NULL);
   botaoAbrirArquivo = principal->objetos->InserirBotao(15,40,65,56,
                                  Cores.corBot.R,Cores.corBot.G,
                                  Cores.corBot.B,"Abrir",1,NULL);
   botaoSalvarArquivo = principal->objetos->InserirBotao(66,40,116,56,
                              Cores.corBot.R,Cores.corBot.G,
                              Cores.corBot.B,"Salvar",1,NULL);
   botaoSair = principal->objetos->InserirBotao(117,40,167,56,Cores.corBot.R,
                              Cores.corBot.G,
                              Cores.corBot.B,"Sair",1,&ProcedimentoSair);
   principal->fechavel = 0;
   principal->Abrir(interf->ljan);

   dialogos=interf->ljan->InserirJanela(0,70,511,581,"Dialogo",1,1,NULL,NULL);
   dialogos->objetos->InserirQuadroTexto(7,15,443,62,1,"NPC:");
   
   criaMenus();
   inserirObjsNPC();    
   inserirObjsPC();
   inserirObjsDialogo();
   dialogos->objetos->InserirFigura(10,285,0,0,"../data/pics/dcc.bmp");

   dialogos->fechavel = 0;
   dialogos->Abrir(interf->ljan);

   sair = -2;

   GLfloat matriz[6][4];
   GLdouble proj[16];
   GLdouble modl[16];
   AtualizaFrustum(matriz, proj, modl);
   GLint viewPort[4];
   glGetIntegerv(GL_VIEWPORT, viewPort);
   Uint8 *keys; 
   Uint8 Mbotao;
   int x,y;

   Uint32 tempo;
   Uint32 tempoAnterior = 0;

   while (!(sair == SAIR))
   {
      tempo = SDL_GetTicks();
      if(tempo - tempoAnterior >= 20)
      {
         glClearColor(0,0,0,1);
         tempoAnterior = tempo;
         SDL_PumpEvents();
         keys = SDL_GetKeyState(NULL);
         Mbotao = SDL_GetMouseState(&x,&y);
         interf->ManipulaEventos(x,y,Mbotao,keys);
         if(keys[SDLK_ESCAPE])
           sair = SAIR;
         interf->Desenhar(proj,modl,viewPort);
         glFlush();
         SDL_GL_SwapBuffers();
         //printf("%d\n",sair);
      }
   }
    
   delete(interf);
   delete(acoes);
   delete(atributos);
   delete(operComp);
   delete(operMod);
   
   return(0);


}

