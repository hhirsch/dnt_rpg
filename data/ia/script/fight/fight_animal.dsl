#############################################################
# This is a dummy script to control animal combat behaviour
#
# NOTE: the control if character can attack, can move or not
#       is done at the script controller, not at the script
#       (this simplify the script code)
script()

   int at
   character tgt

   # Verify if the curren life is too low.
   # If is the case, try to run away
   int curLife
   
   curLife = getActualLife(SELF_CHARACTER)
   
   if(curLife < 2)
      print("Will run away!"
      runAwayFromBattle()

   # Else (if have enough life, try to attack and be near enemy)
   # Cause it's an animal and don't have range attacks
   else if(TARGET_CHARACTER)
      print("Got Target!")
      # verify if the enemy (active character) is reachable
      if(!characterAtRange(SELF_CHARACTER, TARGET_CHARACTER))
         moveToCharacter(SELF_CHARACTER, TARGET_CHARACTER)
      end

      # Now, finally, attack the enemy!
      if(characterAtRange(SELF_CHARACTER, TARGET_CHARACTER))
         at = getRandomAttackFeat(TARGET_CHARACTER)
         featUseAtCharacter(at, TARGET_CHARACTER)
      end
   else
      print("No target!")
      # Set enemy
      tgt = getNearestEnemy(SELF_CHARACTER)
      if(tgt)
         setTargetCharacter(SELF_CHARACTER, tgt);
      end
   end

end


