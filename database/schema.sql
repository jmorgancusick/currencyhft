drop table if exists forex;
drop table if exists bank_rates;
drop table if exists profitable_trends;
drop table if exists efficient_trends;


create table Forex ( --extraneous probly
	start			varchar(40) 	--starting currency abv
	, start_full	varchar(255) 	--full name of currency
	, end			varchar(40) 
	, end_full		varchar(255)
	, rate			float			--value of second given 1 of first
	, ex_id			varchar(40)	primary key	--hash based on start and end currency
);

create table bank_rates ( --extraneous probly
	start			varchar(40) 	--starting currency abv
	, start_full	varchar(255) 	--full name of currency
	, end			varchar(40) 
	, end_full		varchar(255)
	, bank 			varchar(40)		--bank name abv
	, bank_full 	varchar(255)
	, rate			float			--value of second given 1 of first
	, ex_id			varchar(40)	primary key	--hash based on start, end currency, and bank
);


create table profitable_trends(
	start			varchar(40) 	--starting currency abv
	, start_full	varchar(255) 	--full name of currency
	, end			varchar(40) 
	, end_full		varchar(255)
	, profit 		float 			--profit based on initial currency
	, length		int 			--length of path
	, expath  		varchar(500)	--in form "USDCITI|EURCITI|USDWELLS"
									--to go from USD in Citi bank to Euros in Citi to USD in wells fargo
	, frequency		int
	, path_id		varchar(40)	primary key	--hashed based on expath variable
);


create table efficient_trends(
	start			varchar(40) 	--starting currency abv
	, start_full	varchar(255) 	--full name of currency
	, end			varchar(40) 
	, end_full		varchar(255)
	, end_value		float 			--value based on initial currency
	, length		int 			--length of path
	, expath  		varchar(500)	--in form "USDCITI|EURCITI|USDWELLS"
									--to go from USD in Citi bank to Euros in Citi to USD in wells fargo
	, frequency		int
	, path_id		varchar(40)	primary key	--hashed based on expath variable
);
