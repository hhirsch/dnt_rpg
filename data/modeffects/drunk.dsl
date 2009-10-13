
script(character target, int difficulty, int drunkLevel)

   string class;
   string fort;

   int modValue, time, periodicTime

   class = "PHILOSOPHY"
   fort = "FORTITUDE"

   # try the fortitude save
   if(!roll(target, fort, difficulty)) 

      # couldn't save, affected!
      if(classLevel(target, class) >= 1)
         # TODO Some philosophy level, must do the specific class things
      else
         # Non-philosophy, calculate the effect
         periodicTime = 0
         modValue = (-1)*(rollDice(drunkLevel) + 2)
         time = (rollDice(drunkLevel)+2) * 200

         # and add it!
         #addEffect(target, 
      end


   end

end

