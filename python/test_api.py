import sn as simplenote
print("import ok")
simplenote = simplenote.Simplenote("***REMOVED***", "***REMOVED***")
#print("connection ok")
#test = simplenote.get_note_list()
#print (test)

print("stop\n")
print(simplenote.get_note("1e7ac2ccc07042ae707a495ea2abd659"))
