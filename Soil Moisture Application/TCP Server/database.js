const MongoClient = require('mongodb').MongoClient;

let url = "mongodb://mmallum:soccer17@ds033196.mlab.com:33196/soil_moisture";

MongoClient.connect(url, function(err, db){
    if(err){
        throw err;
    }
    console.log("Database connected");
    let myobj = { date: new Date(), sensor: 0, value: 544.3};
    db.collection("Moisture").insertOne(myobj, function(err, res) {
        if(err){
            throw err;
        }
        console.log("Inserted");
    });
    db.close();
});