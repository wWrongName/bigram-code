let act = "enc";
let encDec = function () {
    fs.writeFile("code.txt", document.getElementById('key').value + '\n' + document.getElementById('sentence').value, (err, data) => {
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
        document.getElementById('result').value = stdout;
    });
};

document.getElementById('enc').onclick = function () {
    document.getElementById('enc0').class = "btn btn-primary active";
    document.getElementById('dec0').class = "btn btn-primary";
    act = "enc";
};

document.getElementById('dec').onclick = function () {
    document.getElementById('dec0').class = "btn btn-primary active";
    document.getElementById('enc0').class = "btn btn-primary";
    act = "dec";
};

document.getElementById('run').onclick = function () {
    encDec();
};