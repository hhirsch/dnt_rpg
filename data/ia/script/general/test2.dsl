# A general movimentation script to test
# the NPC movimentation controlled by scripts.

script()
   bool control
   int posX posZ
   int time

   control = true
   while(true)
      if(control)
         posX = 2325 
         posZ = 2300
         moveToPosition(SELF_CHARACTER, posX, posZ)
         posX = 2100
         posZ = 2359
         moveToPosition(SELF_CHARACTER, posX, posZ)
         control = false
      else
         setIdle(SELF_CHARACTER)
         time = 10
         wait(time)
         posX = 2225 
         posZ = 2340
         moveToPosition(SELF_CHARACTER, posX, posZ)
         posX = 2275
         posZ = 2246
         moveToPosition(SELF_CHARACTER, posX, posZ)
         control = true
      end
   end
end

