" Vim syntax file
" Language: DNT Script Language
" Maintainer: Guilherme Farrer <farrer@dnteam.org>
" Last Change: 2008 Nov 14

syn clear

" Key Words
syn keyword dntStatement      script end
syn keyword dntConditional    if else
syn keyword dntRepeat         while

" String
syn region dntString         start=+"+   end=+"+
syn match dntNumber          "-\=\<\d\+\>"
syn match dntByte             "\$[0-9a-fA-F]\+\>"

" Comments
syn match   dntComment       "#.*$" contains=dntTodo
syn keyword dntTodo          contained TODO FIXME XXX NOTE


" Operators and Types
syn keyword dntOperator       SELF_OBJECT SELF_CHARACTER ACTIVE_CHARACTER
syn keyword dntOperator       ACTUAL_MAP SELF_MISSION
syn keyword dntOperator       void bool int float string character
syn keyword dntOperator       true false

" Functions
syn keyword dntFunction       wait print 
syn keyword dntFunction       moveToPosition moveToCharacter
syn keyword dntFunction       moveToObject setIdle
syn keyword dntFunction       missionAdd missionComplete missionIsActive
syn keyword dntFunction       missionAbort
syn keyword dntFunction       missionSetTemp missionGetTemp missionSetXp
syn keyword dntFunction       featActualQuantity featCost featQuantityPerDay
syn keyword dntFunction       getRandomAttackFeat getPowerfullAttackFeat
syn keyword dntFunction       getRandomHealFeat getPowerfullHealFeat
syn keyword dntFunction       skillPoints attModifier
syn keyword dntFunction       combatEnter getNearestEnemy
syn keyword dntFunction       getNPCByName isAlive
syn keyword dntFunction       getPsycho setPsycho getActualLife getMaxLife
syn keyword dntFunction       characterAtRange canAttack
syn keyword dntFunction       featUseAtCharacter runAwayFromBattle
syn keyword dntFunction       inventoryHave
syn keyword dntFunction       dialogSetInitial
syn keyword dntFunction       getObject getObjectState
syn keyword dntFunction       isAllDead mapTravel

syn sync lines=250

if !exists("did_dnt_syntax_inits")
   let did_dnt_syntax_inits = 1
   " The default methods for highlighting.  Can be overridden later
   hi link dntStatement       Statement
   hi link dntConditional     Conditional
   hi link dntRepeat          Repeat
   hi link dntString          String
   hi link dntNumber          Number
   hi link dntByte            Number
   hi link dntComment         Comment
   hi link dntTodo            Todo
   hi link dntOperator        Operator
   hi link dntFunction        Function
endif


let b:current_syntax = "dnt"



