#ifndef _mapobjeto_h
#define _mapobjeto_h

/*   DCC Nightmare is Public Domain - Do whatever you want with this code.
 */

#include "../gui/objeto.h"
#include "../gui/lista.h"

/****************************************************
 *               Map's Objects                      *
 ****************************************************/
class mapObjeto: public Tobjeto
{
   public:
     int deltaVariacao;   /* For how many distance change static LOD*/
     char* nome;          /* Name of the object  */
     char* nomeArq;       /* File Name of the object */

     /* Desenha o objeto, estando em x,z a uma distancia do
      * observador (o que vai definir o LOD).
      * x,z       -> posicao do objeto
      * distancia -> distancia do objeto ao observador*/
     void Desenhar(float x, float z,GLfloat distancia, float orientacao);

     void* modelo3d;     // Modelo com nível máximo de detalhe
     void* modeloMedio;  // Modelo com nível médio de detalhe
     void* modeloMinimo; // Modelo com nível mínimo de detalhe
};  // OBJETO DO MAPA

class LmapObjeto: public Tlista
{
    public:
       /* Construtor */
       LmapObjeto():Tlista(){};
       /* Destruidor */
       ~LmapObjeto();

       /* Insere um objeto de mapa
        * arquivo -> nome do arquivo de definição do objeto*/
       mapObjeto* InserirMapObjeto(const char* arquivo, const char* nome);

       mapObjeto* EndMapObjeto(const char* nome);

       /* Retira um Objeto de Mapa da Lista
        * obj         -> ponteiro para o personagem que se deseja retirar
        * tiraMemoria -> != 0 se eh para retirar realmente o objeto 
        *                da lista, 0 senao (para o caso do destruidor)*/
       void RetirarMapObjeto(mapObjeto* obj, int tiraMemoria);
}; // LISTA DE OBJETOS DO MAPA

#endif
