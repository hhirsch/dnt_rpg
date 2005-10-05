#ifndef _texture_h
#define _texture_h

#include <GL/gl.h>
#include <SDL/SDL_image.h>

typedef struct _texture
{
      char* nome;                   /* Nome da Textura */
      char* arqNome;                /* Nome do Arquivo da Textura */
      GLuint indice;                /* Indice da Textura internamente */
      GLuint w,h;                   /* Dimensoes da Textura */
      GLuint R,G,B;                 /* Cores da Textura para o MINIMAPA */
      int numReferencias;           /* Total de referencias 'a textura. 
                                     * Se 0 retira da lista */
      struct _texture* proximo;     /* Proxima Textura na Lista */
      struct _texture* anterior;    /* Textura Anterior na lista */
}texture;

class texturasLista
{

   public:
      texturasLista();
      ~texturasLista();
      int numTexturas;
      void inserirTextura(char* arqNome, char* nome, 
                          GLuint R, GLuint G, GLuint B);
      void retirarTextura(char* nome);
      void retirarTextura(GLuint ID);
      int idTextura(char* nome, GLuint* R, GLuint* G, GLuint* B);
      char* nomeTextura(GLuint ID);

   private:
      texture* primeiro;

};

#endif

