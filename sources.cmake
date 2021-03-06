# This file is the list of sources needed for dnt, dntMapEditor and 
# dntPartEditor executables

########################################################################
# Files related to FARSO library (DNT's internal GUI)
########################################################################
set(FARSO_SOURCES
src/gui/button.cpp
src/gui/color.cpp
src/gui/cxsel.cpp
src/gui/dntfont.cpp
src/gui/draw.cpp
src/gui/farso.cpp
src/gui/farsoopts.cpp
src/gui/filesel.cpp
src/gui/guilist.cpp
src/gui/healthBar.cpp
src/gui/interface.cpp
src/gui/listtext.cpp
src/gui/menu.cpp
src/gui/messages.cpp
src/gui/mouse.cpp
src/gui/picture.cpp
src/gui/rolbar.cpp
src/gui/seltext.cpp
src/gui/tabbox.cpp
src/gui/tabbutton.cpp
src/gui/textbar.cpp
src/gui/textbox.cpp
src/gui/window.cpp
)

set(FARSO_HEADERS
src/gui/button.h
src/gui/color.h
src/gui/cxsel.h
src/gui/dntfont.h
src/gui/draw.h
src/gui/farso.h
src/gui/farsoopts.h
src/gui/filesel.h
src/gui/guilist.h
src/gui/guiobject.h
src/gui/healthBar.h
src/gui/interface.h
src/gui/listtext.h
src/gui/menu.h
src/gui/messages.h
src/gui/mouse.h
src/gui/picture.h
src/gui/rolbar.h
src/gui/seltext.h
src/gui/tabbox.h
src/gui/tabbutton.h
src/gui/textbar.h
src/gui/textbox.h
src/gui/window.h
)

########################################################################
# Files related to lang directory
########################################################################
set(LANG_SOURCES
src/lang/translate.cpp
)
set(LANG_HEADERS
src/lang/translate.h
)

########################################################################
# Files related to cbook directory
########################################################################
set(CBOOK_SOURCES
src/cbook/comicbook.cpp
src/cbook/comicbox.cpp
src/cbook/comicpage.cpp
)
set(CBOOK_HEADERS
src/cbook/comicbook.h
src/cbook/comicbox.h
src/cbook/comicpage.h
)

########################################################################
# Files related to classes directory
########################################################################
set(CLASSES_SOURCES
src/classes/actions.cpp
src/classes/align.cpp
src/classes/barter.cpp
src/classes/classes.cpp
src/classes/defs.cpp
src/classes/dices.cpp
src/classes/feats.cpp
src/classes/inventory.cpp
src/classes/itemslot.cpp
src/classes/mission.cpp
src/classes/modeffect.cpp
src/classes/money.cpp
src/classes/object.cpp
src/classes/pendingaction.cpp
src/classes/race.cpp
src/classes/skills.cpp
src/classes/thing.cpp
src/classes/weapon.cpp
src/classes/xp.cpp
)
set(CLASSES_HEADERS
src/classes/actions.h
src/classes/align.h
src/classes/barter.h
src/classes/classes.h
src/classes/defs.h
src/classes/dices.h
src/classes/feats.h
src/classes/inventory.h
src/classes/itemslot.h
src/classes/mission.h
src/classes/modeffect.h
src/classes/modifier.h
src/classes/money.h
src/classes/object.h
src/classes/pendingaction.h
src/classes/race.h
src/classes/skills.h
src/classes/thing.h
src/classes/weapon.h
src/classes/xp.h
)

########################################################################
# Files related to etc directory
########################################################################
set(ETC_SOURCES
src/etc/animodel.cpp
src/etc/boundingbox.cpp
src/etc/btreecell.cpp
src/etc/btree.cpp
src/etc/cal3dmodel.cpp
src/etc/defparser.cpp
src/etc/dirs.cpp
src/etc/distance.cpp
src/etc/extensions.cpp
src/etc/list.cpp
src/etc/md5anim.cpp
src/etc/md5model.cpp
src/etc/message3d.cpp
src/etc/npcfile.cpp
src/etc/overlaps.cpp
src/etc/quaternion.cpp
src/etc/scene.cpp
src/etc/scenenode.cpp
src/etc/shader.cpp
src/etc/texrenderer.cpp
src/etc/userinfo.cpp
)
set(ETC_HEADERS
src/etc/animodel.h
src/etc/boundingbox.h
src/etc/btreecell.h
src/etc/btree.h
src/etc/cal3dmodel.h
src/etc/defparser.h
src/etc/dirs.h
src/etc/distance.h
src/etc/extensions.h
src/etc/list.h
src/etc/md5anim.h
src/etc/md5model.h
src/etc/message3d.h
src/etc/npcfile.h
src/etc/overlaps.h
src/etc/quaternion.h
src/etc/scene.h
src/etc/scenenode.h
src/etc/shader.h
src/etc/texrenderer.h
src/etc/userinfo.h
)

