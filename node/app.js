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
    // should this not take any params? just call function like this:
    // ret = addon.TickerData();
    console.log(ret);
    
    // for frontend dev
    // ret = { "0" : [{"ticker":"USDEUR=X", "percentChange": "0.12"},
    //     {"ticker":"USDCAD=X", "percentChange": "0.77"},
    //     {"ticker":"JPYEUR=X", "percentChange": "0.89"},
    //     {"ticker":"AUDEUR=X", "percentChange": "-0.55"},
    //     {"ticker":"NZDJPY=X", "percentChange": "0.64"},
    //     {"ticker":"CHFEUR=X", "percentChange": "-1.90"},
    //     {"ticker":"GBPNZD=X", "percentChange": "0.43"}]
    // }

    res.send(ret);

});

app.get('/chartData/:ticker/:timeframe', function(req, res) {
    // log chartId
    console.log(req.params.ticker);
    console.log(req.params.timeframe);

    ret = addon.chartData(req.params.ticker,req.params.timeframe);
    console.log(ret);

    res.send(ret);
});