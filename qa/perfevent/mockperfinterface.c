#include "perfinterface.h"
#include <stdlib.h>
#include <stdio.h>

perfhandle_t *perf_event_create(const char *configfile)
{
    return malloc(1);
}

void perf_counter_destroy(perf_counter *data, int size,
                          perf_derived_counter *derived_counter, int derived_size)
{
}

void perf_event_destroy(perfhandle_t *inst)
{
    free(inst);
}

int perf_counter_enable(perfhandle_t *inst, int enable)
{
    fprintf(stderr,"perf_counter_enable -> %s\n", (enable == PERF_COUNTER_ENABLE) ? "ENABLED" : "DISABLED" );
    return 0;
}

int perf_counter_enable_one(perfhandle_t *inst, int idx, int enable)
{
    fprintf(stderr,"perf_counter_enable_one(%d) -> %s\n", idx,
            (enable == PERF_COUNTER_ENABLE) ? "ENABLED" : "DISABLED" );
    return 0;
}

int perf_counter_set_user_enabled(perfhandle_t *inst, int idx, int enabled)
{
    fprintf(stderr,"perf_counter_set_user_enabled(%d, %d)\n", idx, enabled);
    return 0;
}

int perf_counter_get_user_enabled(perfhandle_t *inst, int idx)
{
    return 1;
}

int perf_get(perfhandle_t *inst, perf_counter **data, int *size,
             perf_derived_counter **derived_counter, int *derived_size)
{
    return -E_PERFEVENT_RUNTIME;
}

const char *perf_strerror(int err)
{
    return "fake error";
}
