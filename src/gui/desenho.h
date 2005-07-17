/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _desenho_h
#define _desenho_h

/* desenho.h, onde sao definidas as instrucoes de desenho na tela,
 * posteriormente utilizada para o desenvolvimento das primitivas
 * 'janeliferas'.*/
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

//Raiz de 3 dividida por 2
#define RAIZ3D2  0.8660254

/* Todos os desenhos sao feitos utilizando a cor ativa */

/* Define a cor ativa
 * Ri -> valor de Vermelho, de 0 a 255
 * Gi -> valor de Verde, de 0 a 255
 * Bi -> valor de Azul, de 0 a 255 */
void cor_Definir(Uint8 Ri, Uint8 Gi, Uint8 Bi);

/* Retorna em Ri, Gi e Bi a cor ativa
 * Ri -> valor ativo de vermelho
 * Gi -> valor ativo de verde
 * Bi -> valor ativo de azul */
void cor_Pegar(Uint8 *Ri, Uint8 *Gi, Uint8 *Bi);

/* Desenha um pixel com a cor ativa na superficie
 * screen -> superficie na qual o pixel sera desenhado
 * x,y    -> coordenada do pixel na superficie
 * salvar -> se !=0 atualiza a tela */
void pixel_Desenhar(SDL_Surface *screen, int x, int y, int salvar);

Uint32 pixel_Pegar(SDL_Surface *surface, int x, int y);

/* Abre um BMP na superficie
 * file   -> nome do arquivo do BMP
 * screen -> superficie, nao alocada, na qual sera aberto o BMP
 */
//void BMP_Mostrar(char *file, SDL_Surface **image);

/* Desenha uma linha reta na superficie
 * screen  ->  superficie na qual se deseja desenhar a linha
 * x1,y1   -> coordenada do ponto inicial da reta
 * x2,y2   -> coordenada do ponto final da reta 
 * salvar -> se !=0 atualiza a tela */
void linha_Desenhar(SDL_Surface *screen, int x1, int y1, int x2, int y2,
                    int salvar);

/* Desenha um retangulo na superficie
 * screen  ->  superficie na qual se deseja desenhar o retangulo
 * x1,y1   -> coordenada do ponto superior esquerdo do retangulo
 * x2,y2   -> coordenada do ponto inferior direito do retangulo 
 * salvar -> se !=0 atualiza a tela */
void retangulo_Desenhar(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                        int salvar);

/* Desenha e colore um retangulo na superficie
 * screen  ->  superficie na qual se deseja desenhar o retangulo
 * x1,y1   -> coordenada do ponto superior esquerdo do retangulo
 * x2,y2   -> coordenada do ponto inferior direito do retangulo 
 * salvar -> se !=0 atualiza a tela */
void retangulo_Colorir(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                       int salvar);

/* Desenha um retangulo oval(arredondado) na superficie
 * screen  ->  superficie na qual se deseja desenhar o retangulo
 * x1,y1   -> coordenada do ponto superior esquerdo do retangulo
 * x2,y2   -> coordenada do ponto inferior direito do retangulo
 * Ri,Gi,Bi-> cor a ser utilizada na parte inferior do retangulo 
 * salvar -> se !=0 atualiza a tela */
void retangulo_Oval(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                    int Ri, int Gi, int Bi, int salvar);

/* Desenha um retangulo oval(arredondado) na superficie
 * screen  ->  superficie na qual se deseja desenhar o retangulo
 * x1,y1   -> coordenada do ponto superior esquerdo do retangulo
 * x2,y2   -> coordenada do ponto inferior direito do retangulo
 * Ri,Gi,Bi-> cor a ser utilizada na parte inferior do retangulo 
 * salvar -> se !=0 atualiza a tela */
void retangulo_2Cores(SDL_Surface *screen, int x1, int y1, int x2, int y2, 
                      int Ri,int Gi, int Bi, int salvar);

/* Desenha um circulo na superficie
 * screen -> superficie na qual sera desenhado o circulo
 * xC,yC  -> coordenada do centro do circulo
 * r      -> raio do circulo
 * salvar -> se !=0 atualiza a tela */
void circulo_Desenhar(SDL_Surface *screen,int xC, int yC, int r, int salvar);

/* Desenha um hexagono na superficie
 * screen  -> superficie na qual sera desenhado
 * x,y     -> posicao inicial esquerda do hexagono
 * lado    -> tamanho do lado do hexagono
 * salvar  -> se !=0 atualiza */
void hexagono_Desenhar(SDL_Surface *screen, int x,int y, int lado, int salvar);

/* Copia a superficie para a tela como uma textura  */
void AtualizaTela2D(GLuint texturaID, GLdouble proj[16],GLdouble modl[16], 
                    GLint viewPort[4], int xa, int ya,int xb, int yb,
                    double profundidade);


#endif

