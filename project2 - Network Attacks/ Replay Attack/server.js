var host = process.argv[2] || '140.113.66.208';
var port = process.argv[3] || 5566;
var net = require('net');
//var fs = require("fs");

var server = net.createServer();
var client = new net.Socket();

server.listen(port, host);
console.log('TCP server listening on ' + host + ":" +port);

server.on('connection',  function(socket) {
	console.log(socket.remoteAddress + ":" + socket.remotePort + " " + Date());

	socket.setEncoding('binary');
	socket.on('data', function(message) {
		console.log('receive: ' + message);

		client.connect(49175, '140.113.194.85', function() {
			console.log('Connected to 140.113.194.85:49175');
			//fs.writeFile('packet', message, 'binary');

			//fs.readFile('packet_admin', function (err, data) {
			//	if (err) throw err;
			//	client.write(data);
			//});
			client.write(message);
		});
	});
});

client.on('data', function(data) {
	var response = data.toString().slice(0, -1);
	console.log(response);
	if (response != 'Login Success')
		process.stdout.write('>');
});

process.stdin.on('data', function(input) {	
	client.write(input);
});