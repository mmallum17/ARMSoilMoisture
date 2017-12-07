var express = require("express");
var app = express();
var http = require("http").Server(app);
var io = require("socket.io")(http);

http.listen("80", "0.0.0.0", 1000000, function(){
    console.log("connected");
});

app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
});