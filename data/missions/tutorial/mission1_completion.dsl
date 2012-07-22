# TUTORIAL - I
# The first tutorial mission
# - Found a Chrono Trigger Game for Logan
# NOTE: this script is called when completed this mission,
#       setting the Tyrol Kids dialogs to known about it.

script()

   # define tyrol kids characters
   string kid1
   kid1 = "characters/pcs/guri.pc"
   string kid2
   kid2 = "characters/pcs/guri2.pc"
   string kid3
   kid3 = "characters/pcs/guria.pc"

   # define tyrol map
   string tyrol
   tyrol = "mapas/tutorial/tyrol.map"

   # Set kids initial dialog to ask for cartridges!
   dialogSetInitial(kid1, tyrol, 1)
   dialogSetInitial(kid2, tyrol, 1)
   dialogSetInitial(kid3, tyrol, 1)

end