set(FIGHT_SOURCES
src/fight/fightGroup.cpp
src/fight/fightSystem.cpp
src/fight/initiative.cpp
)
set(FIGHT_HEADERS
src/fight/fightGroup.h
src/fight/fightSystem.h
src/fight/initiative.h
)

set(IA_SOURCES
src/ia/agent.cpp
src/ia/astar.cpp
src/ia/fuzzy.cpp
src/ia/fuzzyfunction.cpp
src/ia/fuzzyrule.cpp
src/ia/fuzzyvariable.cpp
src/ia/iafuncs.cpp
src/ia/iascript.cpp
src/ia/iastack.cpp
src/ia/iavariable.cpp
src/ia/pattAgent.cpp
src/ia/potentAgent.cpp
)
set(IA_HEADERS
src/ia/agent.h
src/ia/astar.h
src/ia/fuzzyfunction.h
src/ia/fuzzy.h
src/ia/fuzzyrule.h
src/ia/fuzzyvariable.h
src/ia/iafuncs.h
src/ia/iascript.h
src/ia/iastack.h
src/ia/iavariable.h
src/ia/pattAgent.h
src/ia/potentAgent.h
)

set(MAP_SOURCES
src/map/door.cpp
src/map/map.cpp
src/map/mapfog.cpp
src/map/maplights.cpp
src/map/mapsound.cpp
)
set(MAP_HEADERS
src/map/door.h
src/map/mapfog.h
src/map/map.h
src/map/maplights.h
src/map/mapsound.h
)

set(PARTICLE_SOURCES
src/particle/interplane.cpp
src/particle/lake.cpp
src/particle/partcontroller.cpp
src/particle/partelement.cpp
src/particle/particle.cpp
src/particle/partlist.cpp
src/particle/partorigin.cpp
)
set(PARTICLE_HEADERS
src/particle/interplane.h
src/particle/lake.h
src/particle/partcontroller.h
src/particle/partelement.h
src/particle/particle.h
src/particle/partlist.h
src/particle/partorigin.h
)

set(SOUND_SOURCES
src/sound/ogg_stream.cpp
src/sound/sndfx.cpp
src/sound/sound.cpp
)
set(SOUND_HEADERS
src/sound/ogg_stream.h
src/sound/sndfx.h
src/sound/sound.h
)

set(ENGINE_SOURCES
src/engine/alignwindow.cpp
src/engine/aspectwindow.cpp
src/engine/attwindow.cpp
src/engine/barterwindow.cpp
src/engine/briefing.cpp
src/engine/camera.cpp
src/engine/campaign.cpp
src/engine/character.cpp
src/engine/charwindow.cpp
src/engine/classwindow.cpp
src/engine/collision.cpp
src/engine/culling.cpp
src/engine/cursor.cpp
src/engine/dccnit.cpp
src/engine/dialog.cpp
src/engine/featsel.cpp
src/engine/featswindow.cpp
src/engine/initial.cpp
src/engine/inventwindow.cpp
src/engine/itemwindow.cpp
src/engine/journalwindow.cpp
src/engine/levelup.cpp
src/engine/minimapwindow.cpp
src/engine/modstate.cpp
src/engine/moneywindow.cpp
src/engine/options.cpp
src/engine/racewindow.cpp
src/engine/savefile.cpp
src/engine/savewindow.cpp
src/engine/shadow.cpp
src/engine/shortcutswindow.cpp
src/engine/skillwindow.cpp
src/engine/sky.cpp
src/engine/sun.cpp
src/engine/util.cpp
)
set(ENGINE_HEADERS
src/engine/alignwindow.h
src/engine/aspectwindow.h
src/engine/attwindow.h
src/engine/barterwindow.h
src/engine/briefing.h
src/engine/camera.h
src/engine/campaign.h
src/engine/character.h
src/engine/charwindow.h
src/engine/classwindow.h
src/engine/collision.h
src/engine/culling.h
src/engine/cursor.h
src/engine/dccnit.h
src/engine/dialog.h
src/engine/featsel.h
src/engine/featswindow.h
src/engine/initial.h
src/engine/inventwindow.h
src/engine/itemwindow.h
src/engine/journalwindow.h
src/engine/levelup.h
src/engine/minimapwindow.h
src/engine/modstate.h
src/engine/moneywindow.h
src/engine/options.h
src/engine/racewindow.h
src/engine/savefile.h
src/engine/savewindow.h
src/engine/shadow.h
src/engine/shortcutswindow.h
src/engine/skillwindow.h
src/engine/sky.h
src/engine/sun.h
src/engine/util.h
)

