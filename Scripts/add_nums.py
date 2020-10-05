d = """Idle
Walk
Run
TurnAround
Fall
Unknown
MoveHeadUpwards
SlideTurn
StopRunning
StartWalking
EndWalking
Land
Unknown
StartFastBarking
EndFastBarking
Empty
Sleeping
FallAsleep
WakeUp
JumpForwards
JumpUpwards
Eating
Empty
Scratch
Growl
"""

a = """4742E0
4743F0
4749A0
474C70
4750C0
474070
474220
474DB0
474EC0
474690
4747D0
475AB0
475B50
475B70
475BB0
475290
4752E0
475510
475460
475610
475890
475900
475A90
475550
475590
"""

lines = d.split("\n")
addrs = a.split("\n")

num = 0
for line in lines:
    line = line.strip()
    #line = "ENTRY(State_" + str(num) + "_" + line + "_" + addrs[num] + ") \\" 

    #line = "&Slog::State_" +  str(num) + "_" + line + "_" + addrs[num] + ","

    #line = "EXPORT void State_" +  str(num) + "_" + line + "_" + addrs[num] + "();"

    #line = "void Slog::State_" +  str(num) + "_" + line + "_" + addrs[num] + "()\n{\n    NOT_IMPLEMENTED();\n}\n"

    line = "State_" + str(num) + "_" + line + "_" + addrs[num] + "," 

    print line
    num = num + 1
