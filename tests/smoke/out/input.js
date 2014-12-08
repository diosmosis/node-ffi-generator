var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union'),
    _library = require('./');

loadDependentSymbols();

_library._preload['the_dependent'] = [function () {
    _library.the_dependent = _library.dependent;
}];

_library.my_enum = {
    five: 555,
    VAR_THREE: 24,
    VAR_TWO: 23,
    VAR_FOUR: 25,
    VAR_ONE: -1
};

_library._preload['my_function'] = [function () {
    _library.my_function = ['double', ['int32', 'int', ref.refType('uint'), 'float']];
    _library._functions['my_function'] = _library.my_function;
}];

_library._preload['my_other_function'] = ['pointer', function () {
    _library.my_other_function = [ref.refType('double'), [ref.refType('int32'), ref.refType('int'), ref.refType(ref.refType('uint')), ref.refType('float')]];
    _library._functions['my_other_function'] = _library.my_other_function;
}];

_library.nothing = Struct({});
_library.nothing.size = 1;

_library._preload['nothing'] = [function () {
    _library.nothing.size = 0;
    _library.nothing.defineProperty("__ignore", 'char');
}];

_library._preload['nill'] = [function () {
    _library.nill = _library.nothing;
}];

_library._preload['zilch'] = ['nill', function () {
    _library.zilch = _library.nill;
}];

_library.something = Struct({});
_library.something.size = 1;

_library._preload['something'] = ['dependent', 'dependent', 'nothing', 'nothing', function () {
    _library.something.size = 0;
    _library.something.defineProperty("another", 'int32');
    _library.something.defineProperty("dependent", _library.dependent);
    _library.something.defineProperty("field", 'int');
    _library.something.defineProperty("firstfield", _library.__RefArray('int', 4));
    _library.something.defineProperty("message", ref.refType('char'));
    _library.something.defineProperty("nillo", _library.nothing);
}];

_library.flying_struct = Struct({});
_library.flying_struct.size = 1;

_library._preload['flying_struct'] = ['something', 'something', 'flying_struct', function () {
    _library.flying_struct.size = 0;
    _library.flying_struct.defineProperty("identity", 'longlong');
    _library.flying_struct.defineProperty("object", _library.something);
    _library.flying_struct.defineProperty("recursive_pointer", ref.refType(_library.flying_struct));
}];

_library._preload['invasion_force'] = [function () {
    _library.invasion_force = _library.__RefArray(_library.flying_struct, 64);
}];

_library.variant1 = Union({});
_library.variant1.size = 1;

_library._preload['variant1'] = ['flying_struct', 'flying_struct', 'something', 'something', 'zilch', 'nothing', function () {
    _library.variant1.size = 0;
    _library.variant1.defineProperty("val1", _library.flying_struct);
    _library.variant1.defineProperty("val2", _library.something);
    _library.variant1.defineProperty("val3", _library.zilch);
}];

_library.variant2 = Union({});
_library.variant2.size = 1;

_library._preload['variant2'] = ['stat', 'stat', 'variant1', 'variant1', function () {
    _library.variant2.size = 0;
    _library.variant2.defineProperty("gs", _library.stat);
    _library.variant2.defineProperty("str", (function () {
        var temp = Struct({});
        temp.defineProperty("length", 'ulong');
        temp.defineProperty("text", ref.refType('char'));
        return temp;
    })());
    _library.variant2.defineProperty("v1", _library.variant1);
    _library.variant2.defineProperty("v2", _library.__RefArray('int', 5));
}];

_library._preload['my_function_pointer'] = ['void (int, int *, struct flying_struct *)', function () {
    _library.my_function_pointer = ffi.Function('void', ['int', ref.refType('int'), ref.refType(_library.flying_struct)]);
}];

_library._preload['my_other_function_pointer'] = ['unsigned int (long double, long double, long long)', function () {
    _library.my_other_function_pointer = ffi.Function('uint', ['double', 'double', 'longlong']);
}];

_library._preload['my_undefined_struct_typedef'] = [function () {
    _library.my_undefined_struct_typedef = 'void';
}];

_library._preload['my_struct_function'] = ['variant1', function () {
    _library.my_struct_function = ['void', [ref.refType(_library.variant1)]];
    _library._functions['my_struct_function'] = _library.my_struct_function;
}];

function loadDependentSymbols() {
    require('./used.js');
}
