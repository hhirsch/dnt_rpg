/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "fntEditor.h"

/*************************************************************
 *                          clear                            *
 *************************************************************/
void fntEditor::clear()
{
  int aux,aux2;
  for (aux=0;aux<=256;aux++)
     for (aux2=0;aux2<16;aux2++)
          actualFont.letra[aux][aux2] = 0;
  for(aux = 0;aux<16;aux++)
    for(aux2=0;aux2<16;aux2++)
       character[aux][aux2] = 0;
}

/*************************************************************
 *                        putLetter                          *
 *************************************************************/
void fntEditor::putLetter()
{
   int aux,aux2;
   int sobra;
   if(actualCharacter != -1)
   {
      for(aux=0;aux<16;aux++)
      {
         sobra = 0;
         for (aux2=15;aux2>=0;aux2--)
         {
            if (character[aux][aux2])
            {
               if(aux2 > 0)
               {
                  sobra += (2<<(aux2-1)); // 2^aux2
               }
               else
               {
                  sobra += 1;
               }
            }
         }
         actualFont.letra[actualCharacter][aux] = sobra;
      }
   }
}

/*************************************************************
 *                        getLetter                          *
 *************************************************************/
void fntEditor::getLetter(int n)
{
   SDL_Surface* surf = grid->get();
   if(n == actualCharacter) return;
   int sobra;
   int aux,aux2;
   for (aux=0;aux<16;aux++)
   {
      sobra = actualFont.letra[n][aux];
      for (aux2=0;aux2<16;aux2++)
      {
          character[aux][aux2] = sobra % 2;
          sobra = sobra / 2;
          if(character[aux][aux2])
             color_Set(255,255,255);
          else
             color_Set(0,0,0);
          rectangle_Fill( surf,((aux)*14)+1,((aux2)*14)+1,
                          ((aux+1)*14)-1,((aux2+1)*14)-1);
      }
   }
   grid->set(surf);
   actualCharacter = n;
}

/*************************************************************
 *                          defCP                            *
 *************************************************************/
void fntEditor::defCP()
{
   int aux;
   actualFont.incCP = 0;
   for(aux=0;textCP->getText()[aux]!='\0';aux++)
   {
      actualFont.incCP += textCP->getText()[aux] - '0';
   }
}
/*************************************************************
 *                      copyCharacter                        *
 *************************************************************/
void fntEditor::copyCharacter(int cFrom, int cTo)
{
   int aux;
   for(aux=0;aux<16;aux++)
   {
      actualFont.letra[cTo][aux] = actualFont.letra[cFrom][aux];
   }

   if(actualCharacter == cTo)
   {
      actualCharacter = -1;
      getLetter(cTo);
      editWindow->draw(0,0);
   }
}

/*************************************************************
 *                           open                            *
 *************************************************************/
void fntEditor::open()
{
  char tmp[4];
  FILE *arq;
  if ( !((arq) = fopen (textFileName->getText().c_str(), "rb")))
  { 
     printf("Can't Open: %s\n",textFileName->getText().c_str());
  }
  fread(&actualFont,sizeof(fnt),1,arq);
  fclose(arq);
  sprintf(tmp,"%d",actualFont.incCP);
  textCP->setText(tmp);
  infoWindow->draw(0,0);
  actualCharacter = -1;
}

/*************************************************************
 *                           save                            *
 *************************************************************/
void fntEditor::save()
{
   putLetter();
   FILE *arq;
   if( !((arq) = fopen (textFileName->getText().c_str(), "wb")))
   {
       printf("Can't Save: %s\n",textFileName->getText().c_str());
   }
   fwrite(&actualFont,sizeof(fnt),1,arq);
   fclose(arq);
   printf("File Saved.\n");
}

/*************************************************************
 *                        putColor                           *
 *************************************************************/
