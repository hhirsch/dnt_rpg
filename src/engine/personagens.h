/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _personagens_h
#define _personagens_h

#include "../gui/desenho.h"
#include "../gui/figura.h"
#include "../gui/objeto.h"
#include "../gui/lista.h"
//#include "habilidades.h"
#include "../etc/glm.h"

#define POSRETX 8   // posicao x do retrato na janela
#define POSRETY 20  // posicao y do retrato na janela

class personagem: public Tobjeto
{
   public:
      int agilidade;            // agility
      int bovinice;             // taylor's bovinice
      int brutalidade;          // brutality
      int esperteza;            // wisdow
      int forca;                // strong
      int gambiarrice;          // gambiarry
      int inteligencia;         // inteligency
      int odio;                 // hate
      Tlista *retrato;          // portraits
      Tlista *objetos;          // objetos do personagem
      Tobjeto *armaAtual;       // armaAtual do personagem
      int x,y;                  // coordenada do mapa que o bichinho esta
      char *nome;               // nome do personagem
      GLMmodel* modelo3d;       // modelo estatico do personagem
      float orientacao;         // orientacao do personagem (onde esta sua cara)
      float posicaoFrente;      // posicao do personagem pra frente
      float posicaoLadoX;       // posicao do personagem pro lado
      float posicaoLadoZ;       //
   // Lhabilidade* habilidades; // habilidades personagem (ex:kamasutramaster)
};                 // O PERSONAGEM

class Lpersonagem: public Tlista
{
   public:
      personagem* personagemAtivo;  // Personagem ativo pelo jogador atualmente

      /* Construtor da Lista */
      Lpersonagem():Tlista(){personagemAtivo=NULL;};
      /* Destruidor da Lista */
      ~Lpersonagem();

      /* Insere um personagem na Lista
       * forca, agilidade, etc,
       * inteligencia, esperteza -> atributos do personagem  
       * retrato                 -> nome do arquivo de retrato
       * x,y                     -> coordenada do personagem no Mapa
       * nome                    -> Nome do Personagem
       * arqmodelo               -> nome do arquivo do modelo3d do personagem*/
      personagem* InserirPersonagem(int forca,int agilidade,int inteligencia,
                          int esperteza,char* retrato,int x, int y,
                          char* nome, char* arqmodelo, char* dirTexturas);

      /* Retira um Personagem da Lista
       * persona -> ponteiro para o personagem que se deseja retirar
       * tiraMemoria -> != 0 se eh para retirar realmente o personagem 
       *                da lista, 0 senao (para o caso do destruidor)*/
      void Lpersonagem::RetirarPersonagem(personagem* persona, int tiraMemoria);

};               // LISTA DE PERSONAGENS

#endif

