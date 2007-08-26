# A general movimentation script to test
# the NPC movimentation controlled by scripts.

script()

   bool control
   int posX posZ
   control = true
   while(control)
      posX = 2000
      posZ = 2000
      moveToPosition(SELF_CHARACTER, posX, posZ)
      posX = 2200
      posZ = 2020
      moveToPosition(SELF_CHARACTER, posX, posZ)
   end

end

