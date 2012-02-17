
# The cattle_idle script implements a simple behaviour for cattle actions when
# they are idle on a map;

# Cattle States (actions):
#  2  - sit
#  3  - eat
#  3  - eat & flick tail
#  1  - stand
#  1  - stand & flick tail
#  4  - walk to another position
#  5  - moo

script()

   int state = 1  # initially, they are standed;

   float pX = 0
   float pZ = 0

   int diceValue = 0
   int flickTail = 0

   # The infinite while-the-application-is-running loop
   # Just remember: when in battle mode, this script won't be called,
   #                but the one defined for battle behaviour;
   while(true)

      # The cattle actions is a finite state machine.
      # Let's random it!
      diceValue = rollDice(10)

      if(diceValue <= 2)
         # Stand state (20% chance)
         print("Stand")
         state = 1
         setAnimation(SELF_CHARACTER, 0)
         # Verify if will flickTail
         #flickTail = rollDice(2)
         #if(flickTail == 1)
         #   callAnimation(SELF_CHARACTER, 8)
         #end
         # Stand for 5 seconds
         wait(5)
      else if(diceValue == 3)
         # Moo!
         print("Moo!")
         state = 5
         # Only need one cycle of Moo animation (no repeats)
         callAnimation(SELF_CHARACTER, 7)
         # TODO: call moo sound!
         # Wait 2 seconds before take anoter action
         wait(2)
      else if( (diceValue > 4) && (diceValue <= 6) )
         # Eat state (30%)
         print("Eat")
         state = 3
         setAnimation(SELF_CHARACTER, 5)
         # Verify if will flickTail
         #flickTail = rollDice(2)
         #if(flickTail == 1)
         #   callAnimation(SELF_CHARACTER, 8)
         #end
         # Eat for 10 seconds
         wait(10)
      else if( (diceValue > 6) && (diceValue <= 9) )
         # Sit state (30%)
         print("Sit!")
         state = 2
         setAnimation(SELF_CHARACTER, 6)
         wait(4)
      else if(diceValue == 10)
         # Walk (10%)
         print("Walk!!\n")
         state = 4
         # Get a random position
         #multFact = rollDice(2)
         #multFact = (-1)*(multFact - 1)
         #pX = rollDice(30)
         #pX = OWNER_POSX + pX
         #pZ = OWNER_POSZ + (rollDice(30)*(-1*(rollDice(2)-1)))
         #moveToPosition(SELF_CHARACTER, pX, pZ)
      end
   end

end

