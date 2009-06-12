#
# This script is used to change the initial dialog of the cruzeirense
#

script()
    # Cruzeirense file
    string cruz
    cruz = "characters/pcs/cruzeirense.pc"

    # Map File
    string mapName
    mapName = "mapas/tutorial/house2.map"

    # The dialog number
    int newDialog
    newDialog = 1

    # Finally, change the dialog
    dialogSetInitial(cruz, mapName, newDialog)
end


