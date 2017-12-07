var express = require("express");
var app = express();
var http = require("http").Server(app);
var io = require("socket.io")(http);

const MongoClient = require('mongodb').MongoClient;

var url = "mongodb://mmallum:soccer17@ds033196.mlab.com:33196/soil_moisture";

http.listen("80", "0.0.0.0", 1000000, function(){
    console.log("connected");
});

app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
});

app.get('/data', function(req, res){
    MongoClient.connect(url, function(err, db){
        if(err){
            throw err;
        }
        console.log("Database connected");
        db.collection("Temperature").find({}).toArray(function(err, result){
           if(err){
               throw err;
           }
           /*console.log(result);*/
           res.send(result);
        });
        db.close();
    });
});