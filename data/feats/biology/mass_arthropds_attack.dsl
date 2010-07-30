# Mass Arthropds Attack Talent
# Biology Student 10th level
script(float x, float y, float z)

   dice damage
   int numDices
   character target

   # Set Dices
   setBaseDice(damage, "2*d8+0")
   numDices = classLevel(SELF_CHARACTER, "BIOLOGY") - 10
   setAditionalDice(damage, numDices, "1*d6+0")

   # Do the attack to all characters in area
   target = getCharacterInArea(target, x, y, z, 30)
   while(validCharacter(target)
      if(target != SELF_CHARACTER)
         doAttack(SELF_CHARACTER, target, damage, "ATTRIBUTE", "CHARISMA")
      end
      target = getCharacterInArea(target, x, y, z, 30)
   end
end

