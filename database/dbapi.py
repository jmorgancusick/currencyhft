#!/usr/bin/python

#run: apt-get install python-mysqldb
import MySQLdb as sql
import json

class API:

  defaultCredsFile = "../../stockCredentials.txt"
  formats = None
  db = None
  cur = None

  def __init__(self):
    formatFile = open("tableColumns.json",'r')
    self.formats = json.loads(formatFile.read())
    formatFile.close()


  #open database connection
  def connect(self, credsFile = None):
    if credsFile == None:
      credsFile = self.defaultCredsFile
    file = open(credsFile, 'r')
    creds = file.read().strip()
    file.close()
    creds = creds.split("\n")
    try:
      self.db = sql.connect(host=creds[0],user=creds[1],passwd=creds[2],db='schemaTest')
      self.cur = self.db.cursor()
    except Exception as e:
      print "ERROR: database connection error\n"+str(e)

  #close database connection
  def close(self):
    self.db.close()

  #insert new rows into database
  #data is a dictionary
  def insert(self, table, data):
    tableCols = ""
    values = ""
    update = ""
    cols = self.formats[table]["cols"]
    keys = self.formats[table]["keys"]
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
      self.cur.execute(insertion)
      self.db.commit()
    except Exception as e:
      self.db.rollback()
      print "ERROR: unable to add or update row to table" + str(e) 

if __name__ == '__main__':
  db = API()
  db.connect()
  data = {"datetime" : "05194503202017"   
  , "exstr" : "USDEUR"    
  , "start" : "USD"   
  , "start_full" : "US Dollar"    
  , "end" : "EUR"
  , "end_full" : "Euros"  
  , "rate" : 2.2768768
  , "ex_id" : "1321321"}
  db.insert('forex',data)
  db.close()
