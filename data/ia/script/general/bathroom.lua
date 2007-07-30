print("Playing the goto bathroom planning...")

bathroom = {{"locateAndGo", "done", "doAction", "bathroom"}
           ,{"locateAndGo", "fail", "idle", "bathroom"}
           ,{"doAction", "done", "idle", "pi"}
	        ,{"doAction", "fail", "idle", "pi"}}

f = create(bathroom, "idle")
play("locateAndGo", f)

