# TUTORIAL - Old Factory Computer
# Note: this mission is controlled by this script and
# compdeck and logan dialogs.

script()

   int state

   # define base's map
   string base
   base = "mapas/tutorial/base.map"
   
   # define factory's computer name
   string compdeck
   compdeck = "models/objetos/industrial/compdeck/compdeck.dcc"

   # Define Factory's Computer Position
   # 356.864,0.000,511.227
   float posX, posY, posZ
   posX = 356.864
   posY = 0.000
   posZ = 511.227

   # define map where logan is
   string mapa
   mapa = "mapas/tutorial/house1.map"

   # define logan character
   string logan
   logan = "characters/pcs/logan.pc"

   # Main loop
   while(true)

      if(ACTUAL_MAP == base)
         # Check the state of the old factory computer
         state = getObjectState(getObject(compdeck, posX, posY, posZ))
         if(state == 1)
           dialogSetInitial(logan, mapa, 19)
           missionSetXp(SELF_MISSION, 200)
           missionComplete(SELF_MISSION, 1)
         end

         # Wait some time, before the next check
         wait(2)
      else
         wait(60)
      end

   end

   
end

