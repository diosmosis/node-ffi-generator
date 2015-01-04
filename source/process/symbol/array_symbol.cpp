#include <ffigen/process/symbol/array_symbol.hpp>

namespace ffigen
{
    std::string array_symbol::ffi_reference() const
    {
        if (_ffi_reference.empty()) {
            std::stringstream ss;
            ss << "_library.__RefArray(" << _underlying.ffi_reference() << ", " << _size << ")";
            _ffi_reference = ss.str();
        }

        return _ffi_reference;
    }
}