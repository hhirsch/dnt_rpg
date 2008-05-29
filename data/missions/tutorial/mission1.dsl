# TUTORIAL - I
# The first tutorial mission
# - Found a Chrono Trigger Game for Logan

script()

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

      if( (inventoryHave(ACTIVE_CHARACTER,objFile)) && (dialog == 6) )
         # Have the cartridge, set logan talk to get it!
         dialog = 7
         dialogSetInitial(logan, mapa, dialog)
      else if(dialog != 6)
         # Have't the cartridge, set the logan talk to wait.
         dialog = 6
         dialogSetInitial(logan, mapa, dialog)
      end

   end

end

