/* app.js
Data server used for serving Forex and bank data to the frontend via a REST_API.
Algorithms are also exposed to the frontend.
*/

// ===================
// Server Dependencies
// ===================

// C++ Binary
const addon = require('./src/build/Release/addon');

// JavaScript Libraries
var express = require('express');
var path = require('path');
var fs = require('fs');
var logger = require('morgan');
var exec = require('child_process').exec;
var bodyParser = require('body-parser');
var app = express();

// =====================
// Server Initialization
// =====================

app.use(logger('dev'));
app.use(bodyParser.json());
app.use(function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});

app.listen(3000, function () {
  console.log('photosbyjac.com node server listening on port 3000');
});

// =============
// Exit handling
// =============

function exitHandler(options, err) {
    if (options.cleanup){
      console.log('Press Control-D to exit immediately.');
      addon.shutdown();
      console.log('Cleanup complete. Goodbye :)');
    }
    if (err) console.log(err.stack);
    if (options.exit) process.exit();
}

//cleanup when app is closing
process.on('exit', exitHandler.bind(null,{cleanup:true}));

//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null, {exit:true}));

//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null, {exit:true}));

// ==============
// Data Endpoints
// ==============

// Return the 18 most popular currency pairs along with their most recent exchange rate
app.get('/tickerData/', function(req, res) {
    console.log("ticker data endpoint");

    ret = addon.tickerData();
    
    res.send(ret);

});

// Return high, low and close exchange rates for single ticker over a period of time.
// This data is used to populate the Dashboard chart
app.get('/chartData/:ticker/:timeframe/:startDate/:endDate', function(req, res) {
    console.log("chart data endpoint");

    ret = addon.chartData(req.params.ticker,req.params.timeframe,req.params.startDate,req.params.endDate);

    res.send(ret);
});

// Run the shortest path algorithm and return the most profitable path between two currencies
// and return the most profitable exchange rate using said path
app.get('/arbitrageData/:startCurr/:endCurr/:maxNumberExchanges/:bankRate', function(req, res) {
    console.log("arbitrage data endpoint");

    if (req.query.exclude === undefined){
        req.query.exclude = [];
    }

    ret = addon.arbitrageData(req.params.startCurr, req.params.endCurr, req.params.maxNumberExchanges, req.query.exclude, req.params.bankRate);

    res.send(ret);
});


// Return the most recent exchange rate between two currencies
app.get('/calculatorData/:startCurr/:endCurr', function(req, res) {
    console.log("calculator data endpoint");

    ret = addon.calculatorData(req.params.startCurr, req.params.endCurr);

    if(isNaN(ret["rate"])){
        console.log("rate is not a number, returning 1.")
        ret["rate"] = 1;
    }

    res.send(ret);
});
