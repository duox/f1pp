#env VAR:int "VAR"
#set VAR = 1
#print "VAR = " VAR

#exec "cmd.exe /c echo %VAR%"

#env VAR2="VAR"
#print "VAR2 = " VAR2
#set VAR2 = 2
#print "VAR2 = " VAR2

#print "VAR = " VAR

#exec "cmd.exe /c echo %VAR%"
