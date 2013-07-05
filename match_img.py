#!/usr/bin/python
import xmlrpclib
import sys
from pymongo import MongoClient

# config info
data_url = "/home/bobzc/pool/mosaics/target/"
server_url = "http://127.0.0.1:31128/RPC"
server = xmlrpclib.ServerProxy(server_url);

# check arguements
if len(sys.argv) <= 1:
	print "Invalid call!"
	exit(0)

# match image
fname = sys.argv[1]
ret_val = server.queryImgPath(1, data_url + fname, 1, 0, True)
img_id = ret_val[0][0]

# get image file name according to id
client = MongoClient()
db = client.mosaics_db
clt = db.fname_map

ret_val = clt.find_one({"image_id": img_id})

# write result to file
f = open('tmp.txt', 'w')
f.write(ret_val["file_name"])
f.close();
