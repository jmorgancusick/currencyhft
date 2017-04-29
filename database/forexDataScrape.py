'''
forexDataScrape.py
Usage:
forexDataScrape.py
Description:
Pulls data from yahoo and stores in local files
'''

import datetime
import time
import urllib2
import itertools
import json
import os
import errno
from dateutil.relativedelta import relativedelta
from random import randint

def randomDelay():
  # set by developer
  randomDelayMin = 15
  randomDelayMax = 30

  # get random number between min and max
  randomDelayTime = randint(randomDelayMin, randomDelayMax)
  time.sleep(randomDelayTime)

def make_sure_path_exists(path):
  try:
    os.makedirs(path)
  except OSError as exception:
    if exception.errno != errno.EEXIST:
      raise

def unixTime(dt):
  epoch = datetime.datetime.utcfromtimestamp(0)
  return int((dt - epoch).total_seconds())

def startOfDay(datetimeObj):
  return datetime.datetime(datetimeObj.year, datetimeObj.month, datetimeObj.day)

def parseResponse(response):
  raw = json.load(response)

  data = {}
  data["timestamp"] = raw["chart"]["result"][0]["timestamp"]
  data["quote"] = raw["chart"]["result"][0]["indicators"]["quote"][0]
  return data

def execScrape(ticker, startTime, endTime, interval):
  print "pulling ", ticker, " data from ", startTime , " to ", endTime

  queryURL = "https://query1.finance.yahoo.com/v7/finance/chart/"+ticker+"?period2="+str(unixTime(endTime))+"&period1="+str(unixTime(startTime))+"&interval="+interval+"&indicators=quote&includeTimestamps=true&includePrePost=true&events=div%7Csplit%7Cearn&corsDomain=finance.yahoo.com"
  
  # execute query
  response = urllib2.urlopen(queryURL)

  # parse response
  data = parseResponse(response)

  # dump data
  filename = ticker+"_"+str(startTime.strftime("%Y-%m-%d@%H:%M:%S"))+"_to_"+str(endTime.strftime("%Y-%m-%d@%H:%M:%S"))+"_"+interval+".txt"
  filepath = "data/"+ticker+"/"+filename
  #print filepath
  with open(filepath, 'w') as f:
    json.dump(data, f)

  return data
  

def tickerScrape(ticker):

  # 5 years at 1d interval, 1 query

  # setup query string
  interval ="1d"
  endTime = startOfDay(datetime.datetime.utcnow())
  startTime = startOfDay(endTime - relativedelta(years=5))
  
  # query the url, parse the response, and write to disk
  execScrape(ticker, startTime, endTime, interval)

  #random delay between all queries
  randomDelay()


  # 5 days at 1m interval, 1 query
  
  # setup query string
  interval ="1m"
  endTime = startOfDay(datetime.datetime.utcnow())
  startTime = startOfDay(endTime - datetime.timedelta(days=5))

  # query the url, parse the response, and write to disk
  execScrape(ticker, startTime, endTime, interval)

  #random delay between all queries
  randomDelay()



def main():

  currencies = ["USD", "EUR", "JPY", "GBP", "CHF", "CAD", "AUD", "NZD"]


  currencyPairs = list(itertools.permutations(currencies, 2)) #56
  currencyTickers = []

  for pair in currencyPairs:
    currencyTickers.append(pair[0] + pair[1] + "=X")

  #print currencyTickers

  progressFile = "scrapeProgress.txt"


  #============SCRAPE DATA START============

  # make data directory
  make_sure_path_exists("data")

  #grab progress
  i = 0
  with open(progressFile, 'r') as f:
    startIndex = int(f.read())
    i = startIndex

  #continue scraping
  while i < len(currencyTickers):
    #progress print
    print i , ":",currencyTickers[i]

    #1. create directory
    make_sure_path_exists("data/"+currencyTickers[i])

    #2. scrape the data
    tickerScrape(currencyTickers[i])

    i+=1

    #update progess file
    with open(progressFile, 'w') as f:
      f.write(str(i))

  #when finished, reset the progress file
  with open(progressFile, 'w') as f:
    f.write("0")

if __name__ == '__main__':
  main()
