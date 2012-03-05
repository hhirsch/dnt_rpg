# This script controlls child behaviour

script()

   float pX, pZ
   int val

   while(true)

      # Verify if active character is near
      if(characterAtRange(SELF_CHARACTER, ACTIVE_CHARACTER))
        
        # If active character is a child's killer, must run away!
        if(haveFeat(ACTIVE_CHARACTER, "MJ_FRIENDS_KILLER"))
           showTextStatic(SELF_CHARACTER, "Aaaaahhhh")
           moveFromCharacter(SELF_CHARACTER, ACTIVE_CHARACTER))
        end
        wait(10)
      else
         # TODO: group children, and follow themselves

         val = rollDice(20)

         if(val <= 5)
            # 25% chance of walk 
            pX = OWNER_POSX + (rollDice(3)-2)*rollDice(128)
            pZ = OWNER_POSZ + (rollDice(3)-2)*rollDice(128)
            moveToPosition(SELF_CHARACTER, pX, pZ)
            wait(4)
         end

      end

      wait(2)
   end


end