void fntEditor::putColor(int x, int y)
{
   SDL_Surface* surf = grid->get();
   x -= editWindow->getX1();
   y -= editWindow->getY1();
   int nx = ((x) / 14) ;
   int ny = ((y) / 14) ;
   if (nx>0 && nx<=16 && ny>0 && ny<=16)
   {
      if(actualColor)
         color_Set(255,255,255);
      else
         color_Set(0,0,0);
      rectangle_Fill( surf,((nx-1)*14)+1,((ny-1)*14)+1,
                         ((nx)*14)-1,((ny)*14)-1);
      character[nx-1][ny-1] = actualColor;
      grid->set(surf);
      editWindow->draw(0,0);
   }
}

/*************************************************************
 *                        drawGrid                           *
 *************************************************************/
void fntEditor::drawGrid()
{
    SDL_Surface* surf = grid->get();
    color_Set(0,0,0);
    rectangle_Fill(surf,0,0,224,224);
    int y = 0;
    int x = 0;
    color_Set(color.colorCont[1].R,
              color.colorCont[1].G,
              color.colorCont[1].B);
    int aux;
    for(aux=0;aux<17;aux++)
    {
       line_Draw(surf,0,y,224,y);
       line_Draw(surf,x,0,x,224);
       y+=14;
       x+=14;
    }
    int aux2;
    for(aux=0;aux<16;aux++)
    {
        for(aux2=0;aux2<16;aux2++)
        {
           if(character[aux][aux2])
           { 
               color_Set(255,255,255);
               rectangle_Fill( surf,((aux)*14)+1,
                                  ((aux2)*14)+1,
                                  ((aux+1)*14)-1,((aux2+1)*14)-1);
           }
        }
    }
    grid->set(surf);
    editWindow->draw(0,0);
}

/*************************************************************
 *                     defineCharacter                       *
 *************************************************************/
