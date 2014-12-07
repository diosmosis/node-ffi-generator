var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./../');

loadDependentSymbols();

_library.yet_another_struct = Struct({});

_library._preload.push(function () {
    _library.yet_another_struct.defineProperty("abc", _library.stat);
});

function loadDependentSymbols() {
}