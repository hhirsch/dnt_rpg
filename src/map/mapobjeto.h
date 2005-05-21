#ifndef _mapobjeto_h
#define _mapobjeto_h

#include "../gui/objeto.h"
#include "../gui/lista.h"

class mapObjeto: public Tobjeto
{
   public:
     int deltaVariacao;   /* De quantas em quantas unidades de distancia 
                           * se d� a troca de nivel de detalhe do modelo.*/
     char* nome;

     /* Desenha o objeto, estando em x,z a uma distancia do
      * observador (o que vai definir o LOD).
      * x,z       -> posicao do objeto
      * distancia -> distancia do objeto ao observador*/
     void Desenhar(int x, int z,int distancia);

     void* modelo3d;     // Modelo com n�vel m�ximo de detalhe
     void* modeloMedio;  // Modelo com n�vel m�dio de detalhe
     void* modeloMinimo; // Modelo com n�vel m�nimo de detalhe
};  // OBJETO DO MAPA

class LmapObjeto: public Tlista
{
    public:
       /* Construtor */
       LmapObjeto():Tlista(){};
       /* Destruidor */
       ~LmapObjeto();

       /* Insere um objeto de mapa
        * arquivo -> nome do arquivo de defini��o do objeto*/
       mapObjeto* InserirMapObjeto(char* arquivo, char* nome);

       mapObjeto* EndMapObjeto(char* nome);

       /* Retira um Objeto de Mapa da Lista
        * obj         -> ponteiro para o personagem que se deseja retirar
        * tiraMemoria -> != 0 se eh para retirar realmente o objeto 
        *                da lista, 0 senao (para o caso do destruidor)*/
       void RetirarMapObjeto(mapObjeto* obj, int tiraMemoria);
}; // LISTA DE OBJETOS DO MAPA

#endif
