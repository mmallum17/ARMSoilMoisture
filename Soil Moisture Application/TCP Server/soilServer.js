const net = require('net');
const MongoClient = require('mongodb').MongoClient;

let url = "mongodb://mmallum:soccer17@ds033196.mlab.com:33196/soil_moisture";



/*Start Server*/
const server = net.createServer((c) => {
    // 'connection' listener
    console.log('client connected');

c.on('end', () => {
    console.log('client disconnected');
});
c.on('data', function (chunk) {
    console.log(chunk.toString());
    let tokens = chunk.toString().split(",");
    let moisture = parseFloat(tokens[0]);
    let temperature = parseFloat(tokens[1]);
    MongoClient.connect(url, function(err, db){
        if(err){
            throw err;
        }
        console.log("Database connected");
        let myobj = { date: new Date(), sensor: 0, value: moisture};
        db.collection("Moisture").insertOne(myobj, function(err, res) {
            if(err){
                throw err;
            }
            console.log("Moisture Inserted");
        });
        myobj = { date: new Date(), sensor: 0, value: temperature};
        db.collection("Temperature").insertOne(myobj, function(err, res) {
            if(err){
                throw err;
            }
            console.log("Temperature Inserted");
        });
        db.close();
    });
});
});
server.on('error', (err) => {
    throw err;
});
server.listen(4000, () => {
    console.log('server bound');
});
