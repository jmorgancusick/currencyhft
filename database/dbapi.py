#!/usr/bin/python

#run: apt-get install python-mysqldb
import MySQLdb as sql
import json

CREDSFILE = "../../credentials.txt"

formatFile = open("tableColumns.json",'r')
FORMATS = json.loads(formatFile.read())
formatFile.close()


#open database connection
def Connect(credsFile = None):
    if credsFile == None:
	credsFile = CREDSFILE
    file = open(credsFile, 'r')
    creds = file.read().strip()
    file.close()
    creds = creds.split("\n")
    try:
	db = sql.connect(host=creds[0],user=creds[1],passwd=creds[2],db='currencyhft')
	cur = db.cursor()
	return db, cur
    except Exception as e:
	print "ERROR: database connection error\n"+str(e)

#close database connection
def Close(db):
    db.close()

#insert new rows into database
#data is a dictionary
def Insert(cur, table, data):
    tableCols = ""
    values = ""
    update = ""
    cols = FORMATS[table]["cols"]
    keys = FORMATS[table]["keys"]
    for c in cols:
	tableCols += c+","
	if c not in keys:
	    update += c + " = "
	val = data[c]
	if isinstance(val,basestring):
	    values += "\'"+val+"\',"
	    if c not in keys:
		update += "\'"+val+"\',"
	else:
	    values += str(val)+","
	    if c not in keys:
		update += str(val)+","
    tableCols = tableCols.strip(",")
    values = values.strip(",")
    update = update.strip(",")
    insertion = "INSERT INTO {0} ({1}) VALUES ({2}) ON DUPLICATE KEY UPDATE {3}".format(table,tableCols,values,update)
    try:
	cur.execute(insertion)
	db.commit()
    except Exception as e:
	db.rollback()
	print "ERROR: unable to add or update row to table" + str(e) 

if __name__ == '__main__':
    db,cur = Connect()
    data = {"ticker" : "USDEUR"	
    , "start" : "USD"	
    , "start_full" : "US Dollar"    
    , "end" : "EUR"
    , "end_full" : "Euros"	
    , "rate" : 2.2768768
    , "ex_id" : "1321321"}
    Insert(cur,'forex',data)
    Close(db)