void fntEditor::defineCharacter(Uint8* keys, char* c)
{
   c[0] = ' ';

   int maiuscula = (keys[SDLK_CAPSLOCK] || keys[SDLK_LSHIFT] || 
                    keys[SDLK_RSHIFT]);

   if( keys[SDLK_KP_DIVIDE])
      {
          if(maiuscula)
            c[0] = '?';
          else 
            c[0] = '/';
      }
      else 
      if( keys[SDLK_a])
      {
          if (maiuscula)
            c[0] = 'A';
          else
            c[0] = 'a';
      } 
      else 
      if( keys[SDLK_b])
      {
          if (maiuscula)
            c[0] = 'B';
          else
            c[0] = 'b';
      }
      else 
      if( keys[SDLK_c])
      {
          if (maiuscula)
            c[0] = 'C';
          else
            c[0] = 'c';
      } 
      else 
      if( keys[SDLK_d])
      {
          if (maiuscula)
            c[0] = 'D';
          else
            c[0] = 'd';
      }
      else 
      if( keys[SDLK_e])
      {
          if (maiuscula)
            c[0] = 'E';
          else
            c[0] = 'e';
      }
      else 
      if( keys[SDLK_f])
      {
          if (maiuscula)
            c[0] = 'F';
          else
            c[0] = 'f';
      }
      else 
      if( keys[SDLK_g])
      {
          if (maiuscula)
            c[0] = 'G';
          else
            c[0] = 'g';
      }
      else 
      if( keys[SDLK_h])
      {
          if (maiuscula)
            c[0] = 'H';
          else
            c[0] = 'h';
      }
      else 
      if( keys[SDLK_i])
      {
          if (maiuscula)
            c[0] = 'I';
          else
            c[0] = 'i';
      }
      else 
      if( keys[SDLK_j])
      {
          if (maiuscula)
            c[0] = 'J';
          else
            c[0] = 'j';
      }
      else 
      if( keys[SDLK_k])
      {
          if (maiuscula)
            c[0] = 'K';
          else
            c[0] = 'k';
      }
      else 
      if( keys[SDLK_l])
      {
          if (maiuscula)
            c[0] = 'L';
          else
            c[0] = 'l';
      }
      else 
      if( keys[SDLK_m])
      {
          if (maiuscula)
            c[0] = 'M';
          else
            c[0] = 'm';
      }
      else 
      if( keys[SDLK_n])
      {
          if (maiuscula)
            c[0] = 'N';
          else
            c[0] = 'n';
      }
      else 
      if( keys[SDLK_o])
      {
          if (maiuscula)
            c[0] = 'O';
          else
            c[0] = 'o';
      }
      else 
      if( keys[SDLK_p])
      {
          if (maiuscula)
            c[0] = 'P';
          else
            c[0] = 'p';
      }
      else 
      if( keys[SDLK_q])
      {
          if (maiuscula)
            c[0] = 'Q';
          else
            c[0] = 'q';
      }
      else 
      if( keys[SDLK_r])
      {
          if (maiuscula)
            c[0] = 'R';
          else
            c[0] = 'r';
      }
      else 
      if( keys[SDLK_s])
      {
          if (maiuscula)
            c[0] = 'S';
          else
            c[0] = 's';
      }
      else 
      if( keys[SDLK_t])
      {
          if (maiuscula)
            c[0] = 'T';
          else
            c[0] = 't';
      }
      else 
      if( keys[SDLK_u])
      {
          if (maiuscula)
            c[0] = 'U';
          else
            c[0] = 'u';
      }
      else 
      if( keys[SDLK_w])
      {
          if (maiuscula)
            c[0] = 'W';
          else
            c[0] = 'w';
      }
      else 
      if( keys[SDLK_x])
      {
          if (maiuscula)
            c[0] = 'X';
          else
            c[0] = 'x';
      }
      else 
      if( keys[SDLK_y])
      {
          if (maiuscula)
            c[0] = 'Y';
          else
            c[0] = 'y';
      }
      else 
      if( keys[SDLK_z])
      {
          if (maiuscula)
            c[0] = 'Z';
          else
            c[0] = 'z';
      }
      else 
      if( keys[SDLK_SLASH])
      {
          if (maiuscula)
            c[0] = '?';
          else
            c[0] = '/';
      }
      else 
      if( keys[SDLK_BACKSLASH])
      {
          if (maiuscula)
            c[0] = '|';
          else
            c[0] = '\\';
      }
      else 
      if( keys[SDLK_SEMICOLON])
      {
          if (maiuscula)
            c[0] = ':';
          else
            c[0] = ';';
      }
      else 
      if( keys[SDLK_0])
      {
          if (maiuscula)
            c[0] = ')';
          else
            c[0] = '0';
      }
      else 
      if( keys[SDLK_1])
      {
          if (maiuscula)
            c[0] = '!';
          else
            c[0] = '1';
      }
      else 
      if( keys[SDLK_2])
      {
          if (maiuscula)
            c[0] = '@';
          else
            c[0] = '2';
      }
      else 
      if( keys[SDLK_3])
      {
          if (maiuscula)
            c[0] = '#';
          else
            c[0] = '3';
      }
      else 
      if( keys[SDLK_4])
      {
          if (maiuscula)
            c[0] = '$';
          else
            c[0] = '4';
      }
      else 
      if( keys[SDLK_5])
      {
          if (maiuscula)
            c[0] = '%';
          else
            c[0] = '5';
      }
      else 
      if( keys[SDLK_6])
      {
          if (maiuscula)
            c[0] = '"';
          else
            c[0] = '6';
      }
      else 
      if( keys[SDLK_7])
      {
          if (maiuscula)
            c[0] = '&';
          else
            c[0] = '7';
      }
      else 
      if( keys[SDLK_8])
      {
          if (maiuscula)
            c[0] = '*';
          else
            c[0] = '8';
      }
      else 
      if( keys[SDLK_9])
      {
          if (maiuscula)
            c[0] = '(';
          else
            c[0] = '9';
      }
      else 
      if( keys[SDLK_BACKQUOTE])
      {
          if (maiuscula)
            c[0] = '"';
          else
            c[0] = '\'';
      }
      else 
      if( keys[SDLK_MINUS])
      {
          if (maiuscula)
            c[0] = '_';
          else
            c[0] = '-';
      }
      else 
      if( keys[SDLK_PERIOD])
      {
          if (maiuscula)
            c[0] = '>';
          else
            c[0] = '.';
      }
      else 
      if( keys[SDLK_COMMA])
      {
          if (maiuscula)
            c[0] = '<';
          else
            c[0] = ',';
      }
      else 
      if( keys[SDLK_SPACE])
      {
          if (maiuscula)
            c[0] = ' ';
          else
            c[0] = ' ';
      }
}

