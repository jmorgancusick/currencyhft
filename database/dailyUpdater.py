import forexDataScrape as fds 
import dbapi
import datetime
import itertools
import schedule
from lxml import html
import requests


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


def getCitiRates(currencies):
	page = requests.get('https://www.citibank.com.au/aus/investments/forex-rates/USD.htm')
	tree = html.fromstring(page.content)
	curName = tree.xpath('//tr/td/strong/text()')
	value = tree.xpath('//tr/td[4]/text()')

	values = {}
	for i in range(0,len(value)):
		currency = curName[2*i+1]
		if currency in currencies:
			values[curName[2*i+1]] = float(value[i])
	values["USD"] = 1.00

	return values


def getHSBCRates(currencies):
	page = requests.get('https://www.hsbc.ca/1/2/calculators/foreign-exchange-calculator')
	tree = html.fromstring(page.content)
	curName = tree.xpath('//tr/td[@class="hsbcTdStyle2"][2]/text()')
	value = tree.xpath('//tr/td[@class="hsbcTdStyle2"][4]/text()')

	values = {}
	for i in range(0,len(value)):
		currency = curName[i]
		if currency in currencies:
			values[curName[i]] = float(value[i])
	values["CAD"] = 1.00

	return values


def extrapolate(base, data, pairs):
	exchanges = {}
	for p in pairs:
		cur1 = p[0]
		cur2 = p[1]
		if cur1 == base:
			exchanges[cur1+cur2] = data[cur2]
		elif cur2 == base:
			exchanges[cur1+cur2] = 1/data[cur1]
		else:
			exchanges[cur1+cur2] = data[cur2]/data[cur1]

	return exchanges


def bankUpdate(data, bank):
	update = []
	for d in data.keys():
		obj = {}
		obj["start"] = d[0:3]
		obj["end"] = d[3:6]
		obj["bank"] = bank
		obj["rate"] = data[d]
		update.append(obj)

	return update

def updateBanks():
	currencies = ["USD", "EUR", "JPY", "GBP", "CHF", "CAD", "AUD", "NZD"]
	currencyPairs = list(itertools.permutations(currencies, 2))
	db = dbapi.API()
	db.connect()

	values = getCitiRates(currencies)
	rates = extrapolate("USD", values, currencyPairs)
	update = bankUpdate(rates, "CIT")
	retVal = db.bulkInsert("bankRates", update)
	if retVal == False:
		print "ERROR: bulk insert failed for file: ", filename

	values = getCitiRates(currencies)
	rates = extrapolate("CAN", values, currencyPairs)
	update = bankUpdate(rates, "HSB")
	retVal = db.bulkInsert("bankRates", update)
	if retVal == False:
		print "ERROR: bulk insert failed for file: ", filename


#data must be two dimensions
def mapify(data):
	m = {}
	for row in data:
		m[row[0]] = row
	return m


if __name__ == '__main__':
 	schedule.every().day.at("00:01").do(updateForex)
 	schedule.every().day.at("01:00").do(updateBanks)
 	while 1:
 		schedule.run_pending()
 		#time.sleep(1)