/*
 * perfevent interface
 *
 * Copyright (c) 2013 Joe White
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
#ifndef PERFINTERFACE_H_
#define PERFINTERFACE_H_

#include <stdint.h>
#include <perfmon/pfmlib_perf_event.h>
#include "rapl-interface.h"
#include "architecture.h"

typedef struct perf_data_t_
{
    uint64_t value;
    uint64_t time_enabled;
    uint64_t time_running;
    int id;
} perf_data;

typedef struct perf_counter_t_
{
    char *name;
    int counter_disabled; /* unopened or disabled by the user */
    perf_data *data;
    int ninstances;
} perf_counter;

typedef struct perf_derived_data_t_
{
    double value;
} perf_derived_data;

typedef struct perf_counter_list_t_
{
    perf_counter *counter;
    double scale;
    struct perf_counter_list_t_ *next;
} perf_counter_list;

typedef struct perf_derived_counter_t_
{
    char *name;
    int counter_disabled; /* at least one source counter is unavailable */
    perf_derived_data *data;
    int ninstances;
    perf_counter_list *counter_list;
} perf_derived_counter;

typedef struct eventcpuinfo_t_ {
    uint64_t values[3];
    uint64_t previous[3];
    int type;
    int fd;
    perf_event_attr_t hw; /* perf_event_attr struct passed to perf_event_open() */
    int idx; /* opaque libpfm event identifier */
    char *fstr; /* fstr from library, must be freed */
    rapl_data_t rapldata;
    int cpu;
} eventcpuinfo_t;

typedef struct event_t_ {
    char *name;
    int disable_event;		/* no usable file descriptors for this event */
    eventcpuinfo_t *info;
    int ncpus;			/* number of cpus successfully opened */

    int user_enabled;		/* per-counter state requested via pmStore */

    /* Encoding and cpu placement, stashed so that an event which was not
     * opened during setup can still be opened later on demand.
     */
    uint32_t pmu_type;
    uint64_t config;
    uint64_t config1;
    uint64_t config2;
    int *cpuarr;
    int ncpus_configured;
} event_t;

typedef struct event_list_t_ {
    event_t *event;
    double scale;
    struct event_list_t_ *next;
} event_list_t;

typedef struct derived_event_t_ {
    char *name;
    event_list_t *event_list;
} derived_event_t;

typedef struct dynamic_event_t_ {
    char *pmu;
    char *event;
    struct dynamic_event_t_ *next;
} dynamic_event_t;

typedef struct perfdata_t_
{
    int nevents;
    event_t *events;

    int nderivedevents;
    derived_event_t *derived_events;

    /* information about the architecture (number of cpus, numa nodes etc) */
    archinfo_t *archinfo;

    /* internal state to keep track of cpus for events added in 'round
     * robin' mode */
    int roundrobin_cpu_idx;
    int roundrobin_nodecpu_idx;
} perfdata_t;

typedef intptr_t perfhandle_t;

perfhandle_t *perf_event_create(const char *configfile);

void perf_counter_destroy(perf_counter *data, int size, perf_derived_counter *derived_counter, int derived_size);

void perf_event_destroy(perfhandle_t *inst);

#define PERF_COUNTER_ENABLE 0
#define PERF_COUNTER_DISABLE 1
int perf_counter_enable(perfhandle_t *inst, int enable);

/* Apply the enable/disable ioctl to a single counter, identified by its
 * index in the array returned by perf_get().
 */
int perf_counter_enable_one(perfhandle_t *inst, int idx, int enable);

/* Per-counter state requested via pmStore.  perf_counter_enable() only
 * enables counters whose user_enabled flag is set.
 */
int perf_counter_set_user_enabled(perfhandle_t *inst, int idx, int enabled);
int perf_counter_get_user_enabled(perfhandle_t *inst, int idx);

/* Open a counter that was discovered but not configured at startup.  This
 * needs privileges the PMDA has usually dropped by now, so it is best effort;
 * see the -E option to open such counters while still running as root.
 * Already opened counters, including RAPL counters, are left alone.
 */
int perf_counter_open_late(perfhandle_t *inst, int idx);

/* Open every discovered dynamic event during setup, not just those listed in
 * the [dynamic] section of the configuration file.  Must be called before
 * perf_event_create().
 */
void perf_set_preopen(int preopen);

int perf_get(perfhandle_t *inst, perf_counter **data, int *size, perf_derived_counter **derived_counter, int *derived_size);

#define E_PERFEVENT_LOGIC 1
#define E_PERFEVENT_REALLOC 2
#define E_PERFEVENT_RUNTIME 3

const char *perf_strerror(int err);

#endif /* PERFINTERFACE_H_ */
