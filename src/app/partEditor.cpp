#include "partEditor.h"
#include "../etc/dirs.h"
#include "../engine/camera.h"

bool sair = false;
int tipo = 1;

textBar* bartSalvar;
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
part7* p7;
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


void part::updateGravity(float grv)
{
   gravity = grv;
}

float part::getGravity()
{
   return(gravity);
}


void part::updateInitR(float cor)
{
   initR = cor;
}

float part::getInitR()
{
  return(initR);
}

void part::updateInitG(float cor)
{
   initG = cor;
}

float part::getInitG()
{
  return(initG);
}

void part::updateInitB(float cor)
{
   initB = cor;
}

float part::getInitB()
{
  return(initB);
}

void part::updateFinalR(float cor)
{
   finalR = cor;
}

float part::getFinalR()
{
  return(finalR);
}

void part::updateFinalG(float cor)
{
   finalG = cor;
}

float part::getFinalG()
{
  return(finalG);
}

void part::updateFinalB(float cor)
{
   finalB = cor;
}

float part::getFinalB()
{
  return(finalB);
}

void part::updateAlpha(float a)
{
  alpha = a;
}

float part::getAlpha()
{
  return(alpha);
}

void part::updateDMultCenterX(float d)
{
   dMultCenter[0] = d;
}
float part::getDMultCenterX()
{
   return(dMultCenter[0]);
}

void part::updateDMultCenterY(float d)
{
   dMultCenter[1] = d;
}
float part::getDMultCenterY()
{
   return(dMultCenter[1]);
}

void part::updateDMultCenterZ(float d)
{
   dMultCenter[2] = d;
}
float part::getDMultCenterZ()
{
   return(dMultCenter[2]);
}

void part::updateDSumCenterX(float d)
{
   dSumCenter[0] = d;
}
float part::getDSumCenterX()
{
   return(dSumCenter[0]);
}

void part::updateDSumCenterY(float d)
{
   dSumCenter[1] = d;
}
float part::getDSumCenterY()
{
   return(dSumCenter[1]);
}

void part::updateDSumCenterZ(float d)
{
   dSumCenter[2] = d;
}
float part::getDSumCenterZ()
{
   return(dSumCenter[2]);
}

void part::updateDMultPosX(float d)
{
   dMultPos[0] = d;
}
float part::getDMultPosX()
{
   return(dMultPos[0]);
}

void part::updateDMultPosY(float d)
{
   dMultPos[1] = d;
}
float part::getDMultPosY()
{
   return(dMultPos[1]);
}

void part::updateDMultPosZ(float d)
{
   dMultPos[2] = d;
}
float part::getDMultPosZ()
{
   return(dMultPos[2]);
}

void part::updateDSumPosX(float d)
{
   dSumPos[0] = d;
}
float part::getDSumPosX()
{
   return(dSumPos[0]);
}

void part::updateDSumPosY(float d)
{
   dSumPos[1] = d;
}
float part::getDSumPosY()
{
   return(dSumPos[1]);
}

void part::updateDSumPosZ(float d)
{
   dSumPos[2] = d;
}
float part::getDSumPosZ()
{
   return(dSumPos[2]);
}

void part::updateDMultVelX(float d)
{
   dMultVel[0] = d;
}
float part::getDMultVelX()
{
   return(dMultVel[0]);
}

void part::updateDMultVelY(float d)
{
   dMultVel[1] = d;
}
float part::getDMultVelY()
{
   return(dMultVel[1]);
}

void part::updateDMultVelZ(float d)
{
   dMultVel[2] = d;
}
float part::getDMultVelZ()
{
   return(dMultVel[2]);
}

void part::updateDSumVelX(float d)
{
   dSumVel[0] = d;
}
float part::getDSumVelX()
{
   return(dSumVel[0]);
}

void part::updateDSumVelY(float d)
{
   dSumVel[1] = d;
}
float part::getDSumVelY()
{
   return(dSumVel[1]);
}

void part::updateDSumVelZ(float d)
{
   dSumVel[2] = d;
}
float part::getDSumVelZ()
{
   return(dSumVel[2]);
}

void part::updateDMultColorR(float d)
{
   dMultColor[0] = d;
}
float part::getDMultColorR()
{
   return(dMultColor[0]);
}

