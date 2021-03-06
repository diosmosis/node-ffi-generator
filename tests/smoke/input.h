typedef struct dependent the_dependent;

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "other.h"
#include "used.h"
#include <sys/stat.h>

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
 *    variadic functions         (not supported yet)
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
    struct dependent dependent;
} something;

struct flying_struct
{
    long long identity;
    something object;
    struct flying_struct * recursive_pointer;
};

typedef struct flying_struct invasion_force[64];

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
    struct stat gs;
};

typedef void (*my_function_pointer)(int, int *, struct flying_struct *);
typedef unsigned int (*my_other_function_pointer)(long double, long double, long long);

struct my_undefined_struct;

typedef struct my_undefined_struct my_undefined_struct_typedef; // should come out as 'void'

void my_struct_function(variant1 * v);

struct my_unused_struct
{
    int a;
};

int my_undefined_function(struct my_unused_struct * data, int n);

/* not supported yet
double my_variadic_function(char const* data, ...)
{
    va_list ap;
    va_start(ap, 5);
    va_end(ap);
}*/