# Hippie first mission
# - Get some psychedelic ration for F.S.C. Hippie

script()

   int waitTime
   waitTime = 2

   # create the object fileName, since can't have a literal parameter
   string objFile
   objFile = "models/objetos/itens/ration/ration.dcc"

   # define hippie character
   string hippie
   hippie = "characters/pcs/hippie.pc"

   # define map where hippie is
   string mapa
   mapa = "mapas/tutorial/barraco2.map"

   int dialog
   dialog = 8

   while(true)

      if(ACTUAL_MAP == mapa)
         
         # Verify if hippie is still alive (if dead, mission failed!)
         if(!isAlive(getNPCByName(hippie)))
            missionAbort(SELF_MISSION)
         end

         if(inventoryHave(ACTIVE_CHARACTER, objFile)) 
            if(dialog == 8)
               # Have the cartridge, set logan talk to get it!
               dialog = 11
               dialogSetInitial(logan, mapa, dialog)            
            end
         else 
            if(dialog != 8)
               # Haven't the cartridge, set the logan talk to wait.
               dialog = 8
               dialogSetInitial(logan, mapa, dialog)
            else
               # Verify if hippie has the cartdrige: if you bartered with him
               if(inventoryHave(getNPCByName(hippie),objFile))
                  # Set the dialog to next one
                  dialog = 12
                  dialogSetInitial(hippie, mapa, dialog)
  
                  # You've bartered the ration so finish Mission
                  missionSetXp(SELF_MISSION, 200)
                  missionComplete(SELF_MISSION, 3)
               end

            end   
         end
      end

      # Wait some time, before the next check
      wait(waitTime)

   end

end

