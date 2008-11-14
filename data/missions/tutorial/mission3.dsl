# TUTORIAL - III
# - Install a ruindows at Ocupation's notebook (or a linux at
# Santa Claws' notebook)
# NOTE: this mission is mainly controlled by Santa dialog and this script

script()

   # Some sleep time
   int waitTime
   waitTime = 5
   
   # define occupation's map
   string occupation
   occupation = "mapas/tutorial/house1.map"

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
   string notebook
   notebook = "models/objetos/geral/notebook/notebook.dcc"

   # Define Santa's Notebook Position
   float sPosX, sPosY, sPosZ
   sPosX = 2185.18
   sPosY = 17.90 
   sPosZ = 3571.00

   # Define Occupation's Notebook Position
   float oPosX, oPosY, oPosZ
   oPosX = 349.11
   oPosY = 17.39
   oPosZ = 383.82

   # The state and the dialog variables
   int state, dialog;

   # The controlller if mission is possible or not
   bool cantOccupation, cantSanta
   cantOccupation = false
   cantSanta = false

   while(true)

      # Tyrol Verifications
      if(ACTUAL_MAP == tyrol)

         # Verify if santa is still alive (if dead, mission failed!)
         if(!isAlive(getNPCByName(santa)))
            missionAbort(SELF_MISSION)
         end

         # Get notebook status
         state = getObjectState(getObject(notebook, sPosX, sPosY, sPosZ))

         # Verify if Santa's notebook is with linux
         if( (state == 2) || (state == 3) )
           dialog = 10
           dialogSetInitial(santa, tyrol, dialog)
         else if( (state == 4) || (state == 5) )
            cantSanta = true
         end

      # Occupation Verifications
      else if(ACTUAL_MAP == occupation)

         # Get notebook status
         state = getObjectState(getObject(notebook, oPosX, oPosY, oPosZ))

         # Verify if Ocupation's notebook is with Ruindows
         if( (state == 4) || (state == 5) )
           dialog = 9
           dialogSetInitial(santa, tyrol, dialog)
         else if( (state == 3) || (state == 2) )
            cantOccupation = true
         end

      end

      # Verify if mission is still possible
      if( (cantOccupation) && (cantSanta) )
         # Impossible, so must abort!
         dialog = 7
         dialogSetInitial(santa, tyrol, dialog)
         missionAbort(SELF_MISSION)
      end

      # Sleep for some time
      wait(waitTime)
   end

end

