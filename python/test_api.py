import sn as simplenote
print("import ok")
simplenote = simplenote.Simplenote("online_services_mail@arcor.de", "^ufr1p14JaIIZc3a%VGHViy^%kslyVvY7ExV80%UwEMxcD*&6ubsfv5*DMr^Gh0qB9g0P8UJ")
#print("connection ok")
#test = simplenote.get_note_list()
#print (test)

print("stop\n")
note = simplenote.get_note("agtzaW1wbGUtbm90ZXIRCxIETm90ZRiAgOC20YLlCAw")

print("stop\n")
simplenote.update_note(note)

