#!/usr/bin/env node

const exec = require('child_process').exec;
const FQBN = "adafruit:samd:adafruit_pygamer_m4";
var PORT = "";

// execute a command and get output
function execute(command, callback){
    exec(command, function(error, stdout, stderr) { callback(stdout, stderr); });
};


if (require.main === module) {
    // get port
    execute("arduino-cli board list", (output, stderr) => {
        if (stderr.length !== 0) {
            console.log("Failed to locate device.");
            process.exitCode = -1;
            process.exit();
        }

        var devices = output.split('\n');

        devices.forEach((str, i) => {
            if (str.includes(FQBN)) {
                PORT = str.split(' ', 1)[0];
            }
        });

        console.log("\x1b[32m [✔] Port Found\x1b[0m");

        // compile
        execute("arduino-cli compile --fqbn " + FQBN + " .", (output, stderr) => {
            if (stderr.length !== 0) {
                console.log(stderr);
                process.exitCode = -1;
                process.exit();
            }

            console.log("\x1b[32m [✔] Compiled\x1b[0m");

            // upload
            execute("arduino-cli upload --fqbn " + FQBN + " --port " + PORT, (output, stderr) => {
                if (stderr.length !== 0) {
                    console.log(stderr);
                    process.exitCode = -1;
                    process.exit();
                }

                console.log("\x1b[32m [✔] Uploaded\x1b[0m");
            });
        });
    });
}