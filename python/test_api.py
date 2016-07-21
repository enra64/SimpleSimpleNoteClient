import sn as simplenote
print("import ok")
simplenote = simplenote.Simplenote("***REMOVED***", "***REMOVED***")
print("connection ok")
test = simplenote.get_note_list()
print (test)
