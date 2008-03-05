# A general movimentation script to test
# the NPC movimentation controlled by scripts.

script()
   bool control
   int posX posZ
   int time

   control = true
   while(true)
      if(control)
         posX = 3500
         posZ = 3200
         moveToPosition(SELF_CHARACTER, posX, posZ)
         posX = 3600
         posZ = 3200
         moveToPosition(SELF_CHARACTER, posX, posZ)
         control = false
      else
         setIdle(SELF_CHARACTER)
         time = 10
         wait(time)
         control = true
      end
   end
end

