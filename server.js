const fs = require('fs');
const express = require("express");
const app = express();
const {exec} = require('child_process');

app.get("/run", (req, res) => {
    fs.writeFile("code.txt", req.key + '\n' + req.sentence, (err, data) => {
        if (err) {
            throw err;
        }
    });
    exec(`./cipher ${act} code.txt`, (error, stdout, stderr) => {
        if (error) {
            console.log(`error: ${error.message}`);
            return;
        }
        if (stderr) {
            console.log(`stderr: ${stderr}`);
            return;
        }
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.write(stdout);
        return res.end();
    });
});

app.listen(2000);