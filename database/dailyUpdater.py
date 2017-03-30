import forexDataScrape as fds 
import dbapi
import datetime
import itertools


def getData(ticker):

	endTime = fds.startOfDay(datetime.datetime.utcnow())
	#endTime = endTime.replace(hour = 0, minute = 0, second = 0, microsecond = 0)
	startTime = fds.startOfDay(endTime - datetime.timedelta(days=1))
	interval = "1m"

	fds.execScrape(ticker, startTime, endTime, interval)
	data = fds.execScrape(ticker, startTime, endTime, "1d")

	return data


#data = {ticker : data, ticker2: data2}
def updateTicker(ticker, data, old):
 	cols = db.formats["forex"]["cols"]

 	print data
	update = {}
 	for i in range(0,len(cols)):
 		col = cols[i]
 		if col == "current_rate":
 			update[col] = data["quote"]["close"][0]
		elif col == "last_rate":
			if old[ticker][i] == None:
				update[col] = 0.
			else:
				update[col] = old[ticker][i]
		elif col == "change_day":
			if old[ticker][i] == None or old[ticker][i] == 0:
				update[col] = 0.
			else:
				update[col] = (data["quote"]["close"][0]/old[ticker][i] - 1)*100
		else:
			update[col] = old[ticker][i]
	print "hi"
	print update

	db.insert("forex", update)


def updateForex():
	currencies = ["USD", "EUR", "JPY", "GBP", "CHF", "CAD", "AUD", "NZD"]

 	currencyPairs = list(itertools.permutations(currencies, 2))
	currencyTickers = []

	for pair in currencyPairs:
 		currencyTickers.append(pair[0] + pair[1] + "=X")

	db = dbapi.API()
	db.connect()
	old = db.getAllData('forex')
	old = mapify(old)

	for ticker in currencyTickers:
		data = getData(ticker)
		updateTicker(ticker,data,old)


#data must be two dimensions
def mapify(data):
	m = {}
	for row in data:
		m[row[0]] = row
	print m
	return m

#def pullPrevData

if __name__ == '__main__':
 	#getData("USDEUR=X")
 	db = dbapi.API()
 	db.connect()
 	updateForex()