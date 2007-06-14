-- module(..., package.seeall);

-- Gets the table containig the transitions of the FSM and organize them
-- into the followings structure:
-- fsm[currentState][event] = {nextState}
-- And also sets the FSM's final state
function create(table, finalState)
  local fsm = {finalState = finalState} -- setting the final state
  for _,v in pairs(table) do
    -- getting each transition of the input
    local old, event, new, args = v[1], v[2], v[3], v[4]
    if fsm[old] == nil then
      fsm[old] = {args = args} -- initialize the table for the current state
    end
    -- set the next state and action for the current state in case
    -- that event happens
    fsm[old][event] = {nextState = new}
  end
  return fsm
end

-- This one "plays" the FSM starting at the first current state given.
-- It will return every single possibility of reaching the final state into
-- the var "path", which is a table.
function play(currentState, fsm)
  if currentState == fsm.finalState then -- if reached the final state, return
    print("Reached the end!!!")
    return 1
  end

  event = cppCalls(currentState, dude, fsm[currentState].args)
  print(event)
    -- calls the play function recursevily for each possible event on the
    -- current state.
  play(fsm[currentState][event].nextState, fsm)
    -- removes the last inserted state, so it can explore the other ways.
 -- end
end
