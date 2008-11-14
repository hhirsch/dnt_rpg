/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "dialog.h"
#include "dccnit.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"
#include "../classes/mission.h"
#include "briefing.h"
#include "barterwindow.h"
#include "modstate.h"

/* The Functions */
#define TALK_ACTION_GO_TO_DIALOG         0 /* Go To some conversation point */
#define TALK_ACTION_INIT_FIGHT           1 /* End talk and initiate a fight */
#define TALK_ACTION_FINISH_DIALOG        2 /* End Talk */
#define TALK_ACTION_MOD_PC               3 /* Modify PC attribute */
#define TALK_ACTION_MOD_NPC              4 /* Modify NPC attribute */
#define TALK_ACTION_DIALOG_INIT          5 /* Set the new initial dialog */
#define TALK_ACTION_ADD_MISSION          6 /* Add a mission */
#define TALK_ACTION_COMPLETE_MISSION     7 /* Complete Mission */
#define TALK_ACTION_GIVE_ITEM            8 /* Give an item */
#define TALK_ACTION_RECEIVE_MONEY        9 /* Receive some money */
#define TALK_ACTION_CHANGE_OBJECT_STATE 10 /* Change Object State */
#define TALK_ACTION_RECEIVE_XP          11 /* Receive XP ammount */
#define TALK_ACTION_KILL_ALL            12 /* Kill All NPCs from a map */
#define TALK_ACTION_RECEIVE_ITEM        13 /* Receive an Item */

#define TALK_TEST_TRUE                0  /* Always True */
#define TALK_TEST_ROLL                1  /* Roll some test */
#define TALK_TEST_GREATER             2  /* Test if is greater */
#define TALK_TEST_LESSER              3  /* Test if is lesser */
#define TALK_TEST_EQUAL               4  /* Test if is equal */
#define TALK_TEST_DIFF                5  /* Test if is diff */
#define TALK_TEST_HAVE_ITEM           6  /* Test if have item*/
#define TALK_TEST_ALIGN_NOT           7  /* Test if align not of type */
#define TALK_TEST_ALIGN               8  /* Test if align is of a type */
#define TALK_TEST_ALL_ALIVE           9  /* If all characters are alive */
#define TALK_TEST_ALL_DEAD           10  /* If all characters are dead */

#define BUFFER_SIZE 512

/* General Tokens */
#define TK_TRUE "true"
#define TK_FALSE "false"
#define TK_DIALOG "dialog"
#define TK_END_DIALOG "end_dialog"
#define TK_NPC_BEGIN "npc_begin"
#define TK_NPC_END "npc_end"
#define TK_PC_BEGIN "pc_begin"
#define TK_PC_END "pc_end"
#define TK_PRE_TEST "pre_test"
#define TK_POST_TEST "post_test"
#define TK_TEXT "text"
#define TK_ACTION "action"
#define TK_IF_ACTION "if_action"
#define TK_ELSE_ACTION "else_action"
#define TK_OPTION "option"

/* Action Tokens */
#define TK_ACTION_GO_TO_DIALOG "go_to_dialog"
#define TK_ACTION_INIT_FIGHT "init_fight"
#define TK_ACTION_FINISH_DIALOG "finish_dialog"
#define TK_ACTION_DIALOG_INIT "dialog_init"
#define TK_ACTION_ADD_MISSION "add_mission"
#define TK_ACTION_COMPLETE_MISSION "complete_mission"
#define TK_ACTION_GIVE_ITEM "give_item"
#define TK_ACTION_RECEIVE_MONEY "receive_money"
#define TK_ACTION_CHANGE_OBJECT_STATE "change_object_state"
#define TK_ACTION_RECEIVE_XP "receive_xp"
#define TK_ACTION_KILL_ALL "kill_all"
#define TK_ACTION_RECEIVE_ITEM "receive_item"

/* Test Tokens */
#define TK_TEST_ROLL "roll"
#define TK_TEST_GREATER "greater"
#define TK_TEST_LESSER "lesser"
#define TK_TEST_EQUAL "equal"
#define TK_TEST_DIFF "diff"
#define TK_TEST_ALIGN "align"
#define TK_TEST_ALIGN_NOT "align_not"
#define TK_TEST_HAVE_ITEM "have_item"
#define TK_TEST_ALL_ALIVE "all_alive"
#define TK_TEST_ALL_DEAD "all_dead"