/*************************************************************
 *                       Constructor                         *
 *************************************************************/
fntEditor::fntEditor()
{
   int aux;

   /* Clear Values */
   for(aux = 0;aux<16;aux++)
   {
       character[aux][aux] = 0;
   }
   actualColor = 1;
   color_Alpha(255);
   actualFont.incCP = 1;
   done = false;
   actualCharacter = 0;

   /* Init the system */
   Farso_Init(&screen,"DNT FNT Editor");
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei)screen->w / (GLsizei)screen->h, 1.0, 512);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glGetDoublev(GL_MODELVIEW_MATRIX, proj);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   /* Create the GUI */
   interf = new interface(NULL);

   /* Create the Main Window */
   mainWindow = interf->insertWindow(0,1,230,68,"fntEditor");
   textFileName = mainWindow->getObjectsList()->insertTextBar(9,20,221,34,
                                                              "../data/fnt/",0);
    newButton = mainWindow->getObjectsList()->insertButton(13,42,63,60,
                                                           "New",1);
    openButton = mainWindow->getObjectsList()->insertButton(64,42,114,60,
                                                            "Load",1);
    saveButton = mainWindow->getObjectsList()->insertButton(115,42,165,60,
                                                            "Save",1);
    exitButton = mainWindow->getObjectsList()->insertButton(167,42,217,60,
                                                            "Exit",1);
    mainWindow->setAttributes(false, true, true, true);
    mainWindow->setExternPointer(&mainWindow);
    interf->openWindow(mainWindow);

    /* Info Window */
    infoWindow = interf->insertWindow(0,69,230,205,"Attributes");
    infoWindow->getObjectsList()->insertTextBox(9,21,46,34,0,"INCP:");
    textCP = infoWindow->getObjectsList()->insertTextBar(45,20,70,34,"1",0);
    infoWindow->getObjectsList()->insertTextBox(5, 17 ,225, 37, 1, "");

    activeButton = infoWindow->getObjectsList()->insertButton(28,44,108,62,
                                                              "Active",1);
    inactiveButton = infoWindow->getObjectsList()->insertButton(111,44,200,62,
                                                                "Inactive",1);
    infoWindow->getObjectsList()->insertTextBox(5, 38 ,225, 68, 1, "");
    
    infoWindow->getObjectsList()->insertTextBox(9,78,79,91,0,
                                                "Character:");
    textGoto = infoWindow->getObjectsList()->insertTextBar(80,77,110,91,
                                                           "233",0);
    gotoButton = infoWindow->getObjectsList()->insertButton(113,75,176,93,
                                                            "GoTo",1);
    infoWindow->getObjectsList()->insertTextBox(5, 69 ,225, 99, 1, "");

    infoWindow->getObjectsList()->insertTextBox(9,109,45,121,0,"From:");
    textFrom = infoWindow->getObjectsList()->insertTextBar(45,108,75,122,
                                                           "1",0);
    infoWindow->getObjectsList()->insertTextBox(76,109,106,121,0,"To:");
    textTo = infoWindow->getObjectsList()->insertTextBar(107,108,137,122,
                                                         "2",0);
    copyButton = infoWindow->getObjectsList()->insertButton(150,106,215,124,
                                                            "Copy",1);
    infoWindow->getObjectsList()->insertTextBox(5, 100 ,225, 130, 1, "");

    infoWindow->setAttributes(false, true, true, true);
    infoWindow->setExternPointer(&infoWindow);
    interf->openWindow(infoWindow);

    /* Create the Edit Window */
    editWindow = interf->insertWindow(0,206,255,461, "Caractere");
    grid = editWindow->getObjectsList()->insertPicture(8,18,256,256,NULL);
    drawGrid();
    clear();
    editWindow->setExternPointer(&editWindow);
    editWindow->setAttributes(false, true, true, true);
    interf->openWindow(editWindow);
}