void part::updateDMultColorG(float d)
{
   dMultColor[1] = d;
}
float part::getDMultColorG()
{
   return(dMultColor[1]);
}

void part::updateDMultColorB(float d)
{
   dMultColor[2] = d;
}
float part::getDMultColorB()
{
   return(dMultColor[2]);
}

void part::updateDSumColorR(float d)
{
   dSumColor[0] = d;
}
float part::getDSumColorR()
{
   return(dSumColor[0]);
}

void part::updateDSumColorG(float d)
{
   dSumColor[1] = d;
}
float part::getDSumColorG()
{
   return(dSumColor[1]);
}

void part::updateDSumColorB(float d)
{
   dSumColor[2] = d;
}
float part::getDSumColorB()
{
   return(dSumColor[2]);
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
   glDisable(GL_LIGHTING);
  
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
      case 7:
             {
               delete(p7);
               break;
             }
   }
}

void createParticle(string entrada)
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
      case 7:
             {
               p7 = new part7(50,80,120,entrada);
               p = (part*) p7;
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
                     p1->NextStep(matrizVisivel);
                     break;
                  }
           case 2:
                  {
                     p2->NextStep(matrizVisivel);
                     break;
                  }
           case  3:
                  {
                     p3->NextStep(matrizVisivel);
                     break;
                  }
           case 4:
                  {
                     p4->NextStep(matrizVisivel);
                     break;
                  }
           case 5:
                  {
                    p5->NextStep(matrizVisivel);
                    break;
                  }
           case 6:
                  {
                    p6->NextStep(matrizVisivel);
                    break;
                  }
            case 7:
                  {
                    p7->NextStep(matrizVisivel);
                    break;
                  }
         }
}

/************************************************************************/
/*                   O TRECO DE MUDAR BARRA TEXTO                       */
/************************************************************************/

void editedMaxLive(textBar* bart)
{
   int i;
   sscanf(bart->getText().c_str(),"%d",&i);
   p->updateMaxLive(i);
}

void editedMaxParticles(textBar* bart)
{
   int i;
   sscanf(bart->getText().c_str(),"%d",&i);
   p->updateMaxParticles(i);
   p->Save("temp.tmp");
   deleteParticle();
   createParticle("temp.tmp");
}

void editedCenterX(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateCenterX(i);
}

void editedCenterY(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateCenterY(i);
}

void editedCenterZ(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateCenterZ(i);
}

void editedGravity(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateGravity(i);
}

void editedInitR(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateInitR(i);
}

void editedInitG(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateInitG(i);
}

void editedInitB(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateInitB(i);
}

void editedFinalR(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateFinalR(i);
}

void editedFinalG(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateFinalG(i);
}

void editedFinalB(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateFinalB(i);
}

void editedAlpha(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateAlpha(i);
}

void editedDMultCenterX(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultCenterX(i);
}

void editedDMultCenterY(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultCenterY(i);
}

void editedDMultCenterZ(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultCenterZ(i);
}

void editedDSumCenterX(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumCenterX(i);
}

void editedDSumCenterY(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumCenterY(i);
}

void editedDSumCenterZ(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumCenterZ(i);
}

void editedDMultPosX(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultPosX(i);
}

void editedDMultPosY(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultPosY(i);
}

void editedDMultPosZ(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultPosZ(i);
}

void editedDSumPosX(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumPosX(i);
}

void editedDSumPosY(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumPosY(i);
}

void editedDSumPosZ(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumPosZ(i);
}

void editedDMultVelX(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultVelX(i);
}

void editedDMultVelY(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultVelY(i);
}

void editedDMultVelZ(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultVelZ(i);
}

void editedDSumVelX(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumVelX(i);
}

void editedDSumVelY(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumVelY(i);
}

void editedDSumVelZ(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumVelZ(i);
}

void editedDMultColorR(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultColorR(i);
}

void editedDMultColorG(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultColorG(i);
}

void editedDMultColorB(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDMultColorB(i);
}

void editedDSumColorR(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumColorR(i);
}

void editedDSumColorG(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumColorG(i);
}

