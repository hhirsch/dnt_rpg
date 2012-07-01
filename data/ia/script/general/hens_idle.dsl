
# The hens_idle script implements a simple behaviour for hens actions when
# they are idle on a map;

# Hens States (actions):
#  1  - eat               (40%)
#  2  - walk and clucking (20%)
#  3  - clucking          (30%)
#  4  - hard clucking     (10%)

script()

   float pX,pZ

   int diceValue
   
   string cluckSound1
   string cluckSound2
   string hardCluck1

   cluckSound1 = "sndfx/animal/clucking01.ogg"
   cluckSound2 = "sndfx/animal/clucking02.ogg"
   hardCluck1 = "sndfx/animal/cluck_hard01.ogg"


   # The infinite while-the-application-is-running loop
   while(true)

      # Let's random it!
      diceValue = rollDice(10)

      if(diceValue <= 4)
         # Eating state (40%)
         setAnimation(SELF_CHARACTER, 0)
         wait(8)
      else if(diceValue <= 6)
         # Walk (20%)
         
         # Play sound
         playSound(cluckSound2, OWNER_POSX, OWNER_HEIGHT, OWNER_POSZ)

         # Get a random position 
         # Note that rolDice(3)-2 will return -1, 0 or 1
         pX = OWNER_POSX + (rollDice(3)-2)*rollDice(80)
         pZ = OWNER_POSZ + (rollDice(3)-2)*rollDice(80)
         moveToPosition(SELF_CHARACTER, pX, pZ)
      else if(diceValue <= 9)
         # Cluck! (30%)
         playSound(cluckSound1, OWNER_POSX, OWNER_HEIGHT, OWNER_POSZ)
         wait(5)
      else if(diceValue == 10)
         # Cluck! (10%)
         playSound(hardCluck1, OWNER_POSX, OWNER_HEIGHT, OWNER_POSZ)
         wait(3)
      end
   end

end

