drop table if exists forexDashMinute;
drop table if exists forexDashDaily;
drop table if exists forex;
drop table if exists bankRates;
drop table if exists profitableTrends;
drop table if exists efficientTrends;


create table forexDashMinute ( 
	timestamp 		bigint		-- hhmmssddMMyy
	, ticker  		varchar(40)	-- ie "USDEUR" for usd to eur
	, start	 		varchar(40) 	-- might not need this abv
	, start_full 		varchar(255) 	-- full name of currency
	, end 			varchar(40) 
	, end_full 		varchar(255)
	, high  		double		
	, volume 		double
	, open 			double
	, low 			double
	, close 		double
	, ex_id 		varchar(40)	-- hash based on start and end currency
	, primary key(timestamp, ticker) 
);

create table forexDashDaily ( 
	timestamp 		bigint		-- hhmmssddMMyy
	, ticker  		varchar(40)	-- ie "USDEUR" for usd to eur
	, start	 		varchar(40) 	-- might not need this abv
	, start_full 		varchar(255) 	-- full name of currency
	, end 			varchar(40) 
	, end_full 		varchar(255)
	, high  		double		
	, volume 		double
	, open 			double
	, low 			double
	, close 		double
	, ex_id 		varchar(40)	-- hash based on start and end currency
	, primary key(timestamp, ticker) 
);

create table forex ( 
	ticker 	 		varchar(40) primary key	-- ie "USDEUR" for usd to eur
	, start	 		varchar(40) 	-- might not need this abv
	, start_full 		varchar(255) 	-- full name of currency
	, end 			varchar(40) 
	, end_full 		varchar(255)
	, rate 			double		-- value of second given 1 of first
	, ex_id 		varchar(40)	-- hash based on start and end currency
);

create table bankRates (	-- extraneous probly
	start 			varchar(40)  	-- starting currency abv
	, start_full 		varchar(255)  	-- full name of currency
	, end 			varchar(40) 
	, end_full 		varchar(255)
	, bank 	 		varchar(40)	-- bank name abv
	, bank_full 		varchar(255)
	, rate	 		double		-- value of second given 1 of first
	, ex_id	 		varchar(40)	primary key	-- hash based on start, end currency, and bank
);


create table profitableTrends(
	start	 		varchar(40) 	-- starting currency abv
	, start_full 		varchar(255) 	-- full name of currency
	, end	 		varchar(40) 
	, end_full 		varchar(255)
	, profit  		double 		-- profit based on initial currency
	, length 		int 		-- length of path
	, expath   		varchar(500)	-- in form "USDCITI|EURCITI|USDWELLS"
						-- to go from USD in Citi bank to Euros in Citi to USD in wells fargo
	, frequency 		int
	, path_id 		varchar(40)	primary key	-- hashed based on expath variable
);


create table efficientTrends(
	start	 		varchar(40) 	-- starting currency abv
	, start_full 		varchar(255) 	-- full name of currency
	, end	 		varchar(40) 
	, end_full 		varchar(255)
	, end_value 		double 		-- value based on initial currency
	, length 		int 		-- length of path
	, expath   		varchar(500)	-- in form "USDCITI|EURCITI|USDWELLS"
						-- to go from USD in Citi bank to Euros in Citi to USD in wells fargo
	, frequency 		int
	, path_id 		varchar(40)	primary key	-- hashed based on expath variable
);

alter table forexDashMinute add constraint fminute_unique unique (timestamp,ticker);
alter table forexDashDaily add constraint fdaily_unique unique (timestamp,ticker);
alter table forex add constraint forex_unique unique (ticker);
