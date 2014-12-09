#include <ffigen/process/code_entity/record.hpp>
#include <ffigen/process/code_entity/typedef.hpp>

namespace ffigen
{
    static code_entity const* get_typedef_record(code_entity const& entity)
    {
        if (entity.is_a<typedef_entity>()) {
            code_entity const& alias_type = entity.cast<typedef_entity>().alias_type();
            return get_typedef_record(alias_type);
        } else if (entity.is_a<record_entity>()) {
            return &entity;
        } else {
            return nullptr;
        }
    }

    void record_entity::fill_dependents() const
    {
        for (auto const& pair : _members) {
            _dependents.push_back(&pair.second);

            code_entity const* typedef_record = get_typedef_record(pair.second);
            if (typedef_record) {
                dependent_records.push_back(*typedef_record);
                _dependents.push_back(&dependent_records.back());
            }
        }
    }
}