/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _objeto_h
#define _objeto_h

#define JANELA        0
#define BOTAO         1
#define BARRATEXTO    2
#define CXSEL         3
#define FIGURA        4
#define SELTEXTO      5
#define QUADROTEXTO   6
#define MENUITEM      7
#define MAPOBJETO     99
#define PERSONAGEM    100 


class Tobjeto{
   public:
      int tipo;          //controle dos tipos de objeto 
      Tobjeto *proximo;  //proximo objeto na estrutura
      Tobjeto *anterior; //objeto anterior na estrutura
};



#endif

