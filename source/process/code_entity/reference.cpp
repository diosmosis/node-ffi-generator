#include <ffigen/process/code_entity/reference.hpp>
#include <ffigen/process/code_entity/function_pointer_entity.hpp>

namespace ffigen
{
    std::string reference_entity::ffi_reference() const
    {
        if (_ffi_reference.empty()) {
            if (_underlying) {
                if (_underlying.is_a<function_pointer_entity>()) {
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