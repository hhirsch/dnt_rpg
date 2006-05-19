#include "partEditor.h"

inline double deg2Rad(double x){return 3.1415927 * x/180.0;}

bool sair = false;
int tipo = 1;

barraTexto* bartSalvar;
GLfloat matrizVisivel[6][4]; /* Matriz do frustum atual */
GLdouble proj[16];
GLdouble modl[16];
GLint viewPort[4];

part1* p1;
part2* p2;
part3* p3;
part4* p4;
part5* p5;
part6* p6;
part* p;


void part::updateMaxLive(int mLive)
{
   maxLive = mLive;
}

int part::getMaxLive()
{
   return(maxLive);
}

void part::updateMaxParticles(int mLive)
{
   maxParticles = mLive;
}

int part::getMaxParticles()
{
   return(maxParticles);
}

void part::updateCenterX(float center)
{
   centerX = center;
}

float part::getCenterX()
{
   return(centerX);
}

void part::updateCenterY(float center)
{
   centerY = center;
}

float part::getCenterY()
{
   return(centerY);
}

void part::updateCenterZ(float center)
{
   centerZ = center;
}

float part::getCenterZ()
{
   return(centerZ);
}


/************************************************************************
 *                     Redmensionamento da Janela SDL                   *
 ************************************************************************/
void Redmensiona(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei) screen->w / (GLsizei) screen->h, 1.0, 650.0);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/************************************************************************
 *                     Chamada Inicial SDL                              *
 ************************************************************************/
void Iniciar(SDL_Surface *screen)
{
  
   Redmensiona(screen);
   
   /* Limpa */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);

   /* Define detalhamento */
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Definicao da Luz */
   /*GLfloat light_ambient[] = { 0.8, 0.6, 0.6, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 30.0, 10.0, 20.0, 0.0 };
   
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/

   GLfloat light_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   
   /* Habilita a iluminacao */
   glEnable(GL_LIGHTING);
  
}

/************************************************************************
 *                     Chamada de Erro de Invocacao                     *
 ************************************************************************/
void erro()
{
   printf("/\n");
   printf("| Use:\n");
   printf("| To edit an existent Particle: partEditor -e file.par\n\n");
   exit(2);
}




/************************************************************************
 *                     Trata Botao Sair                                 *
 ************************************************************************/
int botaoSair(void *jan,void *ljan, SDL_Surface *screen)
{
   sair = true;
   return(1);
}

int botaoSalvar(void *jan,void *ljan, SDL_Surface *screen)
{
   switch(tipo)
   {
      case 1:
             { 
                p1->Save(bartSalvar->texto);
                break;
             }
      case 2:
             {
                p2->Save(bartSalvar->texto);
                break;
             }
      case 3:
             {
                p3->Save(bartSalvar->texto);
                break;
             }
      case 4:
             {
                p4->Save(bartSalvar->texto);
                break;
             }
      case 5:
             {
               p5->Save(bartSalvar->texto);
               break;
             }
      case 6:
             {
               p6->Save(bartSalvar->texto);
               break;
             }
   }
   printf("With a lot of hope, i've saved as: %s\n",bartSalvar->texto.c_str());
   return(1);
}


void deleteParticle()
{
   switch(tipo)
   {
      case 1:
             { 
                delete(p1);
                break;
             }
      case 2:
             {
                delete(p2);
                break;
             }
      case 3:
             {
                delete(p3);
                break;
             }
      case 4:
             {
                delete(p4);
                break;
             }
      case 5:
             {
               delete(p5);
               break;
             }
      case 6:
             {
               delete(p6);
               break;
             }
   }
}

void createParticle(char* entrada)
{
    switch(tipo)
   {
      case 1:
             { 
                p1 = new part1(50,60,120,entrada);
                p = (part*) p1;
                break;
             }
      case 2:
             {
                p2 = new part2(50,0,120,entrada);
                p = (part*) p2;
                break;
             }
      case 3:
             {
                p3 = new part3(50,20,120);
                p = (part*) p3;
                break;
             }
      case 4:
             {
                p4 = new part4(50,0,120,entrada);
                p = (part*) p4;
                break;
             }
      case 5:
             {
               p5 = new part5(50,30,120,entrada);
               p = (part*) p5;
               break;
             }
      case 6:
             {
               p6 = new part6(50,250,120,entrada);
               p = (part*) p6;
               break;
             }
      default:
             {
                printf("What the hell is type %d???\n",tipo);
                erro();
                break;
             }
   }
}


