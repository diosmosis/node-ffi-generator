#include <ffigen/process/code_entity/function_pointer_entity.hpp>
#include <sstream>

namespace ffigen
{
    std::string function_pointer_entity::ffi_reference() const
    {
        if (_ffi_reference.empty()) {
            std::stringstream ss;

            ss << "ffi.Function(" << _return_type.ffi_reference() << ", [";

            bool is_first = true;
            for (auto const& param_type : _arguments) {
                if (is_first) {
                    is_first = false;
                } else {
                    ss << ", ";
                }

                ss << param_type.ffi_reference();
            }

            ss << "])";

            _ffi_reference = ss.str();
        }

        return _ffi_reference;
    }
}