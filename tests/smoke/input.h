#include <stdint.h>

/*
 * things to check:
 *    stdint typedefs            (YES!)
 *    basic function             (YES!)
 *    normal int types           (YES!)
 *    enum                       (YES!)
 *    parameter types            (YES!)
 *    pointer types              (YES!)
 *    string types               (skipping for now)
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

double my_function(enum my_enum param, int another, unsigned int * third, float fourth);

double[] my_other_function(my_enum param[], int another[], unsigned int * third[], float fourth[]);
