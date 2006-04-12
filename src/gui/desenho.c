/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "desenho.h"
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>


int R; int G; int B; /* Cores RGB atualmente setadas */

/* Define a cor a ser utilizada */
void cor_Definir(Uint8 Ri, Uint8 Gi, Uint8 Bi)
{
   R = Ri;
   G = Gi;
   B = Bi;
}


/* Pega a cor atualmente utilizada */
void cor_Pegar(Uint8 *Ri, Uint8 *Gi, Uint8 *Bi)
{
   *Ri = R;
   *Gi = G;
   *Bi = B;
}


/* Desenha um pixel em screen com coloracao em RGB */
void pixel_Desenhar(SDL_Surface *screen, int x, int y, int salvar)
{
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);

    if((x>screen->w-1) || (y>screen->h-1) || (x<0) || (y<0))
       return;

    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
    switch (screen->format->BytesPerPixel) {
        case 1: { /* Assumindo 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* Provavelmente 15-bpp ou 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* mode 24-bpp lento,normalmente não usado */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *(bufp+screen->format->Rshift/8) = R;
            *(bufp+screen->format->Gshift/8) = G;
            *(bufp+screen->format->Bshift/8) = B;
        }
        break;

        case 4: { /* Provavelmente 32-bpp */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
}


/* Arredondamento para o calculo da linha */
inline int round(const float a)
{
   return (int)(a+0.5);
}

/* Desenha a linha de inicio (x1,y1) e fim (x2,y2) na tela */
void linha_Desenhar(SDL_Surface *screen, int x1, int y1, int x2, int y2, int salvar)
{
   int dx = x2 - x1;
   int dy = y2 - y1;
   int passos, k;
   float xInc, yInc, x = x1, y = y1;
   if(fabs(dx) > fabs(dy))
      passos = (int) fabs(dx);
   else
      passos = (int) fabs(dy);
   xInc = (float) (dx) / (float) (passos);
   yInc = (float) (dy) / (float) (passos);
   pixel_Desenhar(screen, round(x), round(y), 0);
   for(k=0;k<passos;k++)
   {
      x+=xInc;
      y+=yInc;
      pixel_Desenhar(screen, round(x), round(y), 0);
   }
}


/* Desenha o retangulo (x1,y1,x2,y2) na tela (screen) */
void retangulo_Desenhar(SDL_Surface *screen, int x1, int y1, int x2, int y2, int salvar)
{
    linha_Desenhar(screen,x1,y1,x2,y1,0);
    linha_Desenhar(screen,x1,y2,x2,y2,0);
    linha_Desenhar(screen,x1,y1,x1,y2,0);
    linha_Desenhar(screen,x2,y1,x2,y2,0);
}


void retangulo_2Cores(SDL_Surface *screen, int x1, int y1, int x2, int y2, int Ri,int Gi, int Bi, int salvar)
{
    Uint8 Ra,Ga,Ba;
    cor_Pegar(&Ra,&Ga,&Ba);
    linha_Desenhar(screen,x1,y1,x2,y1,0);
    linha_Desenhar(screen,x1,y1,x1,y2,0);
    cor_Definir(Ri,Gi,Bi);
    linha_Desenhar(screen,x1,y2,x2,y2,0);
    linha_Desenhar(screen,x2,y1,x2,y2,0);
    cor_Definir(Ra,Ga,Ba);
}



/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 pixel_Pegar(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

/* Desenha um Retangulo Colorido */
void retangulo_Colorir(SDL_Surface *screen, int x1, int y1, int x2, int y2, int salvar)
{
   SDL_Rect ret;
   ret.x = x1;
   ret.y = y1;
   ret.w = (x2 - x1)+1;
   ret.h = (y2 - y1)+1;
   Uint32 cor = SDL_MapRGB(screen->format,R,G,B);
   if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
      SDL_FillRect(screen,&ret,cor);
   if ( SDL_MUSTLOCK(screen) ) 
   {
        SDL_UnlockSurface(screen);
   }
}


/* Desenha um Retangulo Oval */
void retangulo_Oval(SDL_Surface *screen, int x1, int y1, int x2, int y2, int Ri, int Gi, int Bi, int salvar)
{
   Uint8 Ra,Ga,Ba;
   cor_Pegar(&Ra,&Ga,&Ba);
   if (x2<x1)
   { 
      int aux = x1;
      x1 = x2;
      x2 = aux;
   }
   if (y2<y1) 
   {
      int aux = y1;
      y1 = y2;
      y2 = aux;
   }
   linha_Desenhar(screen,x1,y1+2,x1+2,y1,0);
   linha_Desenhar(screen,x1+2,y1,x2-2,y1,0);
   linha_Desenhar(screen,x1,y1+2,x1,y2-2,0);
   cor_Definir(Ri,Gi,Bi);
   linha_Desenhar(screen,x1,y2-2,x1+2,y2,0);
   linha_Desenhar(screen,x1+2,y2,x2-2,y2,0);
   linha_Desenhar(screen,x2-2,y2,x2,y2-2,0);
   linha_Desenhar(screen,x2,y2-2,x2,y1+2,0);
   linha_Desenhar(screen,x2-2,y1,x2,y1+2,0);
   cor_Definir(Ra,Ga,Ba);
}


/* Desenha Circulo de raio r e centro (Xc,Yc) */
void circulo_Desenhar(SDL_Surface *screen,int xC, int yC, int r, int salvar)
{
   int x=0, y=r,u=1,v=2*r-1,E=0;
   while(x < y)
   {
     pixel_Desenhar(screen,xC+x, yC+y, 0);
     pixel_Desenhar(screen,xC+y, yC-x, 0);
     pixel_Desenhar(screen,xC-x, yC-y, 0);
     pixel_Desenhar(screen,xC-y, yC+x, 0);
     x++; E+= u; u+=2;
     if (v < 2 * E)
     {
         y--;
         E -= v;
         v -= 2;
      }
      if (x > y)
         break;
      pixel_Desenhar(screen,xC+y, yC+x, 0);
      pixel_Desenhar(screen,xC+x, yC-y, 0);
      pixel_Desenhar(screen,xC-y, yC-x, 0);
      pixel_Desenhar(screen,xC-x, yC+y, 0);
   }
}


void hexagono_Desenhar(SDL_Surface *screen, int x,int y, int lado, int salvar)
{
   int aux, auy;
   //Para dar a perspectiva, o angulo da esquerda passa a ser de 30,
   //ao invez de 60, que seria o habitual
   aux = (int)( lado * RAIZ3D2);
   auy = (lado/2);
   //Parte de Cima
   linha_Desenhar(screen,x,y,x+aux,y-auy,0);
   linha_Desenhar(screen,x+aux,y-auy,x+aux+lado,y-auy,0);
   linha_Desenhar(screen,x+aux+lado,y-auy,x+lado+2*aux,y,0);
   //Parte de Baixo
   linha_Desenhar(screen,x+lado+2*aux,y,x+aux+lado,y+auy,0);
   linha_Desenhar(screen,x+aux+lado,y+auy,x+aux,y+auy,0);
   linha_Desenhar(screen,x+aux,y+auy,x,y,0);
}

void AtualizaTela2D(GLuint texturaID, GLdouble proj[16],GLdouble modl[16], 
                    GLint viewPort[4],int xa, int ya, int xb, int yb, 
                    double profundidade)
{
       

   GLdouble x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4;
   gluUnProject(xa,(600-ya), profundidade, modl, proj, viewPort, &x1, &y1, &z1);
   gluUnProject(xa,(600-yb-1),profundidade, modl, proj, viewPort, &x2, &y2, &z2);
   gluUnProject(xb+1, (600-yb-1), profundidade, modl, proj, viewPort, 
                &x3, &y3, &z3);
   gluUnProject(xb+1,(600-ya),profundidade, modl, proj, viewPort, &x4, &y4, &z4);


   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texturaID);

   glBegin(GL_QUADS);
      glTexCoord2f(0,0);
      glVertex3f(x1, y1, z1);
	
      glTexCoord2f(1,0);
      glVertex3f(x4, y4, z4);
	
      glTexCoord2f(1,1);
      glVertex3f(x3, y3, z3);
	
      glTexCoord2f(0,1);
      glVertex3f(x2, y2, z2);
   glEnd();

   glDisable(GL_TEXTURE_2D);

/*   glFlush();
   SDL_GL_SwapBuffers();*/

   //glDeleteTextures(1,&texturaID);

}

/*********************************************************************
 *                     Aloca uma Textura no OPENGL                   *
 *********************************************************************/
void carregaTextura(SDL_Surface* img, GLuint* textID)
{
   glGenTextures(1,textID);
   glBindTexture(GL_TEXTURE_2D,*textID);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 
                               0, GL_RGB, GL_UNSIGNED_BYTE, 
                               img->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void carregaTexturaRGBA(SDL_Surface* img, GLuint* textID)
{
   glGenTextures(1,textID);
   glBindTexture(GL_TEXTURE_2D,*textID);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 
                               0, GL_RGBA, GL_UNSIGNED_BYTE, 
                               img->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

