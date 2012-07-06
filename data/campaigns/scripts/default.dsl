# The initial script of default campaign

script()

   character logan
   logan = getNPCByName("characters/pcs/logan.pc")

   moveToPosition(logan, 208, 446)

   openCharacterDialog(logan)
end

