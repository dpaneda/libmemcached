/* LibMemcached
 * Copyright (C) 2006-2010 Brian Aker
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license.  See
 * the COPYING file in the parent directory for full text.
 *
 * Summary: 
 *
 */

#include "common.h"

void memcached_server_response_increment(memcached_server_write_instance_st instance, uint32_t opaque)
{
	if (instance->root->flags.check_opaque) {
		if (instance->cursor_active == 0) {
			instance->pending_ops = malloc(sizeof(*instance->pending_ops));
		} else {
			instance->pending_ops = realloc(instance->pending_ops,
					(instance->cursor_active + 1) * sizeof(*instance->pending_ops));
		}
		instance->pending_ops[instance->cursor_active] = opaque;
	}
	instance->cursor_active++;
}

bool memcached_server_response_decrement(memcached_server_write_instance_st instance, uint32_t opaque)
{
	unsigned i;
	if (instance->root->flags.check_opaque) {
		for (i=0; i<instance->cursor_active; i++) {
			if (instance->pending_ops[i] == opaque) {
				if (instance->cursor_active - i - 1 > 0) {
					memmove(&instance->pending_ops[i], &instance->pending_ops[i+1],
							sizeof(uint32_t) * (instance->cursor_active - i - 1));
				}
				instance->cursor_active--;
				if (instance->cursor_active == 0) {
					free(instance->pending_ops);
					instance->pending_ops = NULL;
				}
				return 1;
			}
		}
	} else {
		instance->cursor_active--;
		return 1;
	}
	return 0;
}

void memcached_server_response_reset(memcached_server_write_instance_st instance)
{
	if (instance->pending_ops) {
		free(instance->pending_ops);
		instance->pending_ops = NULL;
	}
	instance->cursor_active = 0;
}
