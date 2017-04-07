import forexDataScrape as fds 
import dbapi
import datetime
import itertools
import schedule


def getData(ticker):

	endTime = fds.startOfDay(datetime.datetime.utcnow())
	startTime = fds.startOfDay(endTime - datetime.timedelta(days=1))
	interval = "1m"

	fds.execScrape(ticker, startTime, endTime, interval)
	data = fds.execScrape(ticker, startTime, endTime, "1d")

	return data


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
		updateTicker(ticker,data,old,db)


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