#include "feats.h"
#include "thing.h"
#include "../engine/personagens.h"
#include "../fight/fightSystem.h"
#include "../lang/lang.h"


int main()
{
   lang language;
   language.ReloadFile(LANG_FRANCES);
   featsList* ft;
   ft = new featsList("../data/feats/Ingles/","../data/feats/feats.ftl");

   /*int aux;
   int i;
   for(aux = 0;aux < NUMBER_OF_FEATS;aux++)
   {
      featDescription Feat = ft->featByNumber(aux);
      printf("%d: %s\n",Feat.internalListNumber,Feat.name.c_str());
      printf("Descrição: %s\n",Feat.description.c_str());
      printf("ReqLevel: %d\t\tReqClass: %d\n",Feat.requeridedLevel,
                                              Feat.requeridedClass);
      printf("QuantityPerDay: %d\tCost: %d\n",Feat.quantityPerDay, 
                                              Feat.costToUse);
      printf("AditionalQuantity: %d\tAditional Levels: %d\n",
                                              Feat.aditionalQuantity, 
                                              Feat.aditionalLevels);
      printf("ActType: %d\t\tAction: %d\n",Feat.actionType, Feat.action);
      printf("Dices: %d*d%d+%d\t\t",Feat.diceInfo.baseDice.numberOfDices,
                         Feat.diceInfo.baseDice.diceID,
                         Feat.diceInfo.baseDice.sumNumber);
      printf("%d*d%d+%d\n",Feat.diceInfo.aditionalDice.numberOfDices,
                         Feat.diceInfo.aditionalDice.diceID,
                         Feat.diceInfo.aditionalDice.sumNumber);
      printf("Concept: Bonus: %d Against: %d Target: %d\n",
                         Feat.conceptBonus,
                         Feat.conceptAgainst,
                         Feat.conceptTarget);
      for(i=0; i<MAX_DEP_FEATS;i++)
      {
         printf("Feat: %s - %.3f,%d\n",
                                 Feat.depFeats[i].featName.c_str(),
                                 Feat.depFeats[i].reason,
                                 Feat.depFeats[i].used);
      }

   }*/

   diceThing dc;
   dc.baseDice.diceID = DICE_D6;
   dc.baseDice.numberOfDices = 2;
   dc.baseDice.sumNumber = 1;
   dc.initialLevel = 1;

   personagem target(ft);
   personagem attacker(ft);
   attacker.sk.m_skills[ATT_CONSTITUTION].pontos = 14;
   attacker.sk.m_skills[ATT_STRENGHT].pontos = 16;
   attacker.sk.m_skills[ATT_WISDOW].pontos = 10;
   attacker.sk.m_skills[ATT_INTELIGENCY].pontos = 11;
   attacker.sk.m_skills[ATT_DEXTERY].pontos = 13; 
   attacker.sk.m_skills[ATT_CHARISM].pontos = 8;
   attacker.lifePoints = 42;
   attacker.armatureClass = 8;
   attacker.actualFeats.defineMeleeWeapon(dc);
   attacker.nome = "Carballo";



   target.lifePoints = 55;
   target.armatureClass = 12;
   target.sk.m_skills[ATT_CONSTITUTION].pontos = 14;
   target.sk.m_skills[ATT_STRENGHT].pontos = 18;
   target.sk.m_skills[ATT_WISDOW].pontos = 6;
   target.sk.m_skills[ATT_INTELIGENCY].pontos = 8;
   target.sk.m_skills[ATT_DEXTERY].pontos = 14; 
   target.sk.m_skills[ATT_CHARISM].pontos = 14;
   target.nome = "Arteaga";

   dc.baseDice.diceID = DICE_D8;
   dc.baseDice.numberOfDices = 1;
   dc.baseDice.sumNumber = 3;
   dc.initialLevel = 1;
   target.actualFeats.defineMeleeWeapon(dc);


   personagem other(ft);
   other.sk.m_skills[ATT_CONSTITUTION].pontos = 11;
   other.sk.m_skills[ATT_STRENGHT].pontos = 13;
   other.sk.m_skills[ATT_WISDOW].pontos = 12;
   other.sk.m_skills[ATT_INTELIGENCY].pontos = 11;
   other.sk.m_skills[ATT_DEXTERY].pontos = 13; 
   other.sk.m_skills[ATT_CHARISM].pontos = 10;
   other.lifePoints = 41;
   other.armatureClass = 10;
   other.actualFeats.defineMeleeWeapon(dc);
   other.nome = "Gaspar";

   dc.baseDice.diceID = DICE_D4;
   dc.baseDice.numberOfDices = 2;
   dc.baseDice.sumNumber = 1;
   dc.initialLevel = 1;
   other.actualFeats.defineMeleeWeapon(dc);

   personagem ot(ft);
   ot.sk.m_skills[ATT_CONSTITUTION].pontos = 13;
   ot.sk.m_skills[ATT_STRENGHT].pontos = 18;
   ot.sk.m_skills[ATT_WISDOW].pontos = 12;
   ot.sk.m_skills[ATT_INTELIGENCY].pontos = 12;
   ot.sk.m_skills[ATT_DEXTERY].pontos = 14; 
   ot.sk.m_skills[ATT_CHARISM].pontos = 5;
   ot.lifePoints = 46;
   ot.armatureClass = 16;
   ot.actualFeats.defineMeleeWeapon(dc);
   ot.nome = "Gilcione";

   dc.baseDice.diceID = DICE_D8;
   dc.baseDice.numberOfDices = 2;
   dc.baseDice.sumNumber = 4;
   dc.initialLevel = 1;
   ot.actualFeats.defineMeleeWeapon(dc);


   string brief;

   fightSystem fight;
   attacker.psychoState = PSYCHO_HOSTILE;
   fight.insertNPC(&attacker, 0, brief);
   printf("%s\n",brief.c_str());
   brief = "";
   target.psychoState = PSYCHO_HOSTILE;
   fight.insertNPC(&target, 1, brief);
   printf("%s\n",brief.c_str());
   brief = "";
   other.psychoState = PSYCHO_HOSTILE;
   fight.insertNPC(&other, 0, brief);
   printf("%s\n",brief.c_str());
   brief = "";
   ot.psychoState = PSYCHO_HOSTILE;
   fight.insertNPC(&ot, 2, brief);
   printf("%s\n",brief.c_str());
   brief = "";


   while(fight.doBattleCicle(brief) != FIGHT_END)
   {
      printf("%s\n",brief.c_str());
      brief = "";
   }

   printf("%s\n",brief.c_str());
   brief = "";


   /*while( (target.lifePoints > 0) &&
          (fe->applyAttackAndBreakFeat(attacker,0,target,brief)))
   {
      printf("%s\n",brief.c_str());
      SDL_Delay(50);
   }
  
   printf("Life Points: %d\n",target.lifePoints);
   if(target.lifePoints <= 0 )
   {
       printf("Dead!\n");
   }*/

   delete(ft);
}

