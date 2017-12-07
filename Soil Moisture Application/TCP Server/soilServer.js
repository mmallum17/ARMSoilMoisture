const net = require('net');
/*const MongoClient = require('mongodb').MongoClient;

let url = "mongodb://mmallum:soccer17@ds033196.mlab.com:33196/soil_moisture";

MongoClient.connect(url, function(err, db){
   if(err){
       throw err;
   }
   console.log("Database created")
});*/



/*Start Server*/
const server = net.createServer((c) => {
    // 'connection' listener
    console.log('client connected');

c.on('end', () => {
    console.log('client disconnected');
});
c.on('data', function (chunk) {
    console.log(chunk.toString());
});
});
server.on('error', (err) => {
    throw err;
});
server.listen(4000, () => {
    console.log('server bound');
});
