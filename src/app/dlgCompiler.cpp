#include "../engine/conversa.h"
#include "../classes/defs.h"

void erro()
{
   printf("/\n");
   printf("| Use dlgCompiler -f filename:\n");
   printf("| -f filename : filename to compile\n");
   exit(2);
}

string returnString(char buffer[512])
{
   int aux = 0;
   string str = "";
   while(buffer[aux] != '"') 
   {
      aux++;
   }
   aux++;
   while(buffer[aux] != '"')
   {
      str = str+buffer[aux];
      aux++;
   }
   return(str);
   
}

int defineAtt(string att)
{
    /*if (!att.compare("AGILITY"))
    {
       return(ATT_AGILITY);
    }
    if (!att.compare("BOVINICE"))
    {
       return(ATT_BOVINICE);
    }
    if (!att.compare("BRUTALITY"))
    {
       return(ATT_BRUTALITY);
    }
    if (!att.compare("WISDOW"))
    {
       return(ATT_WISDOW);
    }
    if (!att.compare("STRONG"))
    {
       return(ATT_STRONG);
    }
    if (!att.compare("GAMBIARRY"))
    {
       return(ATT_GAMBIARRY);
    }
    if (!att.compare("INTELIGENCY"))
    {
       return(ATT_INTELIGENCY);
    }
    if (!att.compare("HATE"))
    {
       return(ATT_HATE);
    }*/
    return(-1);
}

int defineOper(string sinal)
{
   if(!sinal.compare(">="))
   {
      return(TALK_OPER_GREATEREQUAL);
   }
   if(!sinal.compare("<="))
   {
      return(TALK_OPER_LESSEREQUAL);
   }
   if(!sinal.compare("="))
   {
      return(TALK_OPER_EQUAL);
   }
   if(!sinal.compare(">"))
   {
      return(TALK_OPER_GREATER);
   }
   if(!sinal.compare("<"))
   {
      return(TALK_OPER_LESSER);
   }
   return(-1);
}

int defineOperAct(string oper)
{
  if(!oper.compare("0"))
  {
    return(TALK_OPER_ZERO);
  }
  if(!oper.compare("-"))
  {
    return(TALK_OPER_MINUS);
  }
  if(!oper.compare("+"))
  {
    return(TALK_OPER_PLUS);
  }
  return(-1);
}

int defineAction(string act,int oper,int att, int* errors, int line)
{
   if(!act.compare("GOTO"))
   {
      return(TALK_ACTION_GOTO);
   }
   if(!act.compare("FIGHT"))
   {
      return(TALK_ACTION_FIGHT);
   }
   if(!act.compare("CLOSE"))
   {
      return(TALK_ACTION_CLOSE);
   }
   if(!act.compare("MODPC"))
   {
      if(att == -1)
      {
         printf("Error: Unknow Attribute on line: %d\n",line);
         *errors++;
      }
      if(oper == -1)
      {
         printf("Error: Unknow Operation on line: %d\n",line);
         *errors++;
      }
      return(TALK_ACTION_MODPC);
   }
   if(!act.compare("MODNPC"))
   {
      if(att == -1)
      {
         printf("Error: Unknow Attribute on line: %d\n",line);
         *errors++;
      }
      if(oper == -1)
      {
         printf("Error: Unknow Operation on line: %d\n",line);
         *errors++;
      }
      return(TALK_ACTION_MODNPC);
   }
   printf("Error: Unknow Action: %s , on line: %d\n",act.c_str(),line);
   *errors++;
   return(-1);
}


