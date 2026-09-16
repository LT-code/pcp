/*
 * perfmanager interface
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

#ifndef PERFMANAGER_H_
#define PERFMANAGER_H_

#include <stdint.h>
#include "perfinterface.h"

typedef intptr_t perfmanagerhandle_t;

perfmanagerhandle_t *manager_init(const char *configfilename);

void manager_destroy(perfmanagerhandle_t *mgr);

int perf_get_r(perfmanagerhandle_t *inst, perf_counter **data, int *size, perf_derived_counter **derived_counter, int *derived_size);

/* The state the counters are actually in right now, which is the state
 * requested with pmStore unless an external perfalloc(1) lock overrides it.
 */
int perf_enabled(perfmanagerhandle_t *inst);

/* Whether a perfalloc(1) read lock is currently held on the lock file.
 * While it is, the counters stay disabled whatever was requested.
 */
int perf_lock_held(perfmanagerhandle_t *inst);

/* Global on/off switch, as requested with pmStore.  Takes effect
 * immediately rather than on the next poll of the lock file.
 */
int perf_counter_request_enable(perfmanagerhandle_t *inst, int enable);
int perf_counter_desired(perfmanagerhandle_t *inst);

/* Per-counter on/off switch for counters opened during startup, indexed
 * as in the array returned by perf_get_r().
 */
int perf_counter_request_enable_one(perfmanagerhandle_t *inst, int idx, int enable);
int perf_counter_desired_one(perfmanagerhandle_t *inst, int idx);

#endif // PERFMANAGER_H_
