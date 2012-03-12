#
# The drunk script: Add drunk effects to a character, based on the drunkLevel
#
# Usually called when using a 'narcotic-type' object
#

script(character target, int difficulty, int drunkLevel)

   string class;
   string fort;

   int modValue, time, periodicTime
   string factorId, factorType

   string partFile
   int partType

   class = "PHILOSOPHY"
   fort = "FORTITUDE"

   # try the fortitude save
   if(!roll(target, fort, difficulty)) 

      # couldn't save, affected!
      if(classLevel(target, class) >= 1)
         # TODO Some philosophy level, must do the specific class things
      else
         # Non-philosophy, calculate duration time
         periodicTime = 0
         time = (rollDice(drunkLevel)+2) * 200000

         # add modEffect to inteligence
         factorId = "INTELLIGENCE"
         factorType = "ATTRIBUTE"
         modValue = -1*(rollDice(drunkLevel) + 2)
         addEffect(target, modValue, time, periodicTime, factorType, factorId)

         # add modEffect to balance
         factorId = "BLUFF"
         factorType = "SKILL"
         modValue = (rollDice(drunkLevel))
         addEffect(target, modValue, time, periodicTime, factorType, factorId)

         # add drinking saturation
         factorId = "DRINKING"
         factorType = "SKILL"
         modValue = -1
         addEffect(target, modValue, time, periodicTime, factorType, factorId)
      
         # Add particle effect
         partFile = "particles/drunk.par"
         particleAddToCharacter(partFile, target, time)

         # Add Burps
         showText(target, "Burp... hic!")
      end
   end
end


