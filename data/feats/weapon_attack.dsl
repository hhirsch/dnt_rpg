# weapon_attack.dsl : This script implements the basic attack
# (unarmed or with the equiped weapon).

script(character target)

   dice d
   int curAmmo
   weapon curWeapon
   string concept = "STRENGHT"

   # Verify if is an weapon attack or unarmed
   if(weaponEquiped(SELF_CHARACTER) == true)

      # With an weapon. let's see if it uses ammo or not
      curWeapon = getEquipedWeapon(SELF_CHARACTER)

      if(weaponGetAmmoType(curWeapon) != "none")
         # Verify if have ammo to use
         if(weaponGetAmmo(curWeapon) <= 0)
            briefingColor(gettext("Out of ammo!"), 255, 10, 10)
            exit()
         end
      end

      # Verify if is ranged (ranged == use DEXTERITY)
      if(weaponGetRangeType(curWeapon) == "Ranged")
         concept = "DEXTERITY"
      end

      # Get weapon's dice
      d = weaponGetDice(curWeapon)

      # Do the attack
      if(doAttack(SELF_CHARACTER, target, d, concept))
         # Apply the ammo - 1, if needed
         if(weaponGetAmmoType(curWeapon) != "none")
            weaponDecAmmo(curWeapon)

            # Show a message if no more ammo, to allow pc reload
            briefing(gettext("No more ammo. Reload your weapon!"))
         end
      end

   else
      # Unarmed attack, get the bared hands dice
      d = getBaredHandsDice(SELF_CHARACTER)

      # Do the attack
      doAttack(SELF_CHARACTER, target, d, concept)
   end

end


