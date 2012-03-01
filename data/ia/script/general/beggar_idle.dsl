
# The beggar_idle script implements a simple behaviour for beggars

script()

   string message


   float pX,pZ

   int diceValue, waitTime

   while(true)
   
      # Wait some random time
      waitTime = 10+rollDice(20)
      wait(waitTime)

      # Verify if the playable character is near.

      # It's near, let's show a beggar message and wait!
      diceValue = rollDice(10)

      if(diceValue == 1)
         message = gettext("Will no-one spare a dime?")
      else if(diceValue == 2)
         message = gettext("My plate is hot but my food is cold")
      else if(diceValue == 3)
         message = gettext("Wish I go on a diet")
      else if(diceValue == 4)
         message = gettext("People drink too much")
      else if(diceValue == 5)
         message = gettext("Touch my food and suffer the consequences")
      else if(diceValue == 6)
         message = gettext("Food for the hungry!")
      else if(diceValue == 7)
         message = gettext("Im starving")
      else if(diceValue == 8)
         message = gettext("Could you spare some coffee?")
      else if(diceValue == 9)
         message = gettext("Nutrition facts are useless")
      else if(diceValue == 10)
         message = gettext("I cook with wine")
      end

      showTextStatic(SELF_CHARACTER, message)

   end

end

