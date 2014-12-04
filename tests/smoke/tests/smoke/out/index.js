var ref = require('ref'),
    RefArray = require('ref-array'),
    Struct = require('ref-struct'),
    Union = require('ref-union');

exports = function (path) {
    var _library = this;
    return ffi.Library(_library._functions);

};

exports._functions = [];

exports.loadDependentSymbols = function () {
    var _library = exports;
    _library.timespec = Struct({
        "tv_nsec": 'long',
        "tv_sec": 'long'
    });
    
    _library.stat = Struct({
        "__glibc_reserved": RefArray('long', 3),
        "__pad0": 'int',
        "st_atim": _library.timespec,
        "st_blksize": 'long',
        "st_blocks": 'long',
        "st_ctim": _library.timespec,
        "st_dev": 'ulong',
        "st_gid": 'uint',
        "st_ino": 'ulong',
        "st_mode": 'uint',
        "st_mtim": _library.timespec,
        "st_nlink": 'ulong',
        "st_rdev": 'ulong',
        "st_size": 'long',
        "st_uid": 'uint'
    });
    
    
};

exports.loadAllBindings = function () {
    require('./dir/other-header');
    require('./other');
    require('./input');
    require('./used');
};
