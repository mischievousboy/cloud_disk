const Http = require("http");
const redis = require('ioredis');
const client = new redis({
    host: 'redis-db',
    port: 6379
});

client.on("ready", function (res) {
    console.log("ready");
});


const server = new Http.Server((req, res) => {
    const authorization = req.headers["authorization"] || "";
    const extracted = authorization.split(" ");
    if (extracted.length === 2) {
        checkToken(extracted[0], extracted[1]).then((isValid) => {
            if (isValid) {
                res.writeHead(200, {"x-current-user": extracted[0]});
                return res.end();
            } else
                Write403(res);
        }).catch((err) => {
            console.error(err);
            Write403(res);
        })
    } else {
        Write403(res);
    }
});

const port = process.env.PORT || 9002;
server.listen(port);
console.log(`starting HTTP server on: ${port}`);

function checkToken(uid, token) {
    return new Promise((resolve, reject) => {
        client.get(uid, function (err, data) {
            console.log(data);
            if (err) {
                reject(err)
            } else {
                resolve(data === token);
            }
        })
    })
}


function Write403(res) {
    res.writeHead(403);
    res.end();
}