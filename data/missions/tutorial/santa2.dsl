# - Install a ruindows at Hippies's notebook 
script()

   # Some sleep time
   int waitTime
   waitTime = 5
   
   # define hippies's map
   string hippie
   hippie = "mapas/tutorial/barraco2.map"

   # define tyrol's map
   string tyrol
   tyrol = "mapas/tutorial/tyrol.map"

   # define santa character file
   string santa
   santa = "characters/pcs/santa.pc"

   # define santa dialog file
   string santaDialog
   santaDialog = "dialogs/tutorial/santa.dlg"

   # define notebook obect name
   string computer
   computer = "models/objetos/geral/computador/computador.dcc"

   # Define Occupation's Notebook Position
   float oPosX, oPosY, oPosZ
   oPosX = 23.988
   oPosY = 16.800
   oPosZ = 20.123

   # The state and the dialog variables
   int state, dialog;

   # The controlller if mission is possible or not
   bool cantHippie
   cantHippie = false

   while(true)

      # Tyrol Verifications
      if(ACTUAL_MAP == tyrol)

         # Verify if santa is still alive (if dead, mission failed!)
         if(!isAlive(getNPCByName(santa)))
            missionAbort(SELF_MISSION)
         end

      # Occupation Verifications
      else if(ACTUAL_MAP == hippie)

         # Get computer status
         state = getObjectState(getObject(computer, oPosX, oPosY, oPosZ))

         # Verify if Ocupation's notebook is with Ruindows
         if( (state == 4) || (state == 5) )
           dialog = 13
           dialogSetInitial(santa, tyrol, dialog)
         else if( (state == 3) || (state == 2) )
            cantHippie = true
         end

      end

      # Verify if mission is still possible
      if(cantHippie)
         # Impossible, so must abort!
         dialog = 7
         dialogSetInitial(santa, tyrol, dialog)
         missionAbort(SELF_MISSION)
      end

      # Sleep for some time
      wait(waitTime)
   end

end

