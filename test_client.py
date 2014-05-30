#!/usr/bin/env python
# coding: utf-8

import pyjsonrpc

http_client = pyjsonrpc.HttpClient(
				url = "http://localhost:8080"
)
print http_client.call("jail", c=True, j="testjail")
# Result: 3

# It is also possible to use the *method* name as *attribute* name.
print http_client.add(a=1, b=2)
