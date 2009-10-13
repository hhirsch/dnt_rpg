
script(character target, int difficulty, int drunkLevel)

   string class;
   string fort;

   int modValue, time, periodicTime
   string factorId, factorType

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
         time = (rollDice(drunkLevel)+2) * 200

         # add modEffect to inteligence
         factorId = "INTELLIGENCE"
         factorType = "ATTRIBUTE"
         modValue = (-1)*(rollDice(drunkLevel) + 2)
         addEffect(target, modValue, time, periodicTime, factorId, factorType)

         # add modEffect to balance
         factorId = "BALANCE"
         factorType = "SKILL"
         modValue = (-1)*(rollDice(drunkLevel)
         addEffect(target, modValue, time, periodicTime, factorId, factorType)

         # add drinking saturation
         factorId = "DRINKING"
         factorType = "SKILL"
         modValue = -1
         addEffect(target, modValue, time, periodicTime, factorId, factorType)
      end

   end

end

