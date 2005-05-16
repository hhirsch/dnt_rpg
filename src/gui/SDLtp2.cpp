/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include "farso.h"

#define DELTACAMERA 0.1

GLfloat dx = 0.0,      // deslocamento da camera no eixo x
        dy = 0.0,      // deslocamento da camera no eixo y
        dz = 0.0,      // deslocamento da camera no eixo z
        s = 0.0,       // rotacao da cacamba
        tx = 0.0,      // deslocamento geral
        direcao = 0.0, // orientacao geral
        c = 0.0,       // deslocamento de queda do bloco
        btx = 0.0,     // deslocamento do bloco
        bdir = 0.0,    // direcionamento do bloco
        bs = 0.0;      // rotacao do bloco 
        int primeiro = 0;

static GLfloat planoVertices[4][3] = {
  { 0.0, 0.0, 100.0 },
  { 100.0, 0.0, 100.0 },
  { 100.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
};

/* Desenha um pequeno pedaco do plano. Baseado em exemplo do redbook */
void desenhaPlano(void)
{
  glDisable(GL_LIGHTING);
  glColor3f(0.5,0.5,0.5);
  glBegin(GL_QUADS);
    glVertex3fv(planoVertices[0]);
    glVertex3fv(planoVertices[1]);
    glVertex3fv(planoVertices[2]);
    glVertex3fv(planoVertices[3]);
  glEnd();

  glEnable(GL_LIGHTING);
}

/* Funcao para iniciar o OpenGL */
void init(void)
{

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

   /* Luz no infinito, imitando o Sol */
   GLfloat light_position[] = { 0.0, 0.0, 100.0, 0.0 };
   GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

   /* Define as propriedades de material */
   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
    
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   /* Habilita o teste de profundidade dos poligonos */
   glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_FLAT);

   

   /*glClear ((GL_COLOR_BUFFER_BIT));
   glClear (GL_DEPTH_BUFFER_BIT);
   glLoadIdentity (); */
#if 0
    static GLfloat pos[4] =
    {5.0, 5.0, 10.0, 0.0};
    static GLfloat red[4] =
    {0.8, 0.1, 0.0, 1.0};
    static GLfloat green[4] =
    {0.0, 0.8, 0.2, 1.0};
    static GLfloat blue[4] =
    {0.2, 0.2, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
#endif
}

typedef GLfloat vertex[3];

vertex pt[8] = { {-0.5,-0.5,-0.5},{-0.5,0.5,-0.5},{0.5,-0.5,-0.5},{0.5,0.5,-0.5},
                     {-0.5,-0.5,0.5},{-0.5,0.5,0.5},{0.5,-0.5,0.5},{0.5,0.5,0.5}};

void quad(GLint n1, GLint n2, GLint n3, GLint n4)
{
   glBegin(GL_QUADS);
     glVertex3fv(pt[n1]);
     glVertex3fv(pt[n2]);
     glVertex3fv(pt[n3]);
     glVertex3fv(pt[n4]);
   glEnd();
}

void cubo()
{
   quad(6,2,3,7);
   quad(5,1,0,4);
   quad(7,3,1,5);
   quad(4,0,2,6);
   quad(2,0,1,3);
   quad(7,5,4,6);
}

/* Desenha o veiculo, de acordo com os parametros */
void desenha()
{
   /* Cores de Reflexao das partes do caminhao */
   GLfloat frente[] = { 1.0, 0.0, 0.0, 1.0 };
   GLfloat cacamba[] =  { 0.9, 0.6, 0.0, 1.0 };
   GLfloat objcacamba[] = { 0.0, 0.7, 0.7, 1.0 };
   GLfloat cabine[] = {1.0, 0.0, 0.0, 1.0};
   GLfloat suporte[] = {0.07, 0.7, 0.15, 1.0};
   GLfloat pneus[] = {0.0, 0.0, 0.0, 1.0};
   GLfloat vidros[] = {0.0,0.0,1.0,1.0};
   GLfloat farol_tras[] = {1.0, 0.0 ,0.0 ,1.0};
   GLfloat farol_frente[] = {0.8, 0.8 ,0.0 ,1.0};
   GLfloat placa[] = {1.0,1.0,1.0,1.0};

//   if (primeiro<=2) {
     glClear ((GL_COLOR_BUFFER_BIT));
     glClear (GL_DEPTH_BUFFER_BIT);
    // primeiro++;
  // }

   glLoadIdentity ();
   /* Posicionamento dinamico da camera */
   gluLookAt (7.0+dx,7.0+ dy, 7.0+dz, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
 
   /* Desenho simbolico do plano utilizado */
   glPushMatrix();
   desenhaPlano();
   glPopMatrix();

   
   glPushMatrix();
//   glScalef(0.1,0.1,0.1);
   glRotatef(direcao,0,1,0);         // direcao do veiculo
   glTranslatef(0.0+tx, 0.5, 0.0);   // locomocao do veiculo

   //rodas
   GLUquadricObj *pneu[6];
   //vetores de translacao
   GLfloat trlz[] = {-0.01, -0.01, -0.01, 2.01, 2.01, 2.01};   
   GLfloat trlx[] = {0.4 , 1.5, 4.5, 0.4 , 1.5, 4.5};
   int pn; //auxiliar para os pneus
   for (pn = 0; pn<6;pn++){
     glPushMatrix();
     glColor3f(0.0,0.8,0.0);
     glTranslatef(trlx[pn],0.0,trlz[pn]);
     glScalef(0.5,0.5,0.0);
     pneu[pn] = gluNewQuadric();
     gluQuadricDrawStyle(pneu[pn], GLU_FILL);
     glMaterialfv(GL_FRONT, GL_EMISSION, pneus);
     gluDisk(pneu[pn],0.2,0.8,12,12);
     glPopMatrix();
   }

   //vidros laterais
   GLfloat posvidro[] = {2.01, -0.01};
   int vdr; //aux para os vidros
   for (vdr = 0; vdr< 2;vdr++){
     glPushMatrix();
     glMaterialfv(GL_FRONT, GL_EMISSION, vidros);
     glBegin(GL_POLYGON);
       glColor3f(0.0,0.0,1.0);
       glVertex3f(3.8,1.0,posvidro[vdr]);
       glVertex3f(4.7,1.0,posvidro[vdr]);
       glColor3f(0.0,0.0,0.5);
       glVertex3f(4.7,2.0,posvidro[vdr]);
       glVertex3f(3.8,2.0,posvidro[vdr]);
     glEnd();
     glPopMatrix();
   }

   //vidro dianteiro
   glPushMatrix();
   glMaterialfv(GL_FRONT, GL_EMISSION, vidros);
   glBegin(GL_POLYGON);
     glColor3f(0.0,0.0,1.0);
     glVertex3f(5.01,1.1,0.2);
     glVertex3f(5.01,1.1,0.95);
     glColor3f(0.0,0.0,0.5);
     glVertex3f(5.01,1.9,0.95);
     glVertex3f(5.01,1.9,0.2);
   glEnd();
   glMaterialfv(GL_FRONT, GL_EMISSION, vidros);
   glBegin(GL_POLYGON);
     glColor3f(0.0,0.0,1.0);
     glVertex3f(5.01,1.1,1.8);
     glVertex3f(5.01,1.1,1.05);
     glColor3f(0.0,0.0,0.5);
     glVertex3f(5.01,1.9,1.05);
     glVertex3f(5.01,1.9,1.8);
   glEnd();
   glPopMatrix();

   //farol traseiro
   glPushMatrix();
   glRotatef(0.0+s,0.0,0.0,2.0);
   glMaterialfv(GL_FRONT, GL_EMISSION, farol_tras);
   glBegin(GL_POLYGON);
     glColor3f(1.0,0.0,0.0);
     glVertex3f(-0.01,0.4,1.8);
     glVertex3f(-0.01,0.4,1.5);
     glColor3f(0.5,0.0,0.0);
     glVertex3f(-0.01,0.7,1.5);
     glVertex3f(-0.01,0.7,1.8);
   glEnd();
   glMaterialfv(GL_FRONT, GL_EMISSION, farol_tras);
   glBegin(GL_POLYGON);
     glColor3f(1.0,0.0,0.0);
     glVertex3f(-0.01,0.4,0.2);
     glVertex3f(-0.01,0.4,0.5);
     glColor3f(0.5,0.0,0.0);
     glVertex3f(-0.01,0.7,0.5);
     glVertex3f(-0.01,0.7,0.2);
   glEnd();
   glPopMatrix();

   //Farol Dianteiro
   glPushMatrix();
   glMaterialfv(GL_FRONT, GL_EMISSION, farol_frente);
   glBegin(GL_POLYGON);
     glColor3f(1.0,0.0,0.0);
     glVertex3f(5.51,0.4,1.8);
     glVertex3f(5.51,0.4,1.5);
     glColor3f(0.5,0.0,0.0);
     glVertex3f(5.51,0.7,1.5);
     glVertex3f(5.51,0.7,1.8);
   glEnd();
   glMaterialfv(GL_FRONT, GL_EMISSION, farol_frente);
   glBegin(GL_POLYGON);
     glColor3f(1.0,0.0,0.0);
     glVertex3f(5.51,0.4,0.2);
     glVertex3f(5.51,0.4,0.5);
     glColor3f(0.5,0.0,0.0);
     glVertex3f(5.51,0.7,0.5);
     glVertex3f(5.51,0.7,0.2);
   glEnd();
   glPopMatrix();

   //Placa
   glPushMatrix();
   glMaterialfv(GL_FRONT, GL_EMISSION, placa);
   glBegin(GL_POLYGON);
     glColor3f(1.0,0.0,0.0);
     glVertex3f(5.51,0.1,1.4);
     glVertex3f(5.51,0.1,0.6);
     glColor3f(0.5,0.0,0.0);
     glVertex3f(5.51,0.5,0.6);
     glVertex3f(5.51,0.5,1.4);
   glEnd();
   glPopMatrix();

   //suporte da cacamba
   glPushMatrix();
   glColor3f(1.0,0.0,0.0);
   glTranslatef(1.5,0.1,1.0);
   glScalef(3.0,0.2,2.0);
   glMaterialfv(GL_FRONT, GL_EMISSION, suporte);
   //glutSolidCube(1.0);
   cubo();
/*   glBegin(GL_POLYGON);
      glVertex3f(-0.5,0.5,0.5);
      glVertex3f(-0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,0.5);
      
      glVertex3f(-0.5,-0.5,0.5);
      glVertex3f(-0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,0.5);
   glEnd();*/
   glPopMatrix();

   //cacamba
   glPushMatrix();
   glColor3f(0.0,0.0,1.0);
   glRotatef(0.0+s,0.0,0.0,2.0);
   glTranslatef(1.5,0.6,1.0);
   glScalef(3.0,0.8,2.0);
   glMaterialfv(GL_FRONT, GL_EMISSION, cacamba);   
   //glutSolidCube(1.0);
   cubo();
/*   glBegin(GL_SURFACE);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(-0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,0.5);
      
      glVertex3f(-0.5,-0.5,0.5);
      glVertex3f(-0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,0.5);
   glEnd();*/
   glPopMatrix();

   //frente
   glPushMatrix();
   glColor3f(0.5,0.5,1.0);
   glTranslatef(5.25,0.5,1.0);
   glScalef(0.5,1.0,2.0);
   glMaterialfv(GL_FRONT, GL_EMISSION, frente);
//   glutSolidCube(1.0);
   cubo();
/*   glBegin(GL_POLYGON);
      glVertex3f(-0.5,0.5,0.5);
      glVertex3f(-0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,0.5);
      
      glVertex3f(-0.5,-0.5,0.5);
      glVertex3f(-0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,0.5);
   glEnd();*/
   glPopMatrix();

   //cabine
   glPushMatrix();
   glColor3f(1.0,0.0,0.0);
   glTranslatef(4.0,1.0,1.0);
   glScalef(2.0,2.0,2.0);
   glMaterialfv(GL_FRONT, GL_EMISSION, cabine);
//   glutSolidCube(1.0);
   cubo();
/*   glBegin(GL_POLYGON);
      glVertex3f(-0.5,0.5,0.5);
      glVertex3f(-0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,0.5);
      
      glVertex3f(-0.5,-0.5,0.5);
      glVertex3f(-0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,0.5);
   glEnd();*/
   glPopMatrix();

   glPopMatrix();

   //objeto na cacamba
   glPushMatrix();
   //glScalef(0.1,0.1,0.1);
   glRotatef(bdir,0,1,0);
   glTranslatef(0.0+btx, 0.4, 0.0);
   glColor3f(0.0,1.0,0.0);
//   GLfloat cor[] = {0.8,0.0,0.0,1.0};
   glRotatef(0.0+bs,0.0,0.0,2.0);
   glTranslatef(1.75+c,0.95,1.25);
   glScalef(0.5,1.0,0.5);
   glMaterialfv(GL_FRONT, GL_EMISSION, objcacamba); 
//   glutSolidCube(1.0);
   cubo();
/*   glBegin(GL_POLYGON);
      glVertex3f(-0.5,0.5,0.5);
      glVertex3f(-0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,-0.5);
      glVertex3f(0.5,0.5,0.5);
      
      glVertex3f(-0.5,-0.5,0.5);
      glVertex3f(-0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,-0.5);
      glVertex3f(0.5,-0.5,0.5);
   glEnd();*/
   glPopMatrix();


   //glutSwapBuffers();


   glFlush();
}   
   
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
//   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 50.0);
   glOrtho(-21.0,21.0,-21.0,21.0,-50.0,50.0);
//   gluperspective();
   glMatrixMode (GL_MODELVIEW);
}

void keyboard(SDL_Surface *screen)
{
   SDL_PumpEvents();
   Uint8 *keys;
   keys = SDL_GetKeyState(NULL);
   if ( keys[SDLK_q] )
      exit(0);
   else if (keys[SDLK_r])    // reseta os valores iniciais          
   {
         dx = 0.0; dy = 0.0; dz = 0.0; s = 0.0; c = 0.0;
         tx = 0.0; direcao = 0.0; btx = 0.0; bdir = 0.0;
         bs = 0.0;
         desenha();
         SDL_GL_SwapBuffers();
   }
   /* MOVIMENTAÇÃO DA CÂMERA */
   else if (keys[SDLK_e])    // esquerda
   {
         dx += DELTACAMERA;
         desenha();
         SDL_GL_SwapBuffers();

   }
   else if (keys[SDLK_d])    // direita
   {
         dx -= DELTACAMERA;;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_c])    // cima
   {
         dy += DELTACAMERA;;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_b])    // baixo
   {
         dy -= DELTACAMERA;;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_t])    //trás
   {
         dz += DELTACAMERA;;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_f])    // frente
   {
         dz -= DELTACAMERA;;
         desenha();
         SDL_GL_SwapBuffers();
   }
    /* Movimentacao do Caminhao */
   else if (keys[SDLK_s])             //Sobe a cacamba
   {
         s += 0.5; 
         if (c > -1.75) { 
            c -= 0.05;
         }
         if (s>50) s = 50;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_a])            //Desce a cacamba
   {
         s -= 0.5;
         if (s < 0) s = 0.0;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_i])            // Anda para frente
   {
         tx += 0.1;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_k])            // Anda para tras
   {
         tx -= 0.1;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_o])            // Anda para frente com volante à direita
   {
         direcao -= 0.2;
         tx += 0.08;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_u])
   {        // Anda para frente com volante a esquerda
         direcao += 0.2;
         tx += 0.08;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_j])    // Anda para tras com volante a esquerda
   {
         direcao -= 0.2;
         tx -= 0.08;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_l]) // Anda para frente com volante a direita
   {
         direcao += 0.2;
         tx -= 0.08;
         desenha();
         SDL_GL_SwapBuffers();
   }
   else if (keys[SDLK_m])
   {
         /*SDL_Rect ret;
         ret.x = 0;
         ret.y = 0;
         ret.h = 480;
         ret.w = 640;
         SDL_Surface *sc = SDL_CreateRGBSurface(SDL_HWSURFACE,640,480,16,
                                     0,0,0,0);
         SDL_Surface *dst = SDL_CreateRGBSurface(SDL_HWSURFACE,800,600,16,
                                        0,0,0,0);
         SDL_SetColorKey(sc,SDL_SRCCOLORKEY,SDL_MapRGB(sc->format,0,0,0));*/
         //SDL_SetAlpha(sc, 0, 0);
         //SDL_SetAlpha(screen,0,0);
         cor_Definir(0,0,0);
         //retangulo_Colorir(sc,0,0,639,479,1);
         interface* interf = new interface("../../data/pics/akira.bmp");
         janela* jan;
         jan = interf->ljan->InserirJanela(330,100,500,350,"Janela",1,1,NULL,NULL);
         jan->Ativar(interf->ljan,interf->tela2D);
         jan=interf->ljan->InserirJanela(10,10,200,220,"Hehehe",1,1,NULL,NULL);
         jan->Ativar(interf->ljan,interf->tela2D);
         while ((jan!=NULL) && interf->ManipulaEventos(screen))
         { 
        //    desenha();
        //    SDL_BlitSurface(screen,NULL,dst,NULL);
        //    SDL_BlitSurface(sc,NULL,dst,NULL);
         //   SDL_Flip(dst);
//            SDL_BlitSurface(sc,&ret,screen,NULL);
            //SDL_UpdateRects(screen,1,&ret);
    //        SDL_GL_SwapBuffers();
            //SDL_Flip(screen);
         }
         delete(interf);
//         SDL_FreeSurface(sc);
  //       SDL_FreeSurface(dst);
         desenha();
         SDL_GL_SwapBuffers();
   }
      /* Se o pacote da cacamba nao tiver caido, 
      faz ele acompanhar o caminhao */
   if (c> -1.75) {
     btx = tx;
     bdir = direcao;
     bs = s;
   }
}

void ReshapeGL(int width, int height)
{
   glViewport(0,0,(GLsizei)(width),(GLsizei)(height));
   glMatrixMode(GL_PROJECTION);	
   glLoadIdentity();

//   gluPerspective(45.0f,(GLfloat)(width)/(GLfloat)(height),1.0f,100.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   return;

}


int main(int argc, char** argv)
{
/*   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(desenha);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();*/

   SDL_Surface *screen;
   Farso_Iniciar(&screen,"Tp2");
   /*SDL_Init(SDL_INIT_VIDEO);
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,5);
   SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
   SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


   screen = SDL_SetVideoMode(800,600,16,SDL_HWSURFACE | SDL_DOUBLEBUF  | SDL_OPENGLBLIT);
   if (!screen  )
   {
      printf("Deu pau na tela\n");
      SDL_Quit();
      exit(2);
   }*/
//   ReshapeGL(800,600);
   reshape(800,600);

   //SDL_WM_SetCaption("Tp2","tp2");
   init();
   desenha();
   SDL_GL_SwapBuffers();
   
   while(1) keyboard(screen);

   return 0;
}
