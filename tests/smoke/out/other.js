var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./');

loadDependentSymbols();

_library.other_struct = Struct({});
_library.other_struct.size = 1;

_library._preload['other_struct'] = ['dependent', 'dependent', 'stat', 'stat', function () {
    _library.other_struct.size = 0;
    _library.other_struct.defineProperty("dep", _library.dependent);
    _library.other_struct.defineProperty("gs", _library.stat);
}];

_library._preload['yet_another_function'] = [function () {
    _library.yet_another_function = ['void', [ref.refType('void'), 'int']];
    _library._functions['yet_another_function'] = _library.yet_another_function;
}];

function loadDependentSymbols() {
    require('./used.js');
}
