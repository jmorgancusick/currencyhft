import datetime
import time
import urllib2
import itertools
import json


def unixTime(dt):
	epoch = datetime.datetime.utcfromtimestamp(0)
	return int((dt - epoch).total_seconds())



currencies = ["USD", "EUR", "JPY", "GBP", "CHF", "CAD", "AUD", "NZD"]


currencyPairs = list(itertools.permutations(currencies, 2)) #56
currencyTickers = []

for pair in currencyPairs:
	currencyTickers.append(pair[0] + pair[1] + "=X")

numYears = 5

ticker = "EURUSD=X"
startTime = "1363870434" 
endTime = "1489668834"
interval =" 1d"
queryURL = "https://query1.finance.yahoo.com/v7/finance/chart/"+ticker+"?period2="+endTime+"&period1="+startTime+"&interval="+interval+"&indicators=quote&includeTimestamps=true&includePrePost=true&events=div%7Csplit%7Cearn&corsDomain=finance.yahoo.com"

print currencyPairs

print currencyTickers
#raw = urllib2.urlopen(queryURL).read()


#write time logicw
now = datetime.datetime.utcnow()
nowTime = int(time.time())
yesterday = now - datetime.timedelta(days=1)

print "time.time(): ",nowTime
print "datetime.utcnow(): ", unixTime(now)
print "yesterday: ",unixTime(yesterday)





with open('workfile.txt', 'w') as f:
	json.dump(currencyTickers, f)
f.closed

#All components working:
# - get all ticker symbols
# - make queries to yahoo page
# - intervals of day and minute
# - can use utc (gmt) timestamps and shift them by days or years
# - can json dump to a file to ensure no loss of scrapped data

#TODO
# - clean up code: functions, flow comments, etc
# - Logic for 10 queries per ticker (5 years at day intervals, 5 days at minute intervals) 
# - ensure all 56 tickers correct
# - connect with dbAPI
# - logic for after gather historical data... 1 daily query per ticker for 1 day at minute intervals