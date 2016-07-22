import sn as simplenote
print("import ok")
simplenote = simplenote.Simplenote("***REMOVED***", "***REMOVED***")
#print("connection ok")
#test = simplenote.get_note_list()
#print (test)

print("stop\n")
note = simplenote.get_note("agtzaW1wbGUtbm90ZXIRCxIETm90ZRiAgOC20YLlCAw")

print("stop\n")
simplenote.update_note(note)

