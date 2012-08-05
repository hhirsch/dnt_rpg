# TUTORIAL - I
# The first tutorial mission
# - Found a Chrono Trigger Game for Logan
# NOTE: this script is called when completed this mission,
#       setting the Tyrol Kids dialogs to known about it.

script()

   # define kids characters
   string matilde
   matilde = "characters/pcs/matilde.pc"
   string ethan
   matilde = "characters/pcs/ethan.pc"
   string billy
   matilde = "characters/pcs/billy.pc"

   # define tyrol map
   string tyrol
   tyrol = "mapas/tutorial/tyrol.map"

   # Set kids initial dialog to ask for cartridges!
   dialogSetInitial(matilde, tyrol, 1)
   dialogSetInitial(ethan, tyrol, 1)
   dialogSetInitial(billy, tyrol, 1)

end

