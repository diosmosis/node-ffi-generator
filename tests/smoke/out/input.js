var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./');

loadDependentSymbols();

_library.the_dependent = _library.dependent;

_library.my_enum = {
    five: 555,
    VAR_THREE: 24,
    VAR_TWO: 23,
    VAR_FOUR: 25,
    VAR_ONE: -1
};

_library.my_function = ['double', ['int32', 'int', ref.refType('uint'), 'float']];
_library._functions['my_function'] = _library.my_function;

_library.my_other_function = [ref.refType('double'), [ref.refType('int32'), ref.refType('int'), ref.refType(ref.refType('uint')), ref.refType('float')]];
_library._functions['my_other_function'] = _library.my_other_function;

_library.nothing = Struct({});

_library._preload.push(function () {
    _library.nothing.defineProperty("__ignore", 'char');
});

_library.nill = _library.nothing;

_library.zilch = _library.nill;

_library.something = Struct({});

_library._preload.push(function () {
    _library.something.defineProperty("another", 'int32');
    _library.something.defineProperty("dependent", _library.dependent);
    _library.something.defineProperty("field", 'int');
    _library.something.defineProperty("firstfield", RefArray('int', 4));
    _library.something.defineProperty("message", ref.refType('char'));
    _library.something.defineProperty("nillo", _library.nothing);
});

_library.flying_struct = Struct({});

_library._preload.push(function () {
    _library.flying_struct.defineProperty("identity", 'longlong');
    _library.flying_struct.defineProperty("object", _library.something);
    _library.flying_struct.defineProperty("recursive_pointer", ref.refType(_library.flying_struct));
});

_library.variant1 = Union({});

_library._preload.push(function () {
    _library.variant1.defineProperty("val1", _library.flying_struct);
    _library.variant1.defineProperty("val2", _library.something);
    _library.variant1.defineProperty("val3", _library.zilch);
});

_library.variant2 = Union({});

_library._preload.push(function () {
    _library.variant2.defineProperty("gs", _library.stat);
    _library.variant2.defineProperty("str", Struct({}));
    _library.variant2.fields.str.type.defineProperty("length", 'ulong');
    _library.variant2.fields.str.type.defineProperty("text", ref.refType('char'));
    _library.variant2.defineProperty("v1", _library.variant1);
    _library.variant2.defineProperty("v2", RefArray('int', 5));
});

_library.my_function_pointer = ffi.Function('void', ['int', ref.refType('int'), ref.refType(_library.flying_struct)]);

_library.my_other_function_pointer = ffi.Function('uint', ['double', 'double', 'longlong']);

_library.my_undefined_struct_typedef = 'void';

function loadDependentSymbols() {
    require('./used.js');
}