/**
 * Module dependencies.
 */

const addon = require('./src/build/Release/addon');

var express = require('express');
var path = require('path');
var fs = require('fs');
var logger = require('morgan');
var exec = require('child_process').exec;
var bodyParser = require('body-parser');
var app = express();

app.use(logger('dev'));
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public')));  //all html in public
app.use(function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});


console.log("__dirname: "+__dirname);

app.listen(3000, function () {
  console.log('photosbyjac.com node server listening on port 3000');
});

app.get('/exchange/:id', function(req, res) {

    // log id
    console.log(req.params.id);

    ret = addon.exchange(req.params.id);
    console.log(ret);
    
    res.send(ret);
});

app.get('/table/:tableName', function(req, res) {
    // log table name
    console.log(req.params.tableName);

    ret = addon.table(req.params.tableName);
    console.log(ret);

    res.send(ret);
});

app.get('/tickerData/', function(req, res) {
    // function takes 0 args (automatic load)

    ret = addon.tickerData();
    console.log(ret);
    
    res.send(ret);

});

app.get('/chartData/:ticker/:timeframe/:startDate/:endDate', function(req, res) {
    // log chartId
    console.log(req.params.ticker);
    console.log(req.params.timeframe);
    console.log(req.params.startDate);
    console.log(req.params.endDate);

    ret = addon.chartData(req.params.ticker,req.params.timeframe,req.params.startDate,req.params.endDate);
    console.log(ret);

    res.send(ret);
});