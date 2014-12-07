var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./');

loadDependentSymbols();

_library.dependent = Struct({});

_library._preload.push(function () {
    _library.dependent.defineProperty("field1", 'int');
    _library.dependent.defineProperty("field2", 'int');
});

_library.my_predefine = _library.dependent;

function loadDependentSymbols() {
}