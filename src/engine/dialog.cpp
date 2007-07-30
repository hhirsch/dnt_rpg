/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "dialog.h"
#include "dccnit.h"

#define BUFFER_SIZE 512

/* Geral Tokens */
#define TK_TRUE "true"
#define TK_FALSE "false"
#define TK_DIALOG "dialog"
#define TK_END_DIALOG "end_dialog"
#define TK_NPC_BEGIN "npc_begin"
#define TK_NPC_END "npc_end"
#define TK_PC_BEGIN "pc_begin"
#define TK_PC_END "pc_end"
#define TK_TEXT "text"
#define TK_ACTION "action"
#define TK_OPTION "option"

/* Action Tokens */
#define TK_ACTION_GO_TO_DIALOG "go_to_dialog"
#define TK_ACTION_INIT_FIGHT "init_fight"
#define TK_ACTION_FINISH_DIALOG "finish_dialog"

//TODO If-else tokens
#define TK_IF_CONDITION "if_condition"
#define TK_ELSE_CONDITION "else_condition"

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
conversation::conversation(void* pEngine)
{
   first = new(dialog);
   first->next = first;
   first->previous = first;
   first->id = 0;
   total = 0;
   npcText = NULL;
   pcSelText = NULL;
   jan = NULL;
   usedGui = NULL;
   actualPC = NULL;
   actualNPC = NULL;
   actual = -1;
   actualEngine = pEngine;
}

/*************************************************************************
 *                               Destructor                              *
 *************************************************************************/
conversation::~conversation()
{
   if( (jan) && (usedGui) )
   {
      usedGui->closeWindow(jan);
   }
   dialog* dlg;
   while(total>0)
   {
       dlg = first->next;
       removeDialog(dlg->id);
   }
   delete(first);
   total = -1;
}

/*************************************************************************
 *                              getString                                *
 *************************************************************************/
string conversation::getString(int& initialPosition, string buffer,
                               char& separator)
{
   int i = initialPosition;
   string ret = "";
   bool endGet = false;
   bool considerSpace = false;
   while(!endGet)
   {
      if( (i >= (int)buffer.length()-1) )
      {
         endGet = true;
         separator = '\0';
      }
      if( (!considerSpace) && 
          ( (buffer[i] == '\0') || (buffer[i] == '\n') || 
            (buffer[i] == '(') || (buffer[i] == ')') || 
            (buffer[i] == ',') || (buffer[i] == '=') ) )
      {
         separator = buffer[i];
         endGet = true;
      }
      else if(buffer[i] == '"')
      {
         considerSpace = true;
      }
      else if( ( ( buffer[i] != ' ') || considerSpace) && (buffer[i] != '"') )
      {
         ret += buffer[i];
      }
      i++;
   }
   initialPosition = (i);

   return(ret);
}

/*************************************************************************
 *                             printError                                *
 *************************************************************************/
void conversation::printError(string fileName, string error, int lineNumber)
{
   printf("Conversation: %s Error on line: %d\n", fileName.c_str(), lineNumber);
   printf("\t%s\n",error.c_str());
}

/*************************************************************************
 *                             getActionID                               *
 *************************************************************************/
int conversation::getActionID(string token, string fileName, int line)
{
   
   if(token == TK_ACTION_GO_TO_DIALOG)
   {
     return(TALK_ACTION_GOTO);
   }
   else if(token == TK_ACTION_INIT_FIGHT)
   {
      return(TALK_ACTION_FIGHT);
   }
   else if(token == TK_ACTION_FINISH_DIALOG)
   {
      return(TALK_ACTION_CLOSE);
   }
   printError(fileName, "Unknow action!", line);
   return(-1);
}

/*************************************************************************
 *                               loadFile                                *
 *************************************************************************/