/*************************************************************
 *                        Destructor                         *
 *************************************************************/
fntEditor::~fntEditor()
{
   delete(interf);
}

/*************************************************************
 *                            run                            *
 *************************************************************/
void fntEditor::run()
{
   Uint8 *keys;
   int x,y;
   guiObject* object = NULL;
   int event;

   char c[1];
   lastLetter = 0;

   /* Get the matrix */
   actualizeFrustum(visibleMatrix,proj,modl);
  
   /* Run the Editor */
   while (!done)
   {
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      glClearColor(0,0,0,1);
      glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
      Uint8 Mbotao = SDL_GetMouseState(&x,&y);
      object = interf->manipulateEvents(x,y,Mbotao, keys, &event);

      if(event == PRESSED_BUTTON)
      {
         /* New Button */
         if(object == (guiObject*)newButton)
         {
            clear();
            actualCharacter = -1;
         }
         /* Save Button */
         else if(object == (guiObject*)saveButton)
         {
            save();
         }
         /* Open Button */
         else if(object == (guiObject*)openButton)
         {
            open();
         }
         /* Exit Button */
         else if(object == (guiObject*)exitButton)
         {
            done = true;
         }
         /* Active Color Button */
         else if(object == (guiObject*)activeButton)
         {
            actualColor = 1;
         }
         /* Inactive Color Button */
         else if(object == (guiObject*)inactiveButton)
         {
            actualColor = 0;
         }
         /* goTo Button */
         else if(object == (guiObject*)gotoButton)
         {
            int cDesejado =  atoi(textGoto->getText().c_str());
            printf("%c %d\n",cDesejado,cDesejado);
            putLetter();
            getLetter(cDesejado);
            lastLetter = cDesejado;
            editWindow->draw(0,0);
         }
         /* copy Button */
         else if(object == (guiObject*)copyButton)
         {
            int cFrom = atoi(textFrom->getText().c_str());
            int cTo = atoi(textTo->getText().c_str());
            copyCharacter(cFrom, cTo);
         }
      }
      else if(event == WROTE_TEXT_BAR)
      {
         /* Edited the textCP */
         if(object == (guiObject *)textCP)
         {
            defCP();
         }
      }
      else if(event == CLICKED_WINDOW)
      {
         /* Edit the grid */
         if(object == (guiObject *)editWindow)
         {
            putColor(x, y);
         }
      }
      else
      {
         /* Define the new editable character */
         defineCharacter(keys,c);
         if( (c[0] != lastLetter) && (c[0] != ' '))
         {
            printf("%c %d\n",c[0],c[0]);
            putLetter();
            getLetter(c[0]);
            editWindow->draw(0,0);
            lastLetter = c[0];
         }
      }
       
      /* Draw to the window */
      glPushMatrix();
        draw2DMode();
           interf->draw(proj,modl,viewPort);
        draw3DMode(45);
      glPopMatrix();
      glFlush();
      SDL_GL_SwapBuffers();
      SDL_Delay(50);
   }
}

/*************************************************************
 *                           main                            *
 *************************************************************/
int main(int argc, char *argv[])
{
   fntEditor* f = new fntEditor();
   f->run();
   delete(f);
   return(0);
}

