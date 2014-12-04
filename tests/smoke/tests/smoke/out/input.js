var ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./');

_library.the_dependent = _library.dependent;

_library.my_enum = {
    five: 555,
    VAR_THREE: 24,
    VAR_TWO: 23,
    VAR_FOUR: 25,
    VAR_ONE: -1
};

_library.my_function = ['double', ['int32', 'int', ref.refType('uint'), 'float']];
_library._functions.push(_library.my_function);

_library.my_other_function = [ref.refType('double'), [ref.refType('int32'), ref.refType('int'), ref.refType(ref.refType('uint')), ref.refType('float')]];
_library._functions.push(_library.my_other_function);

_library.nothing = Struct({

});

_library.nill = _library.nothing;

_library.zilch = _library.nill;

_library.something = Struct({
    "another": 'int32',
    "dependent": _library.dependent,
    "field": 'int',
    "firstfield": RefArray('int', 4),
    "message": ref.refType('char'),
    "nillo": _library.nothing
});

_library.flying_struct = Struct({
    "identity": 'longlong',
    "object": _library.something
});

_library.variant1 = Union({
    "val1": _library.flying_struct,
    "val2": _library.something,
    "val3": _library.zilch
});

_library.variant2 = Union({
    "gs": _library.stat,
    "str": Struct({
        "length": 'ulong',
        "text": ref.refType('char')
    });,
    "v1": _library.variant1,
    "v2": RefArray('int', 5)
});

_library.my_function_pointer = ref.refType(ffi.Function('void', ['int', ref.refType('int'), ref.refType(_library.flying_struct)]););

_library.my_other_function_pointer = ref.refType(ffi.Function('uint', ['longdouble', 'longdouble', 'longlong']););

