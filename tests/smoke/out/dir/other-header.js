var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./../');

loadDependentSymbols();

_library.yet_another_struct = Struct({});
_library.yet_another_struct.size = 1;

_library._preload['yet_another_struct'] = ['stat', 'stat', function () {
    _library.yet_another_struct.size = 0;
    _library.yet_another_struct.defineProperty("abc", _library.stat);
}];

function loadDependentSymbols() {
}
