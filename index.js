#!/user/bin/env node

const chalk = require('chalk');
const clear = require('clear');
const figlet = require('figlet');
const files = require('./files');

clear();

console.log(
    chalk.yellow(figlet.textSync('StylezSort', { horizontalLayout: 'full' }))
);

const currentDir = files.getCurrentDirectoryBase();
console.log(currentDir + '/');
files.stylesExists('./', '.css');