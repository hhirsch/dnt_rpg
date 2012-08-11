# This script is the main controller of cartridges search mission

# NOTE: 
#         ------------------------
#         |   Completion Type    |
#         | 1 | Matilde have all |
#         | 2 | Billy have all   |
#         | 3 | Ethan have all   |
#         ------------------------

script()

   # define total cartridges are in game
   int totalCart
   totalCart = 10

   # define tyrol map
   string tyrol
   tyrol = "mapas/tutorial/tyrol.map"

   # define kids characters
   string matilde
   matilde = "characters/pcs/matilde.pc"
   string ethan
   ethan = "characters/pcs/ethan.pc"
   string billy
   billy = "characters/pcs/billy.pc"

   # The character pointers
   character matildePtr
   character billyPtr
   character ethanPtr

   # Set amount of XP the mission will give
   missionSetXp(SELF_MISSION, 250)

   while(true)
      # Verify if we are in tyrol now or not.
      if(ACTUAL_MAP == tyrol)

         # Get kids pointers
         matildePtr = getNPCByName(matilde)
         billyPtr = getNPCByName(billy)
         ethanPtr = getNPCByName(ethan)

         # Verify if all kids are dead
         if( (!isAlive(matildePtr)) && (!isAlive(billyPtr)) && (!isAlive(ethanPtr)) )
            # All characters are dead. Mission failed.
            missionAbort(SELF_MISSION)
         end

         # Verify if Matilde have all cartridges
         if(inventoryCountItemByInfo(matildePtr, "SNES cartridge") == totalCart)
            # Set matilde dialog to complete, billy and ethan to sad about it;
            dialogSetInitial(matilde, tyrol, 7)
            dialogSetInitial(billy, tyrol, 7)
            dialogSetInitial(ethan, tyrol, 7)
            # Complete mission
            missionComplete(SELF_MISSION, 1)
         end
         # Verify if Billy have all cartridges
         if(inventoryCountItemByInfo(billyPtr, "SNES cartridge") == totalCart)
            # Set billy dialog to complete, matilde and ethan to sad about it;
            dialogSetInitial(billy, tyrol, 6)
            dialogSetInitial(matilde, tyrol, 8)
            dialogSetInitial(ethan, tyrol, 8)
            # Complete mission
            missionComplete(SELF_MISSION, 2)
         end
         # Verify if Ethan have all cartridges
         if(inventoryCountItemByInfo(ethanPtr, "SNES cartridge") == totalCart)
            # Set ethan dialog to complete, matilde and billy to sad about it;
            dialogSetInitial(ethan, tyrol, 6)
            dialogSetInitial(matilde, tyrol, 9)
            dialogSetInitial(billy, tyrol, 8)
            # Complete mission
            missionComplete(SELF_MISSION, 3)
         end

         # Must wait a little, to not use all recurses
         wait(2)
      else
         # Not on tyrol, must sleep MUCH.
         wait(10)
      end
   end


end


