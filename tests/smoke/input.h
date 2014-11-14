#include <stdint.h>

/*
 * things to check:
 *    stdint typedefs            (YES!)
 *    basic function             (YES!)
 *    normal int types
 *    enum                       (YES!)
 *    parameter types
 *    pointer types
 *    string types
 *    array types
 *    typedef
 *    struct
 *    union
 *    anonymous struct
 *    anonymous union
 *    function pointer?
 *    const/volatile are removed
 */

enum my_enum {
    VAR_ONE = -1,

    VAR_TWO = 23,

    VAR_THREE,

    VAR_FOUR,

    five =555
};

int16_t my_function(my_enum param);