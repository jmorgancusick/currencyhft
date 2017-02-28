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

app.get('/db/:table', function(req, res) {

    // log table name
    console.log(req.params.table);

    ret = addon.hello();
    console.log(ret);
    
    res.send(ret);
});
