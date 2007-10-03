#An NPC behaviour for a turn on the combat mode
#It's our first test for the NPC combat IA scripts

script()
   int actualLife

   # Get the actual life of the character
   actualLife = getActualLife(SELF_CHARACTER)
   if(actualLife < 10)
      if(haveHealFunction)
         #if can cure itself, cure
         healCharacter(SELF_CHARACTER)
      end
      else
         #if can't cure itself, try to run away
         runAwayFromBattle();
      end
   end

   if(canAttack)
      character dude
      dude = getNearestEnemy(SELF_CHARACTER)
      if(!characterAtRange(dude))
         moveToCharacterRange(dude)
      end

      if(canAttack)
         feat at
         at = getRandomAttackFeat()
         featUseAtCharacter(at, dude)
      end
   end

   if(canMove)
      dude = getNearestEnemy(SELF_CHARACTER)
      if(!characterAtRange(dude))
         moveToCharacterRange(dude)
      end
   end
end


