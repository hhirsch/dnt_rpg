
script(character target, int difficulty, int poisonLevel)

   string class;
   string fort;

   class = "PHILOSOPHY"
   fort = "FORTITUDE"

   # try the fortitude save
   if(!roll(target, fort, difficulty)) 

      # couldn't save, affected!
      if(classLevel(target, class) >= 1)
         # TODO Some philosophy level, must do the specific class things
      end

      # TODO set the effects
      # addEffect(target, 
   end

end