int main(int argc, char **argv)
{
   printf("DccNiTghtmare Dialog Compiler\n");
   printf("\tCompiler Version 0.2,\n\tfor Dccnitghtmare Version 0.0.3\n");

   char* entrada = NULL;
   char c;
   int chamadaCorreta = 0;

   while((c=getopt(argc,argv,":f:")) != -1){
      switch(c){
         case 'f':{
                     entrada = optarg;
                     chamadaCorreta = 1; 
                     break;
                  }
         default: erro();
      } 
   }
   if(!chamadaCorreta) erro();

   FILE* arq;
   char buffer[512];
   char lido1[512];
   char lido2[512];
   char lido3[512];
   int line = 0;
   int errors = 0;
 
   dialogo* dlg = NULL;
   conversa* conv = new(conversa);

   if(!(arq = fopen(entrada,"r")))
   {
      printf("Error while opening dialog text file: %s\n", entrada);
	exit(3);
   }

   bool ifOpened = false;
   bool foundPC = false;
   bool foundNPC = false;
   bool initDlg = false;
   bool foundAction = true;
   int actualPC = 0;

   string att;
   string sinal;
   string acao;

   seSenao* se = NULL;

   printf("\nProcessing lines\n");

   while(fscanf(arq, "%s", buffer) != EOF)
   {
      line++;
      switch(buffer[0])
      {
         case 'd': /* New Dialog */
               if(!foundAction) 
               {
                  printf("Error: not founded action: %d\n",line);
                  errors++;
               }
               if( (initDlg) && (!foundPC))
               {
                  printf("Error: Not found PC info: %d\n",line);
                  errors++;
               }
               if( (initDlg) && (!foundNPC))
               {
                  printf("Error: Not found NPC info: %d\n",line);
                  errors++;
               }
               foundPC = false;
               foundNPC = false;
               initDlg = true;
               foundAction = true;
               actualPC = 0;
               dlg = conv->inserirDialogo();
               fgets(buffer, sizeof(buffer), arq);
               sscanf(buffer, "%d",&dlg->id);
         break;
         case 'n': /* NPC */
               
               if(!initDlg)
               {
                  printf("Error: Dialog not initialized: %d\n",line);
                  errors++;
               }
               if(foundNPC)
               {
                  printf("Error: Duplicate NPC info: %d\n",line);
                  errors++;
               }
               fgets(buffer, sizeof(buffer), arq);
               foundNPC = true;
               se = &dlg->NPC;
         break;
         case 'p': /* PC, options to pc talk */
               if(!initDlg)
               {
                  printf("Error: Dialog not initialized: %d\n",line);
                  errors++;
               }
               if(foundPC)
               { 
                  printf("Error: Duplicate PC info: %d\n",line);
                  errors++;
               }
               if(!foundNPC)
               {
                  printf("Error: NPC info should be first: %d\n",line);
                  errors++;
               }
               foundAction = true;
               fgets(buffer, sizeof(buffer), arq);
               foundPC = true;
               se = &dlg->Opcoes[0];
         break;
         case 'w': /* without IF */
               if(!foundAction) 
               {
                  printf("Error: not founded action: %d\n",line);
                  errors++;
               }
               foundAction = false;

               if(!initDlg)
               {
                  printf("Error: Dialog not initialized: %d\n",line);
                  errors++;
               }
               if( (!foundPC) && (!foundNPC) )
               {
                  printf("Error: PC or NPC not initialized: %d\n",line);
                  errors++;
               }

               fgets(buffer, sizeof(buffer), arq);
               se->Se = returnString(buffer);
               if(ifOpened)
               {
                  printf("Else not found on line %d.\n",line);
                  errors++;
               }
               if(foundPC)
               {
                  actualPC++;
                  se = &dlg->Opcoes[actualPC];
               }
         break;
         case 'i': /* IF  */
               if(!foundAction) 
               {
                  printf("Error: not founded action: %d\n",line);
                  errors++;
               }
               foundAction = false;

               if(!initDlg)
               {
                  printf("Error: Dialog not initialized: %d\n",line);
                  errors++;
               }
               if( (!foundPC) && (!foundNPC) )
               {
                  printf("Error: PC or NPC not initialized: %d\n",line);
                  errors++;
               }

               if(ifOpened)
               {
                  printf("Error: Else not found on line %d.\n",line);
                  errors++;
               }
               
               fgets(buffer, sizeof(buffer), arq);
                             /*att,sin,num,texto*/
               sscanf(buffer, "%s %s %d",&lido1[0],&lido2[0],&se->Limite);
               att = lido1;
               sinal = lido2;
               se->Se = returnString(buffer);
               se->Atributo = defineAtt(att);
               if(se->Atributo == -1)
               {
                  printf("Error: Unknow attribute: %s : %d\n",att.c_str(),line);
                  errors++;
               }
               se->Operador = defineOper(sinal);
               if(se->Operador == -1)
               {
                  printf("Error: Unknow operator: %s : %d\n",sinal.c_str(),line);
                  errors++;
               }
               ifOpened = true;
         break;
         case 'e': /* ELSE */
               if(!foundAction) 
               {
                  printf("Error: not founded action: %d\n",line);
                  errors++;
               }
               foundAction = false;

               if(!initDlg)
               {
                  printf("Error: Dialog not initialized: %d\n",line);
                  errors++;
               }
               if( (!foundPC) && (!foundNPC) )
               {
                  printf("Error: PC or NPC not initialized: %d\n",line);
                  errors++;
               }

               if(!ifOpened)
               {
                  printf("Error: Found Else without If on %d.\n",line);
                  errors++;
               }
               ifOpened = false;
               fgets(buffer, sizeof(buffer), arq);
               se->Senao = returnString(buffer);
               if(foundPC)
               {
                  actualPC++;
                  se = &dlg->Opcoes[actualPC];
               }
         break;
         case 'a': /* TALK_ACTION */
              if(foundAction) 
               {
                  printf("Error: duplicated action: %d\n",line);
                  errors++;
               }
               if(!initDlg)
               {
                  printf("Error: Dialog not initialized: %d\n",line);
                  errors++;
               }
               if( (!foundPC) && (!foundNPC) )
               {
                  printf("Error: PC or NPC not initialized: %d\n",line);
                  errors++;
               }
   
               fgets(buffer, sizeof(buffer), arq);
                             /*acao,att,sin*/
               if(!ifOpened)
               {
                  sscanf(buffer, "%s,%s,%s,%d",&lido1[0],&lido2[0],
                        &lido3[0],
                        &se->SenaoAcao.qty);
                  acao = lido1;
                  att = lido2;
                  sinal = lido3;
                  se->SenaoAcao.att = defineAtt(att);
                  se->SenaoAcao.oper = defineOperAct(sinal);
                  se->SenaoAcao.id = defineAction(acao,
                          se->SenaoAcao.oper, se->SenaoAcao.att, 
                          &errors, line);
               }
               else
               { 
                  sscanf(buffer, "%s,%s,%s,%d",&lido1[0],&lido2[0],
                        &lido3[0],
                        &se->SeAcao.qty);
                  acao = lido1;
                  att = lido2;
                  sinal = lido3;
                  se->SeAcao.att = defineAtt(att);
                  se->SeAcao.oper = defineOperAct(sinal);
                  se->SeAcao.id = defineAction(acao,
                          se->SeAcao.oper, se->SeAcao.att, 
                          &errors, line);
               }

               foundAction = true;

         break;
         case '#': /* IGNORE COMMENTS */
               fgets(buffer, sizeof(buffer), arq);
         break;
         default:
           printf("Error on line %d: %s\n",line,buffer);
           errors++;
         break;
      }
   }

   fclose(arq);

   printf("Making result file...\n");
   conv->salvarArquivo("a.out");

   if(errors > 0)
   {
      printf("%d errors. File can be wrong!\n\n",errors);
   }
   else
   {
      printf("Compile done without errors.\n");
      printf("Total Lines: %d\n\n",line);
   }

}

