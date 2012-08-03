# This script controlls child behaviour

script()

   float pX, pZ
   int val
   int snesMsg
   string msg

   while(true)

      # Verify if active character is near
      if(characterAtRange(SELF_CHARACTER, ACTIVE_CHARACTER))
        
        # If active character is a child's killer, must run away!
        if(haveFeat(ACTIVE_CHARACTER, "MJ_FRIENDS_KILLER"))
           showTextStatic(SELF_CHARACTER, "Aaaaahhhh")
           moveFromCharacter(SELF_CHARACTER, ACTIVE_CHARACTER)
        end
        wait(10)
      else
         val = rollDice(20)

         if(val <= 5)
            # 25% chance of walk 
            pX = OWNER_POSX + (rollDice(3)-2)*rollDice(128)
            pZ = OWNER_POSZ + (rollDice(3)-2)*rollDice(128)

            # Show a message related to the cartridges search
            snesMsg = rollDice(3)
            if(snesMsg == 1)
               msg = gettext("Where are these cartridges?")
            else if(snesMsg == 2)
               msg = gettext("Mom would be proud of me")
            else
               msg = gettext("I need to share them")
            end
            showTextStatic(SELF_CHARACTER, msg)

            # Try to walk.
            moveToPosition(SELF_CHARACTER, pX, pZ)

            wait(4)
         end

      end

      wait(2)
   end


end