void actualizeParticle(double segundos)
{
   switch(tipo)
         {
           case 1:
                  { 
                     p1->NextStep(segundos);
                     break;
                  }
           case 2:
                  {
                     p2->NextStep(segundos);
                     break;
                  }
           case  3:
                  {
                     p3->NextStep(segundos);
                     break;
                  }
           case 4:
                  {
                     p4->NextStep(segundos);
                     break;
                  }
           case 5:
                  {
                    p5->NextStep(segundos);
                    break;
                  }
           case 6:
                  {
                    p6->NextStep(segundos);
                    break;
                  }
         }
}

/************************************************************************/
/*                   O TRECO DE MUDAR BARRA TEXTO                       */
/************************************************************************/

void editedMaxLive(barraTexto* bart,SDL_Surface *screen)
{
   int i;
   sscanf(bart->texto.c_str(),"%d",&i);
   p->updateMaxLive(i);
}

void editedMaxParticles(barraTexto* bart,SDL_Surface *screen)
{
   int i;
   sscanf(bart->texto.c_str(),"%d",&i);
   p->updateMaxParticles(i);
   p->Save("temp.tmp");
   deleteParticle();
   createParticle("temp.tmp");
}

void editedCenterX(barraTexto* bart,SDL_Surface *screen)
{
   float i;
   sscanf(bart->texto.c_str(),"%f",&i);
   p->updateCenterX(i);
}

void editedCenterY(barraTexto* bart,SDL_Surface *screen)
{
   float i;
   sscanf(bart->texto.c_str(),"%f",&i);
   p->updateCenterY(i);
}

void editedCenterZ(barraTexto* bart,SDL_Surface *screen)
{
   float i;
   sscanf(bart->texto.c_str(),"%f",&i);
   p->updateCenterZ(i);
}

/************************************************************************
 *                     Escopo Principal                                 *
 ************************************************************************/
