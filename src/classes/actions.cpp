#include "actions.h"

int numberAction(string buffer)
{
   int ret = -1;
   if(buffer.compare(ACT_STR_OPEN) == 0)
     ret = ACT_OPEN;
   else if(buffer.compare(ACT_STR_ATTACK) == 0)
     ret = ACT_ATTACK;
   else if(buffer.compare(ACT_STR_DRINK) == 0)
     ret = ACT_DRINK;
   else if(buffer.compare(ACT_STR_EAT) == 0)
     ret = ACT_EAT;
   else if(buffer.compare(ACT_STR_DEFEND) == 0)
     ret = ACT_DEFEND;
   else if(buffer.compare(ACT_STR_TALK) == 0)
     ret = ACT_TALK;
   else if(buffer.compare(ACT_STR_CLOSE) == 0)
     ret = ACT_CLOSE;
   else if(buffer.compare(ACT_STR_MOVIMENT) == 0)
     ret = ACT_MOVIMENT;
   else if(buffer.compare(ACT_STR_TAKE) == 0)
     ret = ACT_TAKE;
   else if(buffer.compare(ACT_STR_FREE) == 0)
     ret = ACT_FREE;
   else if(buffer.compare(ACT_STR_MODIFY_OWN_CONCEPT) == 0)
     ret = ACT_MODIFY_OWN_CONCEPT;
   else if(buffer.compare(ACT_STR_MODIFY_OTHER_CONCEPT) == 0)
     ret = ACT_MODIFY_OTHER_CONCEPT;
   else if(buffer.compare(ACT_STR_CONJURE) == 0)
     ret = ACT_CONJURE;
   else if(buffer.compare(ACT_STR_MODIFY_OBJECT) == 0)
     ret = ACT_MODIFY_OBJECT;
   else if(buffer.compare(ACT_STR_CURE) == 0)
     ret = ACT_CURE;
   else if(buffer.compare(ACT_STR_FIX) == 0)
     ret = ACT_FIX;
   else if(buffer.compare(ACT_STR_BREAK) == 0)
     ret = ACT_BREAK;

   return(ret);
}

int numberActionType(string buffer)
{
   int ret = -1;

   if(buffer.compare(ACT_TYPE_STR_FREE_ACTION) == 0)
     ret = ACT_TYPE_FREE_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_NORMAL_ACTION) == 0)
     ret = ACT_TYPE_NORMAL_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_SECUNDARY_ACTION) == 0)
     ret = ACT_TYPE_SECUNDARY_ACTION;
   else if(buffer.compare(ACT_TYPE_STR_NO_ACTION) == 0)
     ret = ACT_TYPE_NO_ACTION;

   return(ret);
}
