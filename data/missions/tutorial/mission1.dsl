# TUTORIAL - I
# The first tutorial mission

script()

   # create the object fileName, since can't have a literal parameter
   string objFile
   objFile = "../data/weapons/definitions/beretta92.wcc"

   #create the mission filename too
   string missionFile
   missionFile = "../data/missions/tutorial/mission1.dsl"

   while(true)
      if(inventoryHave(ACTIVE_CHARACTER,objFile))
         int compType
         compType = 1
         missionComplete(missionFile, compType)
      end
   end

end

