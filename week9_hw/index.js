var express = require("express");
const app = express();

var bodyParser = require("body-parser");

app.use(bodyParser.json());
console.log("this is process.env.port" + process.env.PORT);

var readings = {
    Age: 24,
    BirthDay: 2,
    BirthMonth: 9,
    BirthYear: 1997
};

app.get("/queryTest", (request, response) => {
    // send the JSON object as a string response
    console.log(request.query);
    response.end("Howdy there");

});

app.post("/bodyTest", (request, response) => {
    // send the JSON object as a string response
    console.log(request.body);
    response.end("Hello World!");
});

// sget the readings and return to the client:
app.get("/data", (request, response) => {
    // send the JSON object as a string response
    response.json(readings);
});

// send the client's stats back to them:
app.get("/hello", (request, response) => {
    console.log(request.connection.remoteAddress);
    console.log(request.headers);
    // send the JSON object as a string response
    response.end("Hi!");
});

// listen for requests :)
app.listen(process.env.PORT || 80);