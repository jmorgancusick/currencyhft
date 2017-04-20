'''
forexDiskToDB.py

Usage:
forexDiskToDB.py

Description:
Takes local files and insert them to database

'''

import json
import glob
from pprint import pprint

import dbapi

def main():

  data = {}

  db = dbapi.API()
  db.connect()

  progressCounter = 1
  progressFinish = 56.0 # number of currency pairs
  
  for folder in glob.glob('data/*'):
    print "Currency Pair "+ str(progressCounter)+ " of " + str(int(progressFinish)) + ": " + str(int(round(progressCounter/progressFinish*100))) + "%"
    print "foldername: ", folder
    ticker = folder.split('/')[1]
    print "ticker: ", ticker

    #create ticker in forex table
    obj = {}
    obj["ticker"] = ticker
    
    for filename in glob.glob(folder+'/*.txt'):
      
      #print "\tfilename: ", filename
      dataInterval = filename[-6:][0:2] #file name ends in ..._[dataInterval].txt
      print "\tdataInterval: ", dataInterval
      
      #check dataInterval and set table
      table = db.getTableNameForDataInterval(dataInterval)
      if table is None:
        print "ERROR: Bad table, skipping file: ",filename
        continue


      with open(filename, 'r') as f:
        data = json.load(f)
        #pprint(data)

        bulkData = []

        for i in range(len(data["timestamp"])):
          #print "i: " ,i

          #construct the object for a mysql insert
          obj = {}
          obj["ticker"] = ticker
          obj["timestamp"] = data["timestamp"][i]
          obj["volume"] = data["quote"]["volume"][i]
          obj["close"] = data["quote"]["close"][i]
          obj["high"] = data["quote"]["high"][i]
          obj["open"] = data["quote"]["open"][i]
          obj["low"] = data["quote"]["low"][i]

          #print obj

          bulkData.append(obj)

        # insert into database
        retVal = db.bulkInsert(table, bulkData)
        if retVal == False:
          print "ERROR: bulk insert failed for file: ", filename

    progressCounter+=1

  print "Completed. Goodbye."
  db.close()
     




if __name__ == '__main__':
  main()
