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
//#include "../etc/glm.h"
#include <cal3d/cal3d.h> //for now, utilising cal3d
#include <string>
using namespace std;
#include "../map/map.h"

#define POSRETX 8   // posicao x do retrato na janela
#define POSRETY 20  // posicao y do retrato na janela

#define STATE_IDLE  0 
#define STATE_WALK  1


class personagem: public Tobjeto
{
   public:

      int m_state;

      int agilidade;            // agility
      int bovinice;             // taylor's bovinice
      int brutalidade;          // brutality
      int esperteza;            // wisdow
      int forca;                // strong
      int gambiarrice;          // gambiarry
      int inteligencia;         // inteligency
      int odio;                 // hate

      bool amigavel;            //friendly 
 
      Tlista *retrato;          // portraits
      Tlista *objetos;          // objetos do personagem
      Tobjeto *armaAtual;       // armaAtual do personagem

      string nome;              // nome do personagem
      string retratoConversa;   // arquivo de retrato de conversa
      float orientacao;         // orientacao do personagem (onde esta sua cara)
      float posicaoFrente;      // posicao do personagem pra frente
      float posicaoLadoX;       // posicao do personagem pro lado
      float posicaoLadoY;       // posicao vertical do personagem
      float posicaoLadoZ;       // posicao do personagem pro lado

      GLfloat min[3];           // pontos de minimo do bounding box estatico
      GLfloat max[3];           // pontos de maximo do bounding box estatico

   // Lhabilidade* habilidades; // habilidades personagem (ex:kamasutramaster)
      GLuint portrait;

      Square* ocupaQuad;   // quadrado do mapa que ele ocupa com x1,z1

      int ID;

      personagem();
      ~personagem();
      bool LoadModel(const string& strFilename);
      GLuint loadTexture(const string& strFilename);
      void SetState(int state);
      void Render();
      void CalculateBoundingBox();
      void RenderBoundingBox();





// CAL3D related member variables

   CalCoreModel* m_calCoreModel;
  CalModel* m_calModel;

 
protected:
  
  int m_animationId[16];
  int m_animationCount;
  int m_meshId[32];
  int m_meshCount;
  GLuint m_textureId[32];
  int m_textureCount;
  float m_motionBlend[3];
  float m_renderScale;
  float m_lodLevel;
  std::string m_path;


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
      personagem* InserirPersonagem(int forca,int agilidade,
                          int inteligencia,
                          int esperteza, char* retrato,
                          string nome, string arqmodelo);

      /* Retira um Personagem da Lista
       * persona -> ponteiro para o personagem que se deseja retirar
       * tiraMemoria -> != 0 se eh para retirar realmente o personagem 
       *                da lista, 0 senao (para o caso do destruidor)*/
      void Lpersonagem::RetirarPersonagem(personagem* persona, int tiraMemoria);

};               // LISTA DE PERSONAGENS

#endif

