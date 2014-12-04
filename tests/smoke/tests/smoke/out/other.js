var ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./');

_library.other_struct = Struct({
    "dep": _library.dependent,
    "gs": _library.stat
});

_library.yet_another_function = ['void', [ref.refType('void'), 'int']];
_library._functions.push(_library.yet_another_function);

