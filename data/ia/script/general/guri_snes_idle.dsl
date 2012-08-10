# This script controlls child behaviour

script()

   float pX, pZ
   int val
   int snesMsg
   string msg

   # define kids characters, for special messages
   character matilde 
   matilde = getNPCByName("characters/pcs/matilde.pc")
   character ethan
   ethan = getNPCByName("characters/pcs/ethan.pc")
   character billy
   billy = getNPCByName("characters/pcs/billy.pc")

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
            snesMsg = rollDice(4)
            if(snesMsg == 1)
               # Question about where they are
               msg = gettext("Where are these cartridges?")
            else if(snesMsg == 2)
               # The real desire of the kids
               if(SELF_CHARACTER == matilde)
                  msg = gettext("Mom would be proud of me")
               else if(SELF_CHARACTER == billy)
                  msg = gettext("I'll control the game world")
               else if(SELF_CHARACTER == ethan)
                  msg = gettext("The world must be freed of games")
               end
            else if(snesMsg == 3)
               # What they feel about it
               if(SELF_CHARACTER == matilde)
                  msg = gettext("Can't wait for everyone be pleased with them")
               else if(SELF_CHARACTER == billy)
                  msg = gettext("I'm sure there will be lots of interested buyers")
               else if(SELF_CHARACTER == ethan)
                  msg = gettext("They are so hard and unfair")
               end
            else
               # What they desire
               if(SELF_CHARACTER == matilde)
                  msg = gettext("I need to share them")
               else if(SELF_CHARACTER == billy)
                  msg = gettext("I need to add protection to them all")
               else if(SELF_CHARACTER == ethan)
                  msg = gettext("I need to destroy them all")
               end
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