int conversation::loadFile(string name)
{
  lang language;
  string buffer;
  string stmp;
  dialog* dlg = NULL;
  int position = 0;
  string token;
  char separator;
  bool endDialog = true;
  int line = 0;
  int option = -1;

  bool npcBegin = false;
  bool pcBegin = false;

  name = language.DIALOG_DIR + name;

  std::ifstream file;
  
  file.open(name.c_str(), ios::in | ios::binary);
  if(!file)
  {
     printError(name, "Cannot open File!\n", 0);
     return(0);
  }


  while(getline(file, buffer) != NULL)
  {
    line++;
    position = 0;
    separator = '\0';

    /* Ignore comentaries */
    if(buffer[0] != '#')
    {
      token = getString(position, buffer, separator);

      
      /* Create New Dialog */
      if(token == TK_DIALOG)
      {
         if(!endDialog)
         {
            printError(name,"Tried to define dialog before end last one!",line);
         }
         else
         {
            endDialog = false;
            dlg = insertDialog();
            token = getString(position, buffer, separator);
            dlg->id = atoi(token.c_str());
         }
      }
      /* End defined Dialog */
      else if(token == TK_END_DIALOG)
      {
         if( (endDialog) || (dlg == NULL))
         {
            printError(name, "Tried to end an undefined dialog!", line);
         }

         endDialog = true;
         dlg = NULL;
      }
      /* npc */
      else if(token == TK_NPC_BEGIN)
      {
         if(npcBegin || pcBegin)
         {
            printError(name, "Tried to begin NPC before end something!", line);
         }
         npcBegin = true;
      }
      else if(token == TK_NPC_END)
      {
         if(!npcBegin)
         {
            printError(name, "Tried to end NPC before begin it!", line);
         }
         npcBegin = false;
      }
      /* pc */
      else if(token == TK_PC_BEGIN)
      {
         if(npcBegin || pcBegin)
         {
            printError(name, "Tried to begin PC before end something!", line);
         }
         pcBegin = true;
         option = -1;
      }
      else if(token == TK_PC_END)
      {
         if(!pcBegin)
         {
            printError(name, "Tried to end PC before begin it!", line);
         }
         pcBegin = false;
      }
      /* Text */
      else if(token == TK_TEXT)
      {
         if(npcBegin)
         {
            dlg->npc.ifText = getString(position, buffer, separator);
         }
         else if(pcBegin)
         {
            if( (option >= 0) && (option < MAX_OPTIONS))
            {
               char str[10];
               sprintf(str, "%d - ", option+1);
               dlg->options[option].ifText = str;
               dlg->options[option].ifText += getString(position, buffer, 
                                                       separator);
            }
            else
            {
               if(option < 0)
               {
                  printError(name, "Option Text without option!", line);
               }
               else
               {
                  printError(name, "Options overflow!", line);
               }
            }
         }
         else
         {
            printError(name, "Text without any relation!", line);
         }
      }
      /* Option */
      else if(token == TK_OPTION)
      {
         if(pcBegin)
         {
            option++;
         }
         else
         {
            printError(name, "Option without begin PC!", line);
         }
      }
      /* Action */
      else if(token == TK_ACTION)
      {
         if(npcBegin)
         {
            printError(name, "Action isn't yet defined to NPC!", line);
         }
         else if(pcBegin)
         {
            if( (option >= 0) && (option < MAX_OPTIONS))
            {
               token = getString(position, buffer, separator);
               dlg->options[option].ifAction.id = getActionID(token,name,line);
               if(dlg->options[option].ifAction.id == TALK_ACTION_GOTO)
               {
                  //get dialog number
                  token = getString(position, buffer, separator);
                  dlg->options[option].ifAction.att = atoi(token.c_str());
               }
            }
            else
            {
               if(option < 0)
               {
                  printError(name, "Option Action without option!", line);
               }
               else
               {
                  printError(name, "Options overflow!", line);
               }
            }
         }
         else
         {
            printError(name, "Action without any relation!\n", line);
         }
      }
      /* Unkown! */
      else
      {
         printError(name, "Unknow Token!", line);
      }
    }
  }

  file.close();
  return(1);
}

/*************************************************************************
 *                                saveFile                               *
 *************************************************************************/
int conversation::saveFile(string name)
{
   //TODO or not used.
   return(1);
}

/*************************************************************************
 *                             insertDialog                              *
 *************************************************************************/
dialog* conversation::insertDialog()
{
   dialog* dlg = new (dialog);
   dlg->next = first->next;
   dlg->previous = first;
   dlg->next->previous = dlg;
   dlg->id = dlg->next->id+1;
   first->next = dlg;

   dlg->npc.ifText = "";
   dlg->npc.attribute = -1;
   dlg->npc.elseText = "";
   dlg->npc.operation = -1;
   dlg->npc.ifAction.id = -1;
   dlg->npc.elseAction.id = -1;

   int aux;
   for(aux = 0; aux< 5; aux++)
   {
      dlg->options[aux].ifText = "";
      dlg->options[aux].attribute = -1;
      dlg->options[aux].elseText = "";
      dlg->options[aux].operation = -1;
      dlg->options[aux].ifAction.id = -1;
      dlg->options[aux].elseAction.id = -1;
   }

   total++;
   return(dlg);
}

