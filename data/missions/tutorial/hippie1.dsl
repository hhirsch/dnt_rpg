# Hippie first mission
# - Get some psychedelic ration for F.S.C. Hippie

script()

   int waitTime
   waitTime = 2

   # create the object fileName, since can't have a literal parameter
   string objFile
   objFile = "models/objetos/itens/racao/racao.dcc"

   # define hippie character
   string hippie
   hippie = "characters/pcs/hippie.pc"

   # define dona character
   string dona
   dona = "characters/pcs/dona.pc"

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
               # Have the cartridge, set hippie talk to get it!
               dialog = 11
               dialogSetInitial(hippie, mapa, dialog)
            end
         else 
            if(dialog != 8)
               # Haven't the ration, set the talk to wait.
               dialog = 8
               dialogSetInitial(hippie, mapa, dialog)
            else
               # Verify if hippie has the ration: if you bartered with him
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
      else if(ACTUAL_MAP == "mapas/tutorial/faz_inner.map")
         # Verify if dona is still alive (if dead, mission failed!)
         if(!isAlive(getNPCByName(dona)))
            dialogSetInitial(hippie, mapa, 13)
            missionAbort(SELF_MISSION)
         end
      end

      # Wait some time, before the next check
      wait(waitTime)

   end

end

