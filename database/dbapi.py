#!/usr/bin/python

#run: apt-get install python-mysqldb
import MySQLdb as sql
import json


#Class that handles all SQL calls from the python code
class API:

  defaultCredsFile = "../../credentials.txt"
  formats = None
  db = None
  cur = None

  def __init__(self):
    #gets the database table formats from a file
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
      self.db = sql.connect(host=creds[0],user=creds[1],passwd=creds[2],db=creds[3])
      self.cur = self.db.cursor()
    except Exception as e:
      print "ERROR: database connection error\n"+str(e)


  #close database connection
  def close(self):
    self.db.close()


  #takes an array of rows and calls commit after
  def bulkInsert(self, table, data):
    for row in data:
      self.insert(table, row, True)
    return self.commit()


  #insert new rows into database
  #data is a dictionary
  #bulk, if true, disables the commit statemeny. This quickens insertions but
  #the data will not be live until the programmer manually calls commit
  def insert(self, table, data, bulk = False, requireData = True):
    tableCols = ""
    values = ""
    update = ""
    cols = self.formats[table]["cols"]
    keys = self.formats[table]["keys"]
    for c in cols:

      # allows for user to not specify columns in data argument
      if not c in data:
        continue

      # allows for missing data columns
      if data[c] == None:
        continue

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
    insertion = "INSERT INTO {0} ({1}) VALUES ({2})".format(table,tableCols,values)
    
    # allows for only a key to be inserted (with an error on duplicate)
    # IF requireData is also passed as false
    if update == "":
      if requireData: 
        return
    else:
      insertion += " ON DUPLICATE KEY UPDATE {0}".format(update)

    #execute the insertion query
    try:
      self.cur.execute(insertion)
      if not bulk:
        self.db.commit()
    except Exception as e:
      if not bulk:
        self.db.rollback()
        print "ERROR: unable to add or update row to table" + str(e) 
      else:
        print "WARNING: unable to execute call: ",insertion
        print str(e) 


  #gets every row from the the specified table in the database
  def getAllData(self, table):
    insertion = "SELECT * from {0}".format(table)

    try:
      print insertion
      self.cur.execute(insertion)
      rows = self.cur.fetchall()
      return list(rows)
    except Exception as e:
      print e
      if table not in self.formats.keys():
        print "ERROR: the inputted table name does not exist"
      else:
        print "ERROR: could not execute call"
        print str(e)
      return None


  #commits the previous execute calls to the database
  #rolls back changes if there is an error
  def commit(self):
    try:
      self.db.commit()
    except Exception as e:
      self.db.rollback()
      print "ERROR: unable to commit data to table" + str(e) 
      return False
    return True

  #gets the name of the forex history table based on the interval
  def getTableNameForDataInterval(self, dataInterval):
    table = None
    if dataInterval == "1d":
      table = "forexDashDaily"
    elif dataInterval == "1m":
      table = "forexDashMinute"
    else: 
      print "ERROR: unknown data interval: ", dataInterval
    return table

#used for testing
if __name__ == '__main__':
  db = API()
  db.connect()
  db.cur.execute("describe forex")
  data = {"ticker" : "USDEUR=X"   
  , "current_rate" : 120.03
  , "last_rate" : 121.03
  , "change_day" : 1.03}
  db.insert('forex',data)
  db.close()