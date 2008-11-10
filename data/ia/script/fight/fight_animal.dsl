#############################################################
# This is a dummy script to control animal combat behaviour
#
# NOTE: the control if character can attack, can move or not
#       is done at the script controller, not at the script
#       (this simplify the script code)
script()

   # Verify if the curren life is too low.
   # If is the case, try to run away
   int curLife
   curLife = getActualLife(SELF_CHARACTER)
   if(curLife < 2)
      runAwayFromBattle()

   # Else (if have enough life, try to attack and be near enemy)
   # Cause it's an animal and don't have range attacks
   else
      # verify if the enemy (active character) is reachable
      if(!characterAtRange(SELF_CHARACTER, ACTIVE_CHARACTER))
         moveToCharacter(SELF_CHARACTER, ACTIVE_CHARACTER)
      end

      # Now, finally, attack the enemy!
      int at
      at = getRandomAttackFeat(ACTIVE_CHARACTER)
      featUseAtCharacter(at, ACTIVE_CHARACTER)
   end

end


