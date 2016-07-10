import simplenote
print("import ok")
simplenote = simplenote.Simplenote("***REMOVED***", "***REMOVED***")
print("connection ok")
test = simplenote.get_note("088e930172da4910ae07d6b95d09727d")
print (test)
