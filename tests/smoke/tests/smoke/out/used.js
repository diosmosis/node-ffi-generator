var ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./');

_library.dependent = Struct({
    "field1": 'int',
    "field2": 'int'
});

_library.my_predefine = _library.dependent;