/*************************************************************************
 *                             removeDialog                              *
 *************************************************************************/
void conversation::removeDialog(int num)
{
   dialog *dlg = first->next;
   while((dlg != first) && (dlg->id != num))
   {
       dlg = dlg->next;
   }
   if(dlg == first)
   {
      printf("Not found on dialog %d\n",num);
   }
   else
   {
      dlg->next->previous = dlg->previous;
      dlg->previous->next = dlg->next;
      total--;
      delete(dlg);
   }
}

/*************************************************************************
 *                              openDialog                               *
 *************************************************************************/
void conversation::openDialog(int numDialog, interface* gui, character* pers,
                              character* PC)
{
   lang language;
   usedGui = gui;
   actualNPC = pers;
   actualPC = PC;
   actual = -1;
   jan = gui->insertWindow(330,100,585,355,language.DIALOGW_TITLE.c_str());
   jan->getObjectsList()->insertButton(5,86,69,104,"Barter",1);
   jan->getObjectsList()->insertPicture(5,25,0,0,
                                        pers->getPortraitFileName().c_str());
   npcText = jan->getObjectsList()->insertRolBar(71,20,250,115,"",
                                                 jan->getSurface());
   //npcText->fonte = FMINI;
   pcSelText = jan->getObjectsList()->insertSelText(5,116,250,250,"","","",
                                                    "","");
   jan->getObjectsList()->insertPicture(3,15,0,0,
                                        "../data/texturas/dialogw/dialog.png");
   jan->setExternPointer(&jan);
   gui->openWindow(jan);

   changeDialog(numDialog);
}

/*************************************************************************
 *                            proccessAction                             *
 *************************************************************************/
void conversation::proccessAction(int numDialog, int opcao, interface* gui)
{
   dialog* dlg = first->next;
   while( (dlg != first) && (dlg->id != numDialog))
   {
      dlg = dlg->next;
   }
   if(dlg == first)
   {
      return;
   }

   //FIXME else too
   int action  = dlg->options[opcao].ifAction.id;

   switch(action)
   {
      case TALK_ACTION_GOTO:
         /* change dialog */
         changeDialog(dlg->options[opcao].ifAction.att);
      break;
      case TALK_ACTION_FIGHT:
      {
         engine* eng = (engine*)actualEngine;
         actualNPC->setAsEnemy();
         eng->enterBattleMode(false);
         gui->closeWindow(jan);
         jan = NULL;
         usedGui = NULL;
      }
      break;
      case TALK_ACTION_CLOSE:
         gui->closeWindow(jan);
         jan = NULL;
         usedGui = NULL;
      break;
      case TALK_ACTION_MODPC:
         //TODO
      break;
      case TALK_ACTION_MODNPC:
         //TODO
      break;
   }
}

/*************************************************************************
 *                            changeDialog                               *
 *************************************************************************/
void conversation::changeDialog(int numDialog)
{
   int i;

   if(numDialog == actual)
   {
      /* No change at the same dialog! */
      return;
   }

   dialog* dlg = first->next;
   while( (dlg != first) && (dlg->id != numDialog))
   {
      dlg = dlg->next;
   }
   if(dlg == first)
   {
      return;
   }

   actual = numDialog;

   string npc;
   string options[5];
   int aux;

//TODO verify if/else 

   npc = dlg->npc.ifText;
   for(aux = 0; aux<5; aux++)
   {
      options[aux] = dlg->options[aux].ifText; 
   }
 
   npcText->setText(npc);
   for(i = 0; i < MAX_OPTIONS; i++)
   {
      pcSelText->setText(i,options[i]);
   }

   if(windowOpened())
   {
      jan->draw(0,0);
   }
}

/*************************************************************************
 *                                treat                                  *
 *************************************************************************/
bool conversation::treat(guiObject* guiObj, int eventInfo, interface* gui)
{
   if(eventInfo == SELECTED_SEL_TEXT)
   {
      if(guiObj == (guiObject*)pcSelText)
      {
         proccessAction(actual, pcSelText->getLastSelectedItem(), gui);
         return(true);
      }
   }
   return(false);
}

/*************************************************************************
 *                            windowOpened                               *
 *************************************************************************/
bool conversation::windowOpened()
{
   return(jan != NULL);
}

