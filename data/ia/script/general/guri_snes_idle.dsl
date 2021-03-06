# This script controlls child behaviour

script()

   float pX, pZ
   int val
   int snesMsg
   string msg
   int cartCompType

   # define the cartridge search mission script
   string cartMission
   cartMission = "missions/tyrol_kids/tyrol_kids_main.dsl"

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

            # Get the cartridge mission completion type
            cartCompType = missionCompletionValue(cartMission)

            if(cartCompType < 0)
               # Mission not completed (or inactive)
               # Show a message related to the cartridges search
               snesMsg = rollDice(4)
               if(snesMsg == 1)
                  # Question about where they are
                  msg = gettext("Where are these cartridges?")
               else if(snesMsg == 2)
                  # The real desire of the kids
                  if(SELF_CHARACTER == matilde)
                     msg = gettext("I'll share them just like mom did!")
                  else if(SELF_CHARACTER == billy)
                     msg = gettext("I'll control the game world!")
                  else if(SELF_CHARACTER == ethan)
                     msg = gettext("The world must be freed of games!")
                  end
               else if(snesMsg == 3)
                  # What they feel about it
                  if(SELF_CHARACTER == matilde)
                     msg = gettext("I can't wait to share them...")
                  else if(SELF_CHARACTER == billy)
                     msg = gettext("I'm sure there will be many interested players...")
                  else if(SELF_CHARACTER == ethan)
                     msg = gettext("They are so hard and so unfair...")
                  end
               else
                  # What they desire
                  if(SELF_CHARACTER == matilde)
                     msg = gettext("Everybody should be allowed to play them...")
                  else if(SELF_CHARACTER == billy)
                     msg = gettext("I need to get the best copy protection...")
                  else if(SELF_CHARACTER == ethan)
                     msg = gettext("I need to destroy them all...")
                  end
               end
            else if(cartCompType == 1)
               # Matilde completed!
               if(SELF_CHARACTER == matilde)
                  msg = gettext("I'm glad I can finally share them!")
               else if(SELF_CHARACTER == billy)
                  msg = gettext("Now they will loose all value...")
               else if(SELF_CHARACTER == ethan)
                  msg = gettext("Real life is lost forever...")
               end
            else if(cartCompType == 2)
               # Billy completed
               if(SELF_CHARACTER == matilde)
                  msg = gettext("Now only the privileged will be allowed to play...")
               else if(SELF_CHARACTER == billy)
                  msg = gettext("Now to find the richest gamers alive!")
               else if(SELF_CHARACTER == ethan)
                  msg = gettext("At least not everybody will suffer from them...")
               end
            else if(cartCompType == 3)
               # Ethan completed
               if(SELF_CHARACTER == matilde)
                  msg = gettext("What a blow for our culture...")
               else if(SELF_CHARACTER == billy)
                  msg = gettext("What a blow for the market...")
               else if(SELF_CHARACTER == ethan)
                  msg = gettext("Finally I can destroy them!")
               end
            end

            # Show the message with specific kid color
            if(SELF_CHARACTER == matilde)
               showTextStatic(SELF_CHARACTER, msg, 0.94, 0.65, 0.87)
            else if(SELF_CHARACTER == billy)
               showTextStatic(SELF_CHARACTER, msg, 0.6, 0.6, 0.98)
            else if(SELF_CHARACTER == ethan)
               showTextStatic(SELF_CHARACTER, msg, 0.53, 0.76, 0.2)
            end

            # Try to walk.
            moveToPosition(SELF_CHARACTER, pX, pZ)

            wait(4)
         end

      end

      wait(2)
   end


end