int main(int argc, char **argv)
{
   printf(" DccNitghtmare - Particle Editor\n");
   /* Inicia ou abre mapa Existente */
   char* entrada = ""; /*Arquivo de entrada*/
   int chamadaCorreta=0;
   char c;
   
   /* Cosntantes de CAMERA */
      double theta=37;
      double phi=0;
      double d=210;
      double centroX = 70;
      double centroZ = 208;
      double centroY = 30;
      double cameraX = centroX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
      double cameraY = centroY + (float) d * sin(deg2Rad(theta));
      double cameraZ = centroZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
      

   int novo = 0;

   

   while((c=getopt(argc,argv,"e:a:l:n")) != -1){
      switch(c){
         case 'e':{
                     entrada = optarg;
                     chamadaCorreta = 1; 
                     novo = 0;
                     break;
                  }
	 case 'a':break;  
         case 'l':break;
         default: erro();
      } 
   } 

   if(!chamadaCorreta) erro();

   printf("Type of System ( 1 .. 6 ): ");
   scanf("%d",&tipo);

   /* Inicia o Sistema Bizarro */ 
   SDL_Surface* screen;
   Farso_Iniciar(&screen,"DccNiTghtmare's Particle Editor");
   Iniciar(screen);

   createParticle(entrada);

   Uint8 Mbotao;
   Uint8 *teclas;
   int mouseX,mouseY;

   interface* gui = new interface(NULL);
   janela* janSalvar, *janEditar;

   char aux[10];

   janSalvar = gui->ljan->InserirJanela(0,0,127,63,"Actions",1,1,NULL,NULL);
   janSalvar->objetos->InserirBotao(10,37,50,55,janSalvar->Cores.corBot.R,
                                                janSalvar->Cores.corBot.G,
                                                janSalvar->Cores.corBot.B,
                                                "Save",1,&botaoSalvar);
   janSalvar->objetos->InserirBotao(77,37,117,55,janSalvar->Cores.corBot.R,
                                                janSalvar->Cores.corBot.G,
                                                janSalvar->Cores.corBot.B,
                                                "Exit",1,&botaoSair);
   bartSalvar = janSalvar->objetos->InserirBarraTexto(10,17,118,33,
                                         entrada,
                                         0,NULL);

   janSalvar->fechavel = 0;
   janSalvar->Abrir(gui->ljan);

   janEditar = gui->ljan->InserirJanela(0,64,127,575,"Edit",1,1,NULL,NULL);
   
   janEditar->objetos->InserirQuadroTexto(3,17,60,33,0,"MaxLive");
   sprintf(aux, "%d", p->getMaxLive());
   janEditar->objetos->InserirBarraTexto(60,17,123,33,aux,0,&editedMaxLive);

   janEditar->objetos->InserirQuadroTexto(3,37,60,53,0,"MaxParts");
   sprintf(aux, "%d", p->getMaxParticles());
   janEditar->objetos->InserirBarraTexto(60,37,123,53,aux,0,&editedMaxParticles);

   janEditar->objetos->InserirQuadroTexto(3,57,60,73,0,"CenterX");
   sprintf(aux, "%.3f", p->getCenterX());
   janEditar->objetos->InserirBarraTexto(60,57,123,73,aux,0,&editedCenterX);

   janEditar->objetos->InserirQuadroTexto(3,77,60,93,0,"CenterY");
   sprintf(aux, "%.3f", p->getCenterY());
   janEditar->objetos->InserirBarraTexto(60,77,123,93,aux,0,&editedCenterY);

   janEditar->objetos->InserirQuadroTexto(3,97,60,113,0,"CenterZ");
   sprintf(aux, "%.3f", p->getCenterZ());
   janEditar->objetos->InserirBarraTexto(60,97,123,113,aux,0,&editedCenterZ);


   janEditar->fechavel = 0;
   janEditar->Abrir(gui->ljan);

   float wx,wy,wz;
   double xReal, zReal, yReal;
   double varX, varZ;

   double varTempo, segundos;
   double ultimaLeitura = 0;
   

   while(!sair)
   {

      varTempo = (SDL_GetTicks() - ultimaLeitura);
      if(varTempo >= 20)
      {
         ultimaLeitura = SDL_GetTicks();
         segundos = varTempo / 1000.0;
         SDL_PumpEvents();
         Mbotao = SDL_GetMouseState(&mouseX,&mouseY);
         teclas = SDL_GetKeyState(NULL);

         wx = mouseX; wy = 600-mouseY; 

         
              
        // glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
        // gluUnProject(wx,wy,wz,modl,proj,viewPort, &xReal, &yReal, &zReal);

         if(gui->ManipulaEventos(mouseX,mouseY,Mbotao,teclas) == NADA)
         {
            if(teclas[SDLK_UP] || teclas[SDLK_DOWN])
            {
               varX = 2.0 * sin(deg2Rad(phi));
               varZ = 2.0 * cos(deg2Rad(phi));
               if(teclas[SDLK_UP]) 
               {
                    varX *= -1;
                    varZ *= -1;
               }
               centroX += varX;
               centroZ += varZ;
            }
            if(teclas[SDLK_RIGHT] || teclas[SDLK_LEFT])
            {
               varX = 2.0 * sin(deg2Rad(phi)+deg2Rad(90));
               varZ = 2.0 * cos(deg2Rad(phi)+deg2Rad(90));
               if(teclas[SDLK_LEFT])
               {
                   varX *= -1;
                   varZ *= -1;
               }
               centroX += varX;
               centroZ += varZ;
            }
            if(teclas[SDLK_PAGEUP]) // Sobe com a camera ate visao de cima
            {
               theta += 1;
               if(theta > 89) 
                  theta = 89;
            }
            if(teclas[SDLK_PAGEDOWN]) // desce com a camera ate visao em 1ª pessoa
            {
               theta -= 1;
               if(theta < 0)
                  theta = 0;
            }
            if(teclas[SDLK_HOME])
            {
               d -= 1;
               if (d<1) d = 1;
            }
            if(teclas[SDLK_END])
            {
               d += 1;
               if (d>300) d = 300;
            }
            if(teclas[SDLK_PERIOD])
            {
               phi -= 1;
            }
            if(teclas[SDLK_COMMA])
            {
               phi += 1;
            }

            if(teclas[SDLK_r])
            {
               deleteParticle();
               createParticle(entrada);
            }

         }

         /* Draw */
  
         glClearColor(0,0,0,0);
         glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

         glLoadIdentity();
         /* Redefine a posicao dinamica da camera */
         cameraX = centroX+(float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
         cameraY = centroY+(float) d * sin(deg2Rad(theta));
         cameraZ = centroZ+(float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
         gluLookAt(cameraX,cameraY,cameraZ, centroX,centroY,centroZ,0,1,0);
         AtualizaFrustum(matrizVisivel,proj,modl);
 
         
         /* Part Draw */
         glPushMatrix();
           actualizeParticle(segundos);
         glPopMatrix();

         glDisable(GL_LIGHTING);
         glDisable(GL_DEPTH_TEST);
/*         glDisable(GL_TEXTURE_2D);
         glDisable(GL_BLEND);
         glDisable(GL_COLOR_MATERIAL);*/
         //glPushMatrix();
           
            gui->Desenhar(proj,modl,viewPort);
            
         //glPopMatrix();

         glEnable(GL_LIGHTING);
         glEnable(GL_DEPTH_TEST);
         glFlush();
         SDL_GL_SwapBuffers();
      }

   }

   delete(gui);

   deleteParticle(); 

}

