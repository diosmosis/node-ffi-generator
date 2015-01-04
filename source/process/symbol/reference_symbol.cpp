#include <ffigen/process/symbol/reference_symbol.hpp>
#include <ffigen/process/symbol/function_pointer_symbol.hpp>

namespace ffigen
{
    std::string reference_symbol::ffi_reference() const
    {
        if (_ffi_reference.empty()) {
            if (_underlying) {
                if (_underlying.is_a<function_pointer_symbol>()) {
                    _ffi_reference = _underlying.ffi_reference();
                } else {
                    _ffi_reference = "ref.refType(" + _underlying.ffi_reference() + ")";
                }
            } else {
                _ffi_reference = "'pointer'";
            }
        }

        return _ffi_reference;
    }
}