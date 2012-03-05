
# The cattle_idle script implements a simple behaviour for cattle actions when
# they are idle on a map;

# Cattle States (actions):
#  1  - stand
#  2  - look
#  3  - eat
#  4  - walk to another position
#  5  - moo
#  6  - flick tail


script()

   float pX,pZ

   string mooSound
   mooSound = "sndfx/animal/mudchuteanimals/Mudchute_cow_1.ogg"

   int diceValue

   # The infinite while-the-application-is-running loop
   # Just remember: when in battle mode, this script won't be called,
   #                but the one defined for battle behaviour;
   while(true)

      # The cattle actions is a finite state machine.
      # Let's random it!
      diceValue = rollDice(10)

      if(diceValue == 1)
         # Stand state (10% chance)
         setAnimation(SELF_CHARACTER, 0)
         # Stand for 5 seconds
         wait(5)
      else if(diceValue <= 3)
         # Moo (20%)
         showTextStatic(SELF_CHARACTER, "Mooo")
         # Only need one cycle of Moo animation (no repeats)
         callAnimation(SELF_CHARACTER, 8)
         # Play moo sound!
         playSound(mooSound, OWNER_POSX, OWNER_HEIGHT, OWNER_POSZ)
         # Wait 3 seconds before take another action
         wait(3)
      else if(diceValue <= 6)
         # Eat state (30%)
         setAnimation(SELF_CHARACTER, 6)
         # Eat for 10 seconds
         wait(10)
      else if(diceValue == 8)
         # Look state (10%)
         setAnimation(SELF_CHARACTER, 5)
         wait(3)
      else if(diceValue <= 9)
         # Tail flick (20%)
         callAnimation(SELF_CHARACTER, 9)
         wait(3)
      else if(diceValue == 10)
         # Walk (10%)
         # Get a random position 
         # Note that rolDice(3)-2 will return -1, 0 or 1
         pX = OWNER_POSX + (rollDice(3)-2)*rollDice(80)
         pZ = OWNER_POSZ + (rollDice(3)-2)*rollDice(80)
         moveToPosition(SELF_CHARACTER, pX, pZ)
      end
   end

end

