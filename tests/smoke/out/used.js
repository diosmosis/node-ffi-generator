var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./');

loadDependentSymbols();

_library.dependent = Struct({});
_library.dependent.size = 1;

_library._preload['dependent'] = [function () {
    _library.dependent.size = 0;
    _library.dependent.defineProperty("field1", 'int');
    _library.dependent.defineProperty("field2", 'int');
}];

_library._preload['my_predefine'] = [function () {
    _library.my_predefine = _library.dependent;
}];

function loadDependentSymbols() {
}
