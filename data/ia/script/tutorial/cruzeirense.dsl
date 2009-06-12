# Script to control the cruzeirense behaviour.
#

script()

  # To wait 20 seconds
  int waitTime
  waitTime = 20

  # positions
  int posX posZ

  # where
  bool where
  where = false

  while(true)
     wait(waitTime)

     if(where)
        posX = 25
        posZ = 291
        moveToPosition(SELF_CHARACTER, posX, posZ)
        where = false
        waitTime = 20
     else
        posX = 120
        posZ = 150
        moveToPosition(SELF_CHARACTER, posX, posZ)
        where = true
        waitTime = 5
     end
  end
end

