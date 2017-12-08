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

app.get('/stream', function(req, res){
    res.sendFile(__dirname + '/stream.html');
});

app.get('/alldata', function(req, res){
    res.sendFile(__dirname + '/alldata.html');
});

app.get('/new-temp-data', function(req, res){
    var tempLastDate = new Date(req.query.date);
    console.log(tempLastDate);
    MongoClient.connect(url, function(err, db){
        if(err){
            throw err;
        }
        console.log("Database connected");
        db.collection("Temperature").find({date: {$gte: tempLastDate}}).toArray(function(err, tempResult){
            if(err){
                throw err;
            }
            /*console.log(result);*/
            res.send(tempResult);
        });
        db.close();
    });
});

app.get('/new-moist-data', function(req, res){
    var moistLastDate = new Date(req.query.date);
    console.log(moistLastDate);
    MongoClient.connect(url, function(err, db){
        if(err){
            throw err;
        }
        console.log("Database connected");
        db.collection("Moisture").find({date: {$gte: moistLastDate}}).toArray(function(err, moistResult){
            if(err){
                throw err;
            }
            /*console.log(result);*/
            res.send(moistResult);
        });
        db.close();
    });
});

app.get('/last-temp-data', function(req, res){
    MongoClient.connect(url, function(err, db){
        var allTempLastData = [];
        if(err){
            throw err;
        }
        console.log("Database connected");
        db.collection("Temperature").find({sensor: 0}).limit(1).sort({$natural: -1}).toArray(function(err, lastResultZero){
            if(err){
                throw err;
            }
            allTempLastData.push(lastResultZero);
            db.collection("Temperature").find({sensor: 1}).limit(1).sort({$natural: -1}).toArray(function(err, lastResultOne){
                if(err){
                    throw err;
                }
                allTempLastData.push(lastResultOne);
                db.collection("Temperature").find({sensor: 2}).limit(1).sort({$natural: -1}).toArray(function(err, lastResultTwo){
                    if(err){
                        throw err;
                    }
                    allTempLastData.push(lastResultTwo);
                    res.send(allTempLastData);
                    db.close();
                });

            });
                /*console.log(result);*/
            /*res.send(result);*/
        });
        /*db.close();*/
    });
});

app.get('/last-moist-data', function(req, res){
    MongoClient.connect(url, function(err, db){
        var allMoistLastData = [];
        if(err){
            throw err;
        }
        console.log("Database connected");
        db.collection("Moisture").find({sensor: 0}).limit(1).sort({$natural: -1}).toArray(function(err, lastResultZero){
            if(err){
                throw err;
            }
            allMoistLastData.push(lastResultZero);
            db.collection("Moisture").find({sensor: 1}).limit(1).sort({$natural: -1}).toArray(function(err, lastResultOne){
                if(err){
                    throw err;
                }
                allMoistLastData.push(lastResultOne);
                db.collection("Moisture").find({sensor: 2}).limit(1).sort({$natural: -1}).toArray(function(err, lastResultTwo){
                    if(err){
                        throw err;
                    }
                    allMoistLastData.push(lastResultTwo);
                    res.send(allMoistLastData);
                    db.close();
                });

            });
            /*console.log(result);*/
            /*res.send(result);*/
        });
        /*db.close();*/
    });
});

app.get('/temperature-data', function(req, res){
    var sensorData = [];
    MongoClient.connect(url, function(err, db){
        if(err){
            throw err;
        }
        console.log("Database connected");
        db.collection("Temperature").find({sensor: 0}).limit(50).sort({$natural: -1}).toArray(function(err, resultZero){
           if(err){
               throw err;
           }
           sensorData.push(resultZero);
            db.collection("Temperature").find({sensor: 1}).limit(50).sort({$natural: -1}).toArray(function(err, resultOne) {
                if (err) {
                    throw err;
                }
                sensorData.push(resultOne);
                db.collection("Temperature").find({sensor: 2}).limit(50).sort({$natural: -1}).toArray(function (err, resultTwo) {
                    if (err) {
                        throw err;
                    }
                    sensorData.push(resultTwo);
                    console.log(sensorData[0]);
                    console.log(sensorData[1]);
                    console.log(sensorData[2]);
                    res.send(sensorData);
                    db.close();
                });
            });
           /*console.log(result);*/
           /*res.send(result);*/
        });
        /*db.close();*/
    });
});

app.get('/moisture-data', function(req, res){
    var sensorData = [];
    MongoClient.connect(url, function(err, db){
        if(err){
            throw err;
        }
        console.log("Database connected");
        db.collection("Moisture").find({sensor: 0}).limit(50).sort({$natural: -1}).toArray(function(err, resultZero){
            if(err){
                throw err;
            }
            sensorData.push(resultZero);
            db.collection("Moisture").find({sensor: 1}).limit(50).sort({$natural: -1}).toArray(function(err, resultOne) {
                if (err) {
                    throw err;
                }
                sensorData.push(resultOne);
                db.collection("Moisture").find({sensor: 2}).limit(50).sort({$natural: -1}).toArray(function (err, resultTwo) {
                    if (err) {
                        throw err;
                    }
                    sensorData.push(resultTwo);
                    console.log(sensorData[0]);
                    console.log(sensorData[1]);
                    console.log(sensorData[2]);
                    res.send(sensorData);
                    db.close();
                });
            });
            /*console.log(result);*/
            /*res.send(result);*/
        });
        /*db.close();*/
    });
});