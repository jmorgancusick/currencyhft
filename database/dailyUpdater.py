import forexDataScrape as fds 
import dbapi
import datetime
import itertools
import schedule


def getData(ticker):

	endTime = fds.startOfDay(datetime.datetime.utcnow())
	startTime = fds.startOfDay(endTime - datetime.timedelta(days=1))
	interval = "1m"

	dataMin = fds.execScrape(ticker, startTime, endTime, interval)
	dataDay = fds.execScrape(ticker, startTime, endTime, "1d")

	return dataMin,dataDay


#data = {ticker : data, ticker2: data2}
def updateTicker(ticker, data, old, db):
 	cols = db.formats["forex"]["cols"]

 	update = {}
 	for i in range(0,len(cols)):
 		col = cols[i]
 		if col == "current_rate":
 			update[col] = data["quote"]["close"][0]
		elif col == "last_rate":
			if old[ticker][i-1] == None:
				update[col] = 0.
			else:
				update[col] = old[ticker][i-1]
		elif col == "change_day":
			if update["last_rate"] == 0:
				update[col] = 0.
			else:
				update[col] = (data["quote"]["close"][0]/update["last_rate"] - 1)*100
		else:
			update[col] = old[ticker][i]
	
	retVal = db.insert("forex", update)
	if retVal == False:
		print "ERROR: bulk insert failed for file: ", filename



def updateHistory(ticker, data, table, db):
	bulkUpdate = []

	for i in range(0,len(data["timestamp"])):
		update = {}
		
		update["ticker"] = ticker
		update["start"] = ticker[0:3]
		update["end"] = ticker[3:6]
		update["timestamp"] = data["timestamp"][i]
		update["volume"] = data["quote"]["volume"][i]
		update["close"] = data["quote"]["close"][i]
		update["high"] = data["quote"]["high"][i]
		update["open"] = data["quote"]["open"][i]
		update["low"] = data["quote"]["low"][i]

		bulkUpdate.append(update)

	retVal = db.bulkInsert(table, bulkUpdate)
	if retVal == False:
		print "ERROR: bulk insert failed for file: ", filename



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
		dataMin,dataDay = getData(ticker)
		updateTicker(ticker, dataDay, old, db)
		updateHistory(ticker, dataDay, "forexDashDaily", db)
		updateHistory(ticker, dataMin, "forexDashMinute", db)


#data must be two dimensions
def mapify(data):
	m = {}
	for row in data:
		m[row[0]] = row
	return m


if __name__ == '__main__':
 	schedule.every().day.at("00:01").do(updateForex)
 	while 1:
 		schedule.run_pending()
 		#time.sleep(1)