IF(${APPLE})
   set(APPLE_CONFIG_SOURCES
       src/config_mac.cpp
       macosx/SDLMain.m)
   set(APPLE_CONFIG_HEADERS
       macosx/SDLMain.h)
ENDIF(${APPLE})

IF(${WIN32} OR ${MINGW})
   set(WIN_SOURCES
       windows/resource.rc)
ENDIF(${WIN32} OR ${MINGW})

set(DNT_LIB_HEADERS
${CBOOK_HEADERS}
${LANG_HEADERS}
${CLASSES_HEADERS}
${ETC_HEADERS}
${FIGHT_HEADERS}
${IA_HEADERS}
${MAP_HEADERS}
${PARTICLE_HEADERS}
${SOUND_HEADERS}
${ENGINE_HEADERS}
${APPLE_CONFIG_HEADERS}
)

set(DNT_LIB_SOURCES
${CBOOK_SOURCES}
${LANG_SOURCES}
${CLASSES_SOURCES}
${ETC_SOURCES}
${FIGHT_SOURCES}
${IA_SOURCES}
${MAP_SOURCES}
${PARTICLE_SOURCES}
${SOUND_SOURCES}
${ENGINE_SOURCES}
${APPLE_CONFIG_SOURCES}
)

set(DNT_SOURCES
src/engine/teste.cpp
${WIN_SOURCES}
)
set(DNT_HEADERS
)

set(DNT_MAP_EDITOR_SOURCES
src/app/mapEditor/area.cpp
src/app/mapEditor/editor.cpp
src/app/mapEditor/grasswindow.cpp
src/app/mapEditor/guiIO.cpp
src/app/mapEditor/listwindow.cpp
src/app/mapEditor/mapEditor.cpp
src/app/mapEditor/message.cpp
src/app/mapEditor/nodeeditor.cpp
src/app/mapEditor/npcs.cpp
src/app/mapEditor/objects.cpp
src/app/mapEditor/particles.cpp
src/app/mapEditor/portal.cpp
src/app/mapEditor/soundeditor.cpp
src/app/mapEditor/terrain.cpp
src/app/mapEditor/tilewall.cpp
src/app/mapEditor/wall.cpp
src/app/mapEditor/waterwindow.cpp
)
set(DNT_MAP_EDITOR_HEADERS
src/app/mapEditor/area.h
src/app/mapEditor/editor.h
src/app/mapEditor/grasswindow.h
src/app/mapEditor/guiIO.h
src/app/mapEditor/listwindow.h
src/app/mapEditor/mapEditor.h
src/app/mapEditor/message.h
src/app/mapEditor/nodeeditor.h
src/app/mapEditor/npcs.h
src/app/mapEditor/objects.h
src/app/mapEditor/particles.h
src/app/mapEditor/portal.h
src/app/mapEditor/soundeditor.h
src/app/mapEditor/terrain.h
src/app/mapEditor/tilewall.h
src/app/mapEditor/wall.h
src/app/mapEditor/waterwindow.h
)

set(DNT_PART_EDITOR_SOURCES
src/app/partEditor/editor.cpp
src/app/partEditor/main.cpp
src/app/partEditor/partaux.cpp
src/app/partEditor/partelementwindow.cpp
src/app/partEditor/partoriginwindow.cpp
src/app/partEditor/partwindow.cpp
)

set(DNT_PART_EDITOR_HEADERS
src/app/partEditor/editor.h
src/app/partEditor/partaux.h
src/app/partEditor/partelementwindow.h
src/app/partEditor/partoriginwindow.h
src/app/partEditor/partwindow.h
)

