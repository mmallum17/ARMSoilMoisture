const net = require('net');

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