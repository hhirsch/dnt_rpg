# TUTORIAL - IV
# - Fight for Cruzeirense

script()

   # Some sleep time
   int waitTime
   waitTime = 5
   
   # define local map
   string localMap
   localMap = "mapas/tutorial/cruz_local.map"

   # define house map
   string houseMap
   houseMap = "mapas/tutorial/house2.map"

   # define cruzeirense dialog
   string cruz
   cruz = "characters/pcs/cruzeirense.pc"

   # define cruzirense dialog file
   string cruzDialog
   cruzDialog = "dialogs/tutorial/cruzeirense.dlg"

   # Some variables
   int dialog, state, xp

   while(true)
      if( (ACTUAL_MAP == localMap) && (isAllDead(localMap)) )
           
           # Set the new cruzeirense dialog
           dialog = 5
           dialogSetInitial(cruz, houseMap, dialog)

           # Back to the house map
           mapTravel(houseMap)
           
           # Complete the mission
           state = 1
           xp = 150
           missionSetXp(SELF_MISSION, xp)
           missionComplete(SELF_MISSION, state)
      
      end

      wait(waitTime)
   end

end

