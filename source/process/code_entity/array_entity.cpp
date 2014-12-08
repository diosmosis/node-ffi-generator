#include <ffigen/process/code_entity/array_entity.hpp>

namespace ffigen
{
    std::string array_entity::ffi_reference() const
    {
        if (_ffi_reference.empty()) {
            std::stringstream ss;
            ss << "_library.__RefArray(" << _underlying.ffi_reference() << ", " << _size << ")";
            _ffi_reference = ss.str();
        }

        return _ffi_reference;
    }
}