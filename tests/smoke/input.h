#include <stdint.h>
#include <stddef.h>
#include "other.h"

/*
 * things to check:
 *    stdint typedefs            (YES!)
 *    basic function             (YES!)
 *    normal int types           (YES!)
 *    enum                       (YES!)
 *    parameter types            (YES!)
 *    pointer types              (YES!)
 *    string types               (skipping for now)
 *    array types                (YES!)
 *    typedef                    (YES!)
 *    struct                     (YES!)
 *    union                      (YES!)
 *    anonymous struct           (YES!)
 *    anonymous union            (YES!)
 *    const/volatile are removed (YES!)
 */

enum my_enum {
    VAR_ONE = -1,

    VAR_TWO = 23,

    VAR_THREE,

    VAR_FOUR,

    five =555
};

double my_function(enum my_enum param, int another, unsigned int * third, float fourth);

double * my_other_function(enum my_enum param[], int another[], unsigned int * third[], float fourth[]);

typedef int myspecialint;

struct nothing {};

typedef struct nothing nill;
typedef nill zilch;

typedef struct {
    int field;
    enum my_enum another;
    int firstfield[4];
    char const* message;
    struct nothing nillo;
} something;

struct flying_struct
{
    long long identity;
    something object;
};

typedef union {
    struct flying_struct val1;
    something val2;
    zilch val3;
} variant1;

union variant2 {
    variant1 v1;
    int v2[5];
    struct {
        char const* text;
        size_t length;
    } str;
};