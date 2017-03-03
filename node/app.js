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

    // log table name
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
