var ffi = require('ffi'),
    ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union');

var exports = module.exports = function (path) {
    var _library = exports;
    while (_library._preload.length) {
        _library._preload.shift()();
    }
    return ffi.Library(path, _library._functions);
};

exports._functions = {};
exports._preload = [];

exports.loadDependentSymbols = function () {
    var _library = exports;
    _library.timespec = Struct({});
    
    _library._preload.push(function () {
        _library.timespec.defineProperty("tv_nsec", 'long');
        _library.timespec.defineProperty("tv_sec", 'long');
    });
    
    _library.stat = Struct({});
    
    _library._preload.push(function () {
        _library.stat.defineProperty("__glibc_reserved", RefArray('long', 3));
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
    });
    
    
};

exports.loadAllBindings = function () {
    require('./dir/other-header');
    require('./other');
    require('./input');
    require('./used');
};
