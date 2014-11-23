#ifndef OTHER_H
#define OTHER_H

#include "used.h"
#include <sys/stat.h>

struct other_struct {
    struct dependent dep;
    struct stat gs;
};

void yet_another_function(void * thing, int bob);

#endif 