void editedDSumColorB(textBar* bart)
{
   float i;
   sscanf(bart->getText().c_str(),"%f",&i);
   p->updateDSumColorB(i);
}


/************************************************************************
 *                     Escopo Principal                                 *
 ************************************************************************/
int main(int argc, char **argv)
{
   printf(" DccNitghtmare - Particle Editor 0.1\n");
   /* Inicia ou abre mapa Existente */
   string entrada = ""; /*Arquivo de entrada*/
   int chamadaCorreta=0;
   char c;
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

   printf("Type of System ( 1 .. 7 ): ");
   scanf("%d",&tipo);

   /* Get current data directorie */
   dirs dir;
   dir.findDataDirectories();

   /* Inicia o Sistema Bizarro */ 
   SDL_Surface* screen;
   Farso_Init(&screen,"DccNiTghtmare's Particle Editor", 800, 600, false, 0);
   Iniciar(screen);
   extensions ext;
   ext.defineAllExtensions();

   camera* gameCamera = new camera();

   button *buttonExit,
          *buttonSave;

   createParticle(entrada);

   Uint8 Mbotao;
   Uint8 *teclas;
   int mouseX,mouseY;

   interface* gui = new interface(NULL);
   window* janSalvar, *janEditar;

   char aux[10];

   janSalvar = gui->insertWindow(0,0,127,63,"Actions");
   buttonSave = janSalvar->getObjectsList()->insertButton(10,37,50,55,
                                                         "Save",1);
   buttonExit = janSalvar->getObjectsList()->insertButton(77,37,117,55,
                                                          "Exit",1);
   bartSalvar = janSalvar->getObjectsList()->insertTextBar(10,17,118,33,
                                         entrada, 0);
   gui->openWindow(janSalvar);

   janEditar = gui->insertWindow(0,64,127,575,"Edit");
   
   janEditar->getObjectsList()->insertTextBox(3,17,60,33,0,"MaxLive");
   sprintf(aux, "%d", p->getMaxLive());
   janEditar->getObjectsList()->insertTextBar(60,17,123,33,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,37,60,53,0,"MaxParts");
   sprintf(aux, "%d", p->getMaxParticles());
   janEditar->getObjectsList()->insertTextBar(60,37,123,53,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,57,60,73,0,"CenterX");
   sprintf(aux, "%.3f", p->getCenterX());
   janEditar->getObjectsList()->insertTextBar(60,57,123,73,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,77,60,93,0,"CenterY");
   sprintf(aux, "%.3f", p->getCenterY());
   janEditar->getObjectsList()->insertTextBar(60,77,123,93,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,97,60,113,0,"CenterZ");
   sprintf(aux, "%.3f", p->getCenterZ());
   janEditar->getObjectsList()->insertTextBar(60,97,123,113,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,117,60,133,0,"Gravity");
   sprintf(aux, "%.3f", p->getGravity());
   janEditar->getObjectsList()->insertTextBar(60,117,123,133,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,137,60,153,0,"InitR");
   sprintf(aux, "%.3f", p->getInitR());
   janEditar->getObjectsList()->insertTextBar(60,137,123,153,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,157,60,173,0,"InitG");
   sprintf(aux, "%.3f", p->getInitG());
   janEditar->getObjectsList()->insertTextBar(60,157,123,173,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,177,60,193,0,"InitB");
   sprintf(aux, "%.3f", p->getInitB());
   janEditar->getObjectsList()->insertTextBar(60,177,123,193,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,197,60,213,0,"FinalR");
   sprintf(aux, "%.3f", p->getFinalR());
   janEditar->getObjectsList()->insertTextBar(60,197,123,213,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,217,60,233,0,"FinalG");
   sprintf(aux, "%.3f", p->getFinalG());
   janEditar->getObjectsList()->insertTextBar(60,217,123,233,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,237,60,253,0,"FinalB");
   sprintf(aux, "%.3f", p->getFinalB());
   janEditar->getObjectsList()->insertTextBar(60,237,123,253,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,257,60,273,0,"Alpha");
   sprintf(aux, "%.3f", p->getAlpha());
   janEditar->getObjectsList()->insertTextBar(60,257,123,273,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,277,60,293,0,"MCntX");
   sprintf(aux, "%.3f", p->getDMultCenterX());
   janEditar->getObjectsList()->insertTextBar(60,277,123,293,aux,0);
   janEditar->getObjectsList()->insertTextBox(3,297,60,313,0,"MCntY");
   sprintf(aux, "%.3f", p->getDMultCenterY());
   janEditar->getObjectsList()->insertTextBar(60,297,123,313,aux,0);
   janEditar->getObjectsList()->insertTextBox(3,317,60,333,0,"MCntZ");
   sprintf(aux, "%.3f", p->getDMultCenterZ());
   janEditar->getObjectsList()->insertTextBar(60,317,123,333,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,337,60,353,0,"SCntX");
   sprintf(aux, "%.3f", p->getDSumCenterX());
   janEditar->getObjectsList()->insertTextBar(60,337,123,353,aux,0);
   janEditar->getObjectsList()->insertTextBox(3,357,60,373,0,"SCntY");
   sprintf(aux, "%.3f", p->getDSumCenterY());
   janEditar->getObjectsList()->insertTextBar(60,357,123,373,aux,0);
   janEditar->getObjectsList()->insertTextBox(3,377,60,393,0,"SCntZ");
   sprintf(aux, "%.3f", p->getDSumCenterZ());
   janEditar->getObjectsList()->insertTextBar(60,377,123,393,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,397,60,413,0,"MPosX");
   sprintf(aux, "%.3f", p->getDMultPosX());
   janEditar->getObjectsList()->insertTextBar(60,397,123,413,aux,0);
   janEditar->getObjectsList()->insertTextBox(3,417,60,433,0,"MPosY");
   sprintf(aux, "%.3f", p->getDMultPosY());
   janEditar->getObjectsList()->insertTextBar(60,417,123,433,aux,0);
   janEditar->getObjectsList()->insertTextBox(3,437,60,453,0,"MPosZ");
   sprintf(aux, "%.3f", p->getDMultPosZ());
   janEditar->getObjectsList()->insertTextBar(60,437,123,453,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,457,60,473,0,"SPosX");
   sprintf(aux, "%.3f", p->getDSumPosX());
   janEditar->getObjectsList()->insertTextBar(60,457,123,473,aux,0);
   janEditar->getObjectsList()->insertTextBox(3,474,60,490,0,"SPosY");
   sprintf(aux, "%.3f", p->getDSumPosY());
   janEditar->getObjectsList()->insertTextBar(60,474,123,490,aux,0);
   janEditar->getObjectsList()->insertTextBox(3,491,60,505,0,"SPosZ");
   sprintf(aux, "%.3f", p->getDSumPosZ());
   janEditar->getObjectsList()->insertTextBar(60,491,123,505,aux,0);

   gui->openWindow(janEditar);

   janEditar = gui->insertWindow(672,64,799,575,"Edit-2");

   janEditar->getObjectsList()->insertTextBox(3,17,60,33,0,"DMColorR");
   sprintf(aux, "%d", p->getMaxLive());
   janEditar->getObjectsList()->insertTextBar(60,17,123,33,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,37,60,53,0,"DMColorG");
   sprintf(aux, "%d", p->getMaxParticles());
   janEditar->getObjectsList()->insertTextBar(60,37,123,53,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,57,60,73,0,"DMColorB");
   sprintf(aux, "%.3f", p->getCenterX());
   janEditar->getObjectsList()->insertTextBar(60,57,123,73,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,77,60,93,0,"DSColorR");
   sprintf(aux, "%.3f", p->getCenterY());
   janEditar->getObjectsList()->insertTextBar(60,77,123,93,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,97,60,113,0,"DSColorG");
   sprintf(aux, "%.3f", p->getCenterZ());
   janEditar->getObjectsList()->insertTextBar(60,97,123,113,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,117,60,133,0,"DSColorB");
   sprintf(aux, "%.3f", p->getGravity());
   janEditar->getObjectsList()->insertTextBar(60,117,123,133,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,137,60,153,0,"DMVelX");
   sprintf(aux, "%.3f", p->getInitR());
   janEditar->getObjectsList()->insertTextBar(60,137,123,153,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,157,60,173,0,"DMVelY");
   sprintf(aux, "%.3f", p->getInitG());
   janEditar->getObjectsList()->insertTextBar(60,157,123,173,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,177,60,193,0,"DMVelZ");
   sprintf(aux, "%.3f", p->getInitB());
   janEditar->getObjectsList()->insertTextBar(60,177,123,193,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,197,60,213,0,"DSVelX");
   sprintf(aux, "%.3f", p->getFinalR());
   janEditar->getObjectsList()->insertTextBar(60,197,123,213,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,217,60,233,0,"DSVelY");
   sprintf(aux, "%.3f", p->getFinalG());
   janEditar->getObjectsList()->insertTextBar(60,217,123,233,aux,0);

   janEditar->getObjectsList()->insertTextBox(3,237,60,253,0,"DSVelZ");
   sprintf(aux, "%.3f", p->getFinalB());
   janEditar->getObjectsList()->insertTextBar(60,237,123,253,aux,0);

   gui->openWindow(janEditar);

   float wx,wy;

   double varTempo, segundos;
   double ultimaLeitura = 0;
   guiObject* obj = NULL;
   

   while(!sair)
   {

      varTempo = (SDL_GetTicks() - ultimaLeitura);
      if(varTempo >= UPDATE_RATE)
      {
         ultimaLeitura = SDL_GetTicks();
         segundos = varTempo / 1000.0;
         SDL_PumpEvents();
         Mbotao = SDL_GetMouseState(&mouseX,&mouseY);
         teclas = SDL_GetKeyState(NULL);

         wx = mouseX; wy = 600-mouseY; 

         
              
        // glReadPixels((int)wx,(int)wy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); 
        // gluUnProject(wx,wy,wz,modl,proj,viewPort, &xReal, &yReal, &zReal);

         int eventInfo = NOTHING;
         obj = gui->manipulateEvents(mouseX,mouseY,Mbotao,teclas,eventInfo);
         if(eventInfo == NOTHING)
         {
            gameCamera->doIO(teclas, Mbotao, mouseX, mouseY, DELTA_CAMERA);

            if(teclas[SDLK_r])
            {
               deleteParticle();
               createParticle(entrada);
            }
         }
         else if(eventInfo == PRESSED_BUTTON)
         {
            if(obj == buttonExit)
            {
               sair = true;
            }
            else if(obj == buttonSave)
            {
               switch(tipo)
               {
                  case 1:
                  { 
                     p1->Save(bartSalvar->getText());
                     break;
                  }
                  case 2:
                  {
                     p2->Save(bartSalvar->getText());
                     break;
                  }
                  case 3:
                  {
                     p3->Save(bartSalvar->getText());
                     break;
                  }
                  case 4:
                  {
                     p4->Save(bartSalvar->getText());
                     break;
                  }
                  case 5:
                  {
                     p5->Save(bartSalvar->getText());
                     break;
                  }
                  case 6:
                  {
                     p6->Save(bartSalvar->getText());
                     break;
                  }
                  case 7:
                  {
                     p7->Save(bartSalvar->getText());
                     break;
                  }
               }
               printf("With a lot of hope, i've saved as: %s\n",
                      bartSalvar->getText().c_str());
            }
         }

         /* Draw */
  
         glClearColor(0,0,0,0);
         glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

         glLoadIdentity();
         /* Redefine a posicao dinamica da camera */
         gameCamera->lookAt(NULL);
         updateFrustum(matrizVisivel,proj,modl);
 
         
         /* Part Draw */
         glPushMatrix();
           actualizeParticle(segundos);
         glPopMatrix();

         glColor3f(1.0,1.0,1.0);
         glDisable(GL_LIGHTING);
         glDisable(GL_DEPTH_TEST);
         glDisable(GL_BLEND);
          
         draw2DMode(); 
            gui->draw(proj,modl,viewPort);
         draw3DMode(FARVIEW);
            
         //glPopMatrix();

         //glEnable(GL_LIGHTING);
         glEnable(GL_DEPTH_TEST);
         glFlush();
         SDL_GL_SwapBuffers();
      }

   }

   delete(gameCamera);
   delete(gui);

   deleteParticle(); 

}

