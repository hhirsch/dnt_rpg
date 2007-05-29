module(..., package.seeall);

-- Gets the table containig the transitions of the FSM and organize them
-- into the followings structure:
-- fsm[currentState][event] = {nextState, nextAction}
-- And also sets the FSM's final state
function create(table, finalState)
  local fsm = {finalState = finalState} -- setting the final state
  for _,v in ipairs(table) do
    -- getting each transition of the input
    local old, event, new, action = v[1], v[2], v[3], v[4]
    if fsm[old] == nil then
      fsm[old] = {} -- initialize the table for the current state
    end
    -- set the next state and action for the current state in case
    -- that event happens
    fsm[old][event] = {nextState = new, nextAction = action}
  end
  return fsm
end

-- This one "plays" the FSM starting at the first current state given.
-- It will return every single possibility of reaching the final state into
-- the var "path", which is a table.
function play(currentState, fsm, path)
  table.insert(path, currentState) -- saves into "path" the currentState
  if currentState == fsm.finalState then -- if reached the final state, return
      table.foreach(path, print)
      print("Reached the end!!!")
    return path
  end

  for event, _ in pairs(fsm[currentState]) do
    -- calls the play function recursevily for each possible event on the
    -- current state.
    play(fsm[currentState][event].nextState, fsm, path)
    -- removes the last inserted state, so it can explore the other ways.
    table.remove(path, # path)
  end
end

ex = {{"s1", "e1", "s2", "a1"},
      {"s1", "e4", "s4", "a1"},
      {"s1", "e2", "s2", "a1"},
      {"s4", "e5", "s2", "a4"},
      {"s2", "e2", "s3", "a2"},
      {"s3", "e3", "s1", "a3"}}
f = create(ex, "s3")
cs = "s1"
p = {}
play(cs, f, p)

