# -*- coding: utf-8 -*-
"""
    simplenote.py
    ~~~~~~~~~~~~~~

    Python library for accessing the Simplenote API

    :copyright: (c) 2011 by Daniel Schauenberg
    :license: MIT, see LICENSE for more details.
"""
import sys
if sys.version_info > (3, 0):
    import urllib.request as urllib2
    import urllib.error
    from urllib.error import HTTPError
    import urllib.parse as urllib
else:
    import urllib2
    from urllib2 import HTTPError
    import urllib


import base64
import time
import datetime

try:
    import json
except ImportError:
    try:
        import simplejson as json
    except ImportError:
        # For Google AppEngine
        from django.utils import simplejson as json
        
response = urllib2.urlopen("https://app.simplenote.com/api2/data/1e7ac2ccc07042ae707a495ea2abd659?auth=D45F9AC2493DE08C68C9C79A90570E735688EEC7C96390EF4C7882DC58F047F7&email=***REMOVED***")
print(response.info())
print(response.geturl())
print(response.read().decode('utf-8'))
