# A general movimentation script to test
# the NPC movimentation controlled by scripts.

script()
   bool control
   int fuzzyHungry
   int posX posZ

   control = true
   fuzzyHungry = 0
   while(true)
      if(control)
         posX = 200
         posZ = 120
         moveToPosition(SELF_CHARACTER, posX, posZ)
         posX = 30
         posZ = 295
         moveToPosition(SELF_CHARACTER, posX, posZ)
         fuzzyHungry = fuzzyHungry + 1
         if(fuzzyHungry >= 1)
            control = false
            fuzzyHungry = 2
         end
      else
         setIdle(SELF_CHARACTER)
         fuzzyHungry = fuzzyHungry - 1
         if(fuzzyHungry < 0)
            control = true
         end
      end
   end
end

