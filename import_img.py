#!/usr/bin/python

import xmlrpclib
import sys
from pymongo import MongoClient

# config info
data_url = "/home/bobzc/pool/mosaics/src/"
server_url = "http://127.0.0.1:31128/RPC"
server = xmlrpclib.ServerProxy(server_url);


# check arguements
if len(sys.argv) <= 1:
	print "Invalid call!"
	exit(0)


# add image into imgseek database
fname = sys.argv[1]
img_id = server.getDbImgCount(1) + 1

ret_val = server.addImg(1, img_id, data_url + fname, False)


# check result
if ret_val != 1:
	print "Fail to import image!"
	exit(0)
else:
	print "Importation successful: "
	print fname + " => " + str(img_id)


# insert file name mapping into mongodb
client = MongoClient()
db = client.mosaics_db
clt = db.fname_map

ret_val = clt.insert({"file_name": fname, "image_id": img_id})

# check result
if ret_val == None:
	print "Fail to insert doc into mongodb!"
else:
	print "Success to insert doc into mongodb!"
