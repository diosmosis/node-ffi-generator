var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union');

var exports = module.exports = function (path) {
    var _library = exports;
    for (var key in _library._preload) {
        invoke_preload_function(key);
    }
    return ffi.Library(path, _library._functions);
};

exports._functions = {};
exports._preload = [];

// avoid prototype chain bug in ref-array that is triggered when an array is used in a struct
exports.__RefArray = function () {
    var result = RefArray.apply(null, arguments);
    result.toString = function () { return this.type.toString() + '[]'; };
    return result;
};

exports.loadDependentSymbols = function () {
    var _library = exports;
    _library.timespec = Struct({});
    _library.timespec.size = 1;
    
    _library._preload['timespec'] = [function () {
        _library.timespec.size = 0;
        _library.timespec.defineProperty("tv_nsec", 'long');
        _library.timespec.defineProperty("tv_sec", 'long');
    }];
    
    _library.stat = Struct({});
    _library.stat.size = 1;
    
    _library._preload['stat'] = ['timespec', 'timespec', 'timespec', 'timespec', 'timespec', 'timespec', function () {
        _library.stat.size = 0;
        _library.stat.defineProperty("__glibc_reserved", _library.__RefArray('long', 3));
        _library.stat.defineProperty("__pad0", 'int');
        _library.stat.defineProperty("st_atim", _library.timespec);
        _library.stat.defineProperty("st_blksize", 'long');
        _library.stat.defineProperty("st_blocks", 'long');
        _library.stat.defineProperty("st_ctim", _library.timespec);
        _library.stat.defineProperty("st_dev", 'ulong');
        _library.stat.defineProperty("st_gid", 'uint');
        _library.stat.defineProperty("st_ino", 'ulong');
        _library.stat.defineProperty("st_mode", 'uint');
        _library.stat.defineProperty("st_mtim", _library.timespec);
        _library.stat.defineProperty("st_nlink", 'ulong');
        _library.stat.defineProperty("st_rdev", 'ulong');
        _library.stat.defineProperty("st_size", 'long');
        _library.stat.defineProperty("st_uid", 'uint');
    }];
    
    
};

exports.loadAllBindings = function () {
    require('./dir/other-header');
    require('./other');
    require('./input');
    require('./used');
};

function invoke_preload_function(key) {
    var _library = exports;
    var entry = _library._preload[key];
    delete _library._preload[key];

    if (entry) {
        var func = entry.pop();

        // execute dependent preloads
        for (var i = 0; i != entry.length; ++i) {
            invoke_preload_function(entry[i]);
        }

        func();
    }
}