/* Constant Tokens */
#define TK_CONST_OBJECT_STATE "OBJECT_STATE"

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                              talkAction                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
talkAction::talkAction()
{
   id = -1;
   oper = -1; 
   qty = 0;
   att = -1;
   satt = "";
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                               talkTest                                //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
talkTest::talkTest()
{
   id = TALK_TEST_TRUE;
   test = "";
   against = "";
}

/*************************************************************************
 *                                  set                                  *
 *************************************************************************/
bool talkTest::set(string token, string t, string a)
{
   /* Set test and against */
   test = t;
   against = a;

   /* translate token string to id */

   /* roll */
   if(token == TK_TEST_ROLL)
   {
      id = TALK_TEST_ROLL;
   }
   /* greater */
   else if(token == TK_TEST_GREATER)
   {
      id = TALK_TEST_GREATER;
   }
   /* lesser */
   else if(token == TK_TEST_LESSER)
   {
      id = TALK_TEST_LESSER;
   }
   /* equal */
   else if(token == TK_TEST_EQUAL)
   {
      id = TALK_TEST_EQUAL;
   }
   /* diff */
   else if(token == TK_TEST_DIFF)
   {
      id = TALK_TEST_DIFF;
   }
   /* have_item */
   else if(token == TK_TEST_HAVE_ITEM)
   {
      id = TALK_TEST_HAVE_ITEM;
   }
   /* align_not */
   else if(token == TK_TEST_ALIGN_NOT)
   {
      id = TALK_TEST_ALIGN_NOT;
   }
   /* align */
   else if(token == TK_TEST_ALIGN)
   {
      id = TALK_TEST_ALIGN;
   }
   /* all_dead */
   else if(token == TK_TEST_ALL_DEAD)
   { 
      id = TALK_TEST_ALL_DEAD;
   }
   /* all_alive */
   else if(token == TK_TEST_ALL_ALIVE)
   {
      id = TALK_TEST_ALL_ALIVE;
   }
   else
   {
      /* Unknow test function! */
      return(false);
   }

   return(true);
}

/*************************************************************************
 *                              getTestName                              *
 *************************************************************************/
string talkTest::getTestName(character* pc)
{
   string res = "";

   /* Get the skill name */
   if(pc)
   {
      skill* sk = pc->sk.getSkillByString(test);
      if(sk)
      {
         res = "(" + sk->name + ") ";
      }
   }

   return(res);
}

/*************************************************************************
 *                                 doTest                                *
 *************************************************************************/
bool talkTest::doTest(character* pc, thing* owner)
{
   //FIXME - Must verify if the against is a number or a thing...
   //        for now, always is a number
   int value;

   /* Verify if we have a pc to check */
   if(!pc)
   {
      return(false);
   }

   /* true*/
   if(id == TALK_TEST_TRUE)
   {
      /* Always return true */
      return(true);
   }
   else if(id == TALK_TEST_ROLL)
   {
      /* Get the value */
      sscanf(against.c_str(), "%d", &value);

      /* Roll the thing! */
      skill* sk = pc->sk.getSkillByString(test);
      int rollValue = pc->sk.doSkillCheck(sk);
      bool res = (rollValue > value);

      /* Brief the result */
      char buffer[512];
      briefing brief;
      sprintf(&buffer[0], "%s: %d x %d: %s.",
              sk->name.c_str(), rollValue, value, 
              res?gettext("Success"):gettext("Failure"));
      if(res)
      {
         /* With blue color */
         brief.addText(buffer, 27, 169, 245);
      }
      else
      {
         /* With red color */
         brief.addText(buffer, 233, 0, 5);
      }
      
      /* Return what got */
      return(res);
   }

   /* Have Item test */
   else if(id == TALK_TEST_HAVE_ITEM)
   {
      return(pc->inventories->getItemByFileName(test) != NULL);  
   }

   /* Align Not Test */
   else if(id == TALK_TEST_ALIGN_NOT)
   {
      return(!pc->isAlignOf(test));
   }

   /* Align Test */
   else if(id == TALK_TEST_ALIGN)
   {
      return(pc->isAlignOf(test));
   }

   /* All Alive */
   else if(id == TALK_TEST_ALL_ALIVE)
   {
      modState modif;
      return(modif.allCharactersAlive(test));
   }

   /* All Dead */
   else if(id == TALK_TEST_ALL_DEAD)
   {
      modState modif;
      return(modif.allCharactersDead(test));
   }


   /* Comparasion tests (greater, equal, etc) */
   else
   {
      /* First the the compValue */
      int compValue = 0;

      /* Is comparing an owner state */
      if(test == TK_CONST_OBJECT_STATE)
      {
         object* obj = (object*)owner;
         compValue = obj->getState();
      }

      /* Must be comparing the active character skill or attribute */
      else
      {

         /* Get the skill or attribute to compare */
         skill* sk = pc->sk.getSkillByString(test);

         if(!sk)
         {
            cerr << "Unknow Attribute or Skill: " << test << endl;
            return(false);
         }

         compValue = sk->points;
      }

      /* Get the value to compare with */
      sscanf(against.c_str(), "%d", &value);

      /* Now, finally do the test */

      /* greater */
      if(id == TALK_TEST_GREATER)
      {
         return(compValue > value);
      }
      /* lesser */
      else if(id == TALK_TEST_LESSER)
      {
         return(compValue < value);
      }
      /* equal */
      else if(id == TALK_TEST_EQUAL)
      {
         return(compValue == value);
      }
      /* diff */
      else if(id == TALK_TEST_DIFF)
      {
         return(compValue != value);
      }
   }

   return(false);
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                             dialogOption                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
dialogOption::dialogOption()
{
   text = "";
   totalIfActions = 0;
   totalElseActions = 0;
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                             Conversation                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
conversation::conversation()
{
   first = new(dialog);
   first->next = first;
   first->previous = first;
   first->id = 0;
   total = 0;
   actualPC = NULL;
   owner = NULL;
   ownerMap = "";
   actual = -1;
   initialDialog = 0;
}

/*************************************************************************
 *                               Destructor                              *
 *************************************************************************/
conversation::~conversation()
{
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
   bool gnuGet = false;

   /* Remove spaces at begining */
   while( ( i < (int)buffer.length() ) && ( buffer[i] == ' ' ) )
   {
      i++;
   }

   /* Verify the gettext */
   if( (buffer.length() > 10) && (buffer[i] == 'g') &&
       (buffer[i+1] == 'e') && (buffer[i+2] == 't') && 
       (buffer[i+3] == 't') && (buffer[i+4] == 'e') &&
       (buffer[i+5] == 'x') && (buffer[i+6] == 't') )
   {
      gnuGet = true;
   }


   while(!endGet)
   {
      if( (i >= (int)buffer.length()-1) )
      {
         endGet = true;
         separator = '\0';
      }

      if(gnuGet)
      {
         /* It's an gettext("STRING") */

         /* Verify the begining */
         if(buffer[i] == '"')
         {
            considerSpace = !considerSpace;
         }

         /* Verify the end */
         if( (buffer[i] == ')') && (!considerSpace))
         {
            endGet = true;
            separator = buffer[i];
         }
         ret += buffer[i];
      }
      else
      {

         if( (!considerSpace) && 
             ( (buffer[i] == '\0') || (buffer[i] == '\n') || 
               (buffer[i] == '(') || (buffer[i] == ')') || 
               (buffer[i] == ',') || (buffer[i] == '=') ||
	   		   (buffer[i] == 13) ) )
         {
            separator = buffer[i];
            endGet = true;
         }
         else if(buffer[i] == '"')
         {
            /* Mark the init/end of the "string" */
            considerSpace = !considerSpace;
         }
         else if( ( ( buffer[i] != ' ') || considerSpace) && 
                  (buffer[i] != '"') )
         {
            ret += buffer[i];
         }
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
     return(TALK_ACTION_GO_TO_DIALOG);
   }
   else if(token == TK_ACTION_INIT_FIGHT)
   {
      return(TALK_ACTION_INIT_FIGHT);
   }
   else if(token == TK_ACTION_FINISH_DIALOG)
   {
      return(TALK_ACTION_FINISH_DIALOG);
   }
   else if(token == TK_ACTION_DIALOG_INIT)
   {
      return(TALK_ACTION_DIALOG_INIT);
   }
   else if(token == TK_ACTION_ADD_MISSION)
   {
      return(TALK_ACTION_ADD_MISSION);
   }
   else if(token == TK_ACTION_COMPLETE_MISSION)
   {
      return(TALK_ACTION_COMPLETE_MISSION);
   }
   else if(token == TK_ACTION_GIVE_ITEM)
   {
      return(TALK_ACTION_GIVE_ITEM);
   }
   else if(token == TK_ACTION_RECEIVE_ITEM)
   {
      return(TALK_ACTION_RECEIVE_ITEM);
   }
   else if(token == TK_ACTION_RECEIVE_MONEY)
   {
      return(TALK_ACTION_RECEIVE_MONEY);
   }
   else if(token == TK_ACTION_CHANGE_OBJECT_STATE)
   {
      return(TALK_ACTION_CHANGE_OBJECT_STATE);
   }
   else if(token == TK_ACTION_RECEIVE_XP)
   {
      return(TALK_ACTION_RECEIVE_XP);
   }
   else if(token == TK_ACTION_KILL_ALL)
   {
      return(TALK_ACTION_KILL_ALL);
   }

   printError(fileName, "Unknow action!", line);
   return(-1);
}

/*************************************************************************
 *                               loadFile                                *
 *************************************************************************/
int conversation::loadFile(string name)
{
  dirs dir;
  string buffer;
  string stmp;
  dialog* dlg = NULL;
  int position = 0;
  string token;
  char separator;
  bool endDialog = true;
  int line = 0;
  int option = -1;
  int preTest = 0;

  bool npcBegin = false;
  bool pcBegin = false;

  std::ifstream file;
 
  file.open(dir.getRealFile(name).c_str(), 
            ios::in | ios::binary);
  if(!file)
  {
     printError(dir.getRealFile(name), "Cannot open File!\n", 0);
     return(0);
  }


  while(getline(file, buffer) != NULL)
  {
    line++;
    position = 0;
    separator = '\0';

    /* Get the first token */
    token = getString(position, buffer, separator);

    /* Ignore comentaries and empty lines */
    if( (token[0] != '#') && (!token.empty()) )
    {
      
      /* Create New Dialog */
      if(token == TK_DIALOG)
      {
         if(!endDialog)
         {
            printError(name, "Tried to define dialog before end last one!",
                       line);
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
            dlg->npcText = translateDataString(getString(position, buffer, 
                                                         separator));
         }
         else if(pcBegin)
         {
            if(option >= 0)
            {
               dlg->options[option].text = translateDataString(
                                                   getString(position, buffer, 
                                                             separator));
            }
            else
            {
               printError(name, "Option Text without option!",line);
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
            preTest = 0;
            if(option+1 < MAX_OPTIONS)
            {
               option++;
            }
            else
            {
               printError(name, "Options overflow!", line);
            }
         }
         else
         {
            printError(name, "Option without begin PC!", line);
         }
      }
      /* PreTest and PostTest */
      else if((token == TK_PRE_TEST) || (token == TK_POST_TEST))
      {
         if(pcBegin)
         {
            if(option >= 0)
            {
               /* Get function(param,param) */
               string func = getString(position, buffer, separator); 
               string test = getString(position, buffer, separator);
               string against = getString(position, buffer, separator);

               bool res = true;

               /* Assign it to its test */
               if(token == TK_PRE_TEST)
               {
                  /* Pre-Test */
                  if(preTest < MAX_PRE_TESTS)
                  {
                     res = dlg->options[option].preTest[preTest].set(func, 
                                                                     test, 
                                                                     against);
                     preTest++;
                  }
                  else
                  {
                     printError(name, "Number of pre-tests overflow!", line);
                  }
               }
               else
               {
                  /* Post-Test */
                  res = dlg->options[option].postTest.set(func, test, against);
               }

               /* Verify test function parser error */
               if(!res)
               {
                  printError(name, "Unknow Test Function!", line);
               }
            }
            else
            {
               printError(name, "Test without option!",line);
            }
         }
         else
         {
            printError(name, "Option Test Without PC!", line);
         }
      }
      /* Action */
      else if( (token == TK_ACTION) || (token == TK_IF_ACTION) ||
               (token == TK_ELSE_ACTION) )
      {
         if(npcBegin)
         {
            printError(name, "Action isn't yet defined to NPC!", line);
         }
         else if(pcBegin)
         {
            if( (option >= 0) && (option < MAX_OPTIONS))
            {
               talkAction* tact = NULL;
               int curAction = 5;
              
               /* Get the current number of actions (if or else) */
               if(token == TK_ELSE_ACTION)
               {
                  curAction = dlg->options[option].totalElseActions;
               }
               else
               {
                  curAction = dlg->options[option].totalIfActions;
               }

               if(curAction < MAX_ACTIONS)
               {
                  /* Increment the number of actins and set a pointer to work */
                  if(token == TK_ELSE_ACTION)
                  {
                     dlg->options[option].totalElseActions++;
                     tact = &dlg->options[option].elseAction[curAction];
                  }
                  else
                  {
                     dlg->options[option].totalIfActions++;
                     tact = &dlg->options[option].ifAction[curAction];
                  }

                  /* Get The action and its parameters */
                  token = getString(position, buffer, separator);               
                  tact->id = getActionID(token, name,line);

                  /* Parse Action Parameters */
                  if( (tact->id == TALK_ACTION_GO_TO_DIALOG) ||
                      (tact->id == TALK_ACTION_DIALOG_INIT) ||
                      (tact->id == TALK_ACTION_RECEIVE_MONEY) ||
                      (tact->id == TALK_ACTION_CHANGE_OBJECT_STATE) ||
                      (tact->id == TALK_ACTION_RECEIVE_XP))
                  {
                     //get number
                     token = getString(position, buffer, separator);
                     tact->att = atoi(token.c_str());
                  }
                  else if( (tact->id == TALK_ACTION_ADD_MISSION) ||
                           (tact->id == TALK_ACTION_COMPLETE_MISSION))
                  {
                     //get mission
                     token = getString(position, buffer, separator);
                     tact->satt = token;

                     //get xp, if needed
                     if(tact->id == TALK_ACTION_COMPLETE_MISSION)
                     {
                        token = getString(position, buffer, separator);
                        tact->att =  atoi(token.c_str());

                        //and get completion type
                        token = getString(position, buffer, separator);
                        tact->qty =  atoi(token.c_str());
                     }
                  }
                  else if( (tact->id == TALK_ACTION_GIVE_ITEM) ||
                           (tact->id == TALK_ACTION_RECEIVE_ITEM) ||
                           (tact->id == TALK_ACTION_KILL_ALL) )
                  {
                     //get name
                     token = getString(position, buffer, separator);
                     tact->satt = token;
                  }
               }
               else
               {
                  printError(name, "Actions List overflow!", line);
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

   dlg->npcText = "";

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
      printf(gettext("Not found on dialog: %d\n"),num);
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
 *                           setInitialDialog                            *
 *************************************************************************/
void conversation::setInitialDialog(int numDialog)
{
   modState mod;
   
   /* Set the initial dialog */
   initialDialog = numDialog;
   
   /* Tell the modeState */
   if(owner)
   {
      if(owner->getThingType() == THING_TYPE_CHARACTER)
      {
         character* ownerNPC = (character*)owner;
         mod.mapTalkAddAction(MODSTATE_TALK_ENTER_VALUE, 
                              ownerNPC->getCharacterFile(),
                              ownerMap, numDialog);
      }
   }
}

/*************************************************************************
 *                               setOwner                                *
 *************************************************************************/
void conversation::setOwner(thing* o, string mapFile)
{
   owner = o;
   ownerMap = mapFile;
}

/*************************************************************************
 *                                 setPC                                 *
 *************************************************************************/
void conversation::setPC(character* PC)
{
   actualPC = PC;
}

/*************************************************************************
 *                                getOwner                               *
 *************************************************************************/
thing* conversation::getOwner()
{
   return(owner);
}

/*************************************************************************
 *                                 getPC                                 *
 *************************************************************************/
character* conversation::getPC()
{
   return(actualPC);
}

/*************************************************************************
 *                            proccessAction                             *
 *************************************************************************/
void conversation::proccessAction(int opcao, void* curEngine)
{
   /* Get dialog on list */
   int numDialog = actual;
   dialogWindow dlgWindow;
   dialog* dlg = first->next;
   int i, totalActions = 0;
   talkAction* actions = NULL;

   while( (dlg != first) && (dlg->id != numDialog))
   {
      dlg = dlg->next;
   }
   if(dlg == first)
   {
      return;
   }

   /* To the post check to resolve wich actions take (if or else actions) */
   if(dlg->options[opcao].postTest.doTest(actualPC, owner))
   {
      /* Passed test, so if action */
      totalActions = dlg->options[opcao].totalIfActions;
      actions = &dlg->options[opcao].ifAction[0];
   }
   else
   {
      /* Failed test, so else action */
      totalActions = dlg->options[opcao].totalElseActions;
      actions = &dlg->options[opcao].elseAction[0];
   }

   /* Take all actions */
   for(i = 0; i < totalActions; i++)
   {
      switch(actions[i].id)
      {
         /* Change Dialog */
         case TALK_ACTION_GO_TO_DIALOG:
            changeDialog(actions[i].att);
         break;
         /* Init a Fight with the owner */
         case TALK_ACTION_INIT_FIGHT:
         {
            engine* eng = (engine*)curEngine;
            owner->setAsEnemy();
            eng->enterBattleMode(false);
            dlgWindow.close();
         }
         break;
         /* Close the dialog */
         case TALK_ACTION_FINISH_DIALOG:
            dlgWindow.close();
         break;
         case TALK_ACTION_MOD_PC:
            //TODO
         break;
         case TALK_ACTION_MOD_NPC:
            //TODO
         break;
         /* Set new initial dialog */
         case TALK_ACTION_DIALOG_INIT:
         {
            setInitialDialog(actions[i].att);
         }
         break;
         /* Add a misstion */
         case TALK_ACTION_ADD_MISSION:
         {
            missionsController missions;
            missions.addNewMission(actions[i].satt);
         }
         break;
         /* Complete a mission */
         case TALK_ACTION_COMPLETE_MISSION:
         {
            missionsController missions;
            mission* m;
            m=missions.getCurrentMission(actions[i].satt);
            if(m)
            {
               m->setXp(actions[i].att);
               missions.completeMission(m, actions[i].qty);
               char vstr[200];
               sprintf(vstr,gettext("Mission Completed: %d XP!"),m->getXp());
               messageController msgController;
               msgController.addMessage(actualPC->xPosition,
                                        actualPC->max[1]+actualPC->yPosition,
                                        actualPC->zPosition, vstr,
                                        0.94, 0.8, 0.0);
            }
         }
         break;
         /* Give a Item */
         case TALK_ACTION_GIVE_ITEM:
         {
            /* Only give item if the owner is a character */
            if(owner->getThingType() == THING_TYPE_CHARACTER)
            {
               character* ownerNPC = (character*)owner;
               /* Search for the item at actor's inventory */
               object* obj = actualPC->inventories->getItemByFileName(
                     actions[i].satt);
               if(obj)
               {
                  /* Remove it from there */
                  actualPC->inventories->removeFromInventory(obj);

                  /* Add it to the Owner NPC inventory */
                  ownerNPC->inventories->addObject(obj);
                  /* NOTE: The NPC inventory is always saved at modstate when 
                   * the PC leaves the map, and reloaded when it come back */
               }
               else
               {
                  cerr << "Error: No object '" << actions[i].satt 
                     << "' to give at character inventory!" << endl;
               }
            }
         }
         break;

         /* Receive Item */
         case TALK_ACTION_RECEIVE_ITEM:
         {
            /* Search for the item (or load one, if needed) */
            object* obj = objectsList::search(actions[i].satt, 0,0,0);
            modState modif;
            Map* actualMap = ((engine*)curEngine)->getCurrentMap();
            weaponTypes* wTypes = ((engine*)curEngine)->getWeaponTypes();
            modelList* mdlList = ((engine*)curEngine)->getModelList();

            if(obj == NULL)
            {
               /* Must load one */
               obj = actualMap->insertObject(actions[i].satt, (*mdlList), 
                                             (*wTypes));
            }

            /* Now, put it at the actualCharacter Inventory */
            if(obj != NULL)
            {
               if(!actualPC->inventories->addObject(obj))
               {
                  /* Can't add to the inventory (is full?), so puting it
                   * at the floor. */
                  float height =  actualMap->getHeight(actualPC->xPosition, 
                                                       actualPC->zPosition);
                  actualMap->insertObject(actualPC->xPosition, height,
                                          actualPC->zPosition, 0, obj, 0);
                  modif.mapObjectAddAction(MODSTATE_ACTION_OBJECT_ADD,
                                           actions[i].satt,
                                           actualMap->getFileName(),
                                           actualPC->xPosition, height,
                                           actualPC->zPosition);
               }
            }
         }
         break;

         /* Receive Money */
         case TALK_ACTION_RECEIVE_MONEY:
            //TODO
         break;
         /* Change Object State */
         case TALK_ACTION_CHANGE_OBJECT_STATE:
         {
            /* Change the state */
            object* obj = (object*)owner;
            obj->setState(actions[i].att);
            /* Tell ModState about the change */
            modState modif;
            modif.mapObjectAddAction(MODSTATE_ACTION_OBJECT_CHANGE_STATE,
                                     obj->getFileName(), ownerMap,
                                     obj->xPosition, obj->yPosition, 
                                     obj->zPosition, obj->getState());
         }
         break;
         /* Receive XP */
         case TALK_ACTION_RECEIVE_XP:
         {
            /* At XP to the character */
            actualPC->addXP(actions[i].att);

            /* Set the Message */
            char vstr[200];
            sprintf(vstr, "%d XP", actions[i].att);

            /* Put Message at game */
            messageController msgController;
            msgController.addMessage(actualPC->xPosition,
                  actualPC->max[1]+actualPC->yPosition,
                  actualPC->zPosition, vstr,
                  0.94, 0.8, 0.0);

            /* Put Message at Briefing */
            briefing brief;
            brief.addText(vstr, 251, 209, 12);
         }
         break;

         /* Kill All NPCs from a file */
         case TALK_ACTION_KILL_ALL:
         {
            npcFile npcs;
            if(npcs.load(actions[i].satt))
            {
               npcs.killAll();
            }
         }
         break;
      }
   }
}

/*************************************************************************
 *                            changeDialog                               *
 *************************************************************************/
void conversation::changeDialog()
{
   actual = -1;
   changeDialog(initialDialog);
}

/*************************************************************************
 *                            changeDialog                               *
 *************************************************************************/
void conversation::changeDialog(int numDialog)
{
   int i, j, curOpt;
   string text;
   char conv[16];
   dialogWindow dlgWindow;

   if(numDialog == actual)
   {
      /* No change at the same dialog! */
      return;
   }

   /* Get the dialog pointer */
   dialog* dlg = first->next;
   while( (dlg != first) && (dlg->id != numDialog))
   {
      dlg = dlg->next;
   }
   if(dlg == first)
   {
      return;
   }

   /* Define the current dialog number */
   actual = numDialog;

   /* Define the NPC Text */
   dlgWindow.setNPCText(dlg->npcText);

   /* Define the options */
   curOpt = 0;
   dlgWindow.clearOptions();  
   for(i = 0; i < MAX_OPTIONS; i++)
   {
      /* Only insert the option if isn't empty */
      if(!dlg->options[i].text.empty())
      {
         bool res = true;

         /* And passes all preTests */
         for(j = 0; j < MAX_PRE_TESTS; j++)
         {
            res &= dlg->options[i].preTest[j].doTest(actualPC, owner);
         }

         /* So, if pass all, add the option */
         if(res)
         {
            sprintf(conv, "%d - ", curOpt+1);
            text = conv + dlg->options[i].postTest.getTestName(actualPC) + 
               dlg->options[i].text;
            dlgWindow.addOption(curOpt, text, i);
            curOpt++;
         }
      }
   }
}



///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                            dialogWindow                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                                 open                                  *
 *************************************************************************/
void dialogWindow::open(guiInterface* gui, character* PC, conversation* cv,
                        string pictureFile)
{
   dirs dir;

   /* Do not open NULL conversations */
   if(!cv)
   {
      return;
   }

   /* Close the window, if it is currently opened */
   if(isOpened())
   {
      if(cv == conv)
      {
         /* Dialog for this one is already opened! */
         return;
      }
      close();
   }

   /* Set Pointers*/
   conv = cv;
   usedGui = gui;
   conv->setPC(PC);

   /* Create the Window */
   jan = gui->insertWindow(280,100,605,355,gettext("Dialog"));
   
   /* Barter Button (only for characters) */
   if(conv->getOwner()->getThingType() == THING_TYPE_CHARACTER)
   {
      barterButton = jan->getObjectsList()->insertButton(5,86,69,104,
                                                         gettext("Barter"),1);
   }
   else
   {
      barterButton = NULL;
   }

   /* Picture */
   jan->getObjectsList()->insertPicture(10,25,0,0, 
                                        dir.getRealFile(pictureFile).c_str());
   
   /* Texts */
   npcText = jan->getObjectsList()->insertRolBar(71,20,320,115,"");
   pcSelText = jan->getObjectsList()->insertSelText(5,116,320,250,"","","",
                                                    "","");
   
   /* Open the Window */
   jan->setExternPointer(&jan);
   gui->openWindow(jan);

   /* Set the initial dialog */
   conv->changeDialog();
}

/*************************************************************************
 *                                close                                  *
 *************************************************************************/
void dialogWindow::close()
{
   if((usedGui) && (jan))
   {
      usedGui->closeWindow(jan);
      jan = NULL;
   }
}

/*************************************************************************
 *                               redraw                                  *
 *************************************************************************/
void dialogWindow::redraw()
{
   if(isOpened())
   {
      jan->draw(0,0);
   }
}

/*************************************************************************
 *                                treat                                  *
 *************************************************************************/
bool dialogWindow::treat(guiObject* guiObj, int eventInfo, itemWindow* infoW,
                         void* curEngine)
{
   barterWindow tradeWindow;
   int index = -1;

   if(!jan)
   {
      /* No Opened window, so no event to treat here */
      return(false);
   }

   if(eventInfo == SELECTED_SEL_TEXT)
   {
      if(guiObj == (guiObject*)pcSelText)
      {
         /* Get the last Selected Item Index */
         pcSelText->getLastSelectedItem(&index);

         if(index != -1)
         {
            /* Process the action! */
            conv->proccessAction(index, curEngine);
         }
         return(true);
      }
   }
   else if(eventInfo == PRESSED_BUTTON)
   {
      if(guiObj == (guiObject*)barterButton)
      {
         /* Closes the dialog window  */
         close();

         /* If exists a barter, delete it! */
         if( (tradeWindow.isOpen()))
         {
            tradeWindow.close();
         }
         
         /* Open the trade. If trade button is avalaible, the
          * owner is, for sure, a character. */
         tradeWindow.open((character*)conv->getOwner(), conv->getPC(), 
                          usedGui, infoW);

      }
   }
   return(false);
}

/*************************************************************************
 *                              isOpened                                 *
 *************************************************************************/
bool dialogWindow::isOpened(conversation* cv)
{
   if(isOpened())
   {
      return(cv != conv);
   }

   return(false);
}

/*************************************************************************
 *                              isOpened                                 *
 *************************************************************************/
bool dialogWindow::isOpened()
{
   return(jan != NULL);
}

/*************************************************************************
 *                            setNPCText                                 *
 *************************************************************************/
void dialogWindow::setNPCText(string text)
{
   if(isOpened())
   {
      npcText->setText(text);
   }
}

/*************************************************************************
 *                           clearOptions                                *
 *************************************************************************/
void dialogWindow::clearOptions()
{
   if(isOpened())
   {
      pcSelText->clearText();
   }
}

/*************************************************************************
 *                            setNPCText                                 *
 *************************************************************************/
void dialogWindow::addOption(int optNumber, string text, int info)
{
   if(isOpened())
   {
      pcSelText->setText(optNumber, text, info);
   }
}

/*************************************************************************
 *                             Static Members                            *
 *************************************************************************/
conversation* dialogWindow::conv = NULL;
window* dialogWindow::jan = NULL;
guiInterface* dialogWindow::usedGui = NULL;
rolBar* dialogWindow::npcText = NULL;
selText* dialogWindow::pcSelText = NULL;
button* dialogWindow::barterButton = NULL;

