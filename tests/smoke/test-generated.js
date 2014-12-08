var lib_path = process.argv[2];

var mod = require('./out');
mod.loadDependentSymbols();
mod.loadAllBindings();
mod(lib_path);