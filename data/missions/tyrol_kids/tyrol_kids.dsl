# This script is used by tyrol kids to get all SNES cartridges from
# player's inventory, changing it by money.


script()
   
   float totalValue
   string curCart
   float t

   # Let's get all cartridges
   curCart = inventoryGetItemByInfo(ACTIVE_CHARACTER, "SNES cartridge")
   totalValue = 0

   while(curCart != "")
      # Sum total value
      totalValue = totalValue + inventoryGetItemValue(ACTIVE_CHARACTER, curCart)
      # Remove it from player's inventory
      inventoryGiveItem(ACTIVE_CHARACTER, SELF_CHARACTER, curCart)

      # Get next cartridge
      curCart = inventoryGetItemByInfo(ACTIVE_CHARACTER, "SNES cartridge")
   end

   # Finaly, give the ammount quantity to the player
   inventoryAddMoney(ACTIVE_CHARACTER, totalValue)
end

