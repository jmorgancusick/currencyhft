'''
forexDataScrape.py

Usage:
forexDiskToDB.py

Description:
Takes local files and insert them to database

'''

import json
import glob
from pprint import pprint

def main():
	print "Hello World!"

	data = {}
	
	for folder in glob.glob('data/*'):
		print "foldername: ", folder
		for filename in glob.glob(folder+'/*.txt'):
			print "\tfilename: ", filename
			ticker = folder.split('/')[1]
			print "ticker: ", ticker

			with open(filename, 'r') as f:
				data = json.load(f)
				pprint(data)
				return			


if __name__ == '__main__':
	main()