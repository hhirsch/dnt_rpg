# TUTORIAL - II
# - Kill creatures at subterrain bunker school
# NOTE: this mission is mainly  controlled by
#       Logan's tutorial dialog.

script()

   # Some sleep time
   int waitTime
   
   # define map where logan is
   string mapa
   mapa = "mapas/tutorial/house1.map"

   # define logan character
   string logan
   logan = "characters/pcs/logan.pc"

   while(true)

      if(ACTUAL_MAP == mapa)
         # Verify if logan is still alive (if dead, mission failed!)
         if(!isAlive(getNPCByName(logan)))
            missionAbort(SELF_MISSION)
         end

         # Quick sleep
         waitTime = 20
      else
         # Sleep for a minute
         waitTime = 60
      end

      # Sleep for some time
      wait(waitTime)

   end

end

