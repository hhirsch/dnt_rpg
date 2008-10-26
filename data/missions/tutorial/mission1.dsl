# TUTORIAL - I
# The first tutorial mission
# - Found a Chrono Trigger Game for Logan
# NOTE: this mission is controlled between this script and
#       Logan's tutorial dialog.

script()

   int waitTime
   waitTime = 2

   # create the object fileName, since can't have a literal parameter
   string objFile
   objFile = "models/objetos/icex/snes/chrono/snes_chrono.dcc"

   # define logan character
   string logan
   logan = "characters/pcs/logan.pc"   

   # define map where logan is
   string mapa
   mapa = "mapas/tutorial/house1.map"

   # the dialog number
   int dialog
   dialog = 6
   
   while(true)

      if(inventoryHave(ACTIVE_CHARACTER,objFile)) 
         if(dialog == 6)
            # Have the cartridge, set logan talk to get it!
            dialog = 7
            dialogSetInitial(logan, mapa, dialog)
         end   
      else 
         if(dialog != 6)
            # Haven't the cartridge, set the logan talk to wait.
            dialog = 6
            dialogSetInitial(logan, mapa, dialog)
         else
            # Verify if logan has the cartdrige: if you barteres with him
            if(inventoryHave(getNPCByName(logan),objFile))
               # Set the dialog to next one
               dialog = 9
               dialogSetInitial(logan, mapa, dialog)

               # You've bartered the cartridge with logan, so finish Mission
               int xp
               int type
               xp = 180
               type = 3
               missionSetXp(SELF_MISSION, xp)
               missionComplete(SELF_MISSION, type)
            end

         end   
      end

      # Wait some time, before the next check
      # FIXME: accept pendingActions at mission scripts!!
      #        (for now it is always deleted on map changes!)
      #wait(waitTime)

   end

end

