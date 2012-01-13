/*
 * Version of fnv1 created to make libmemcached hashing compatible with PHP
 * memcache extension which is pretty broken in that mather because of
 * post-processing of the hash returned by the original hash function.
 *
 */

#include "common.h"
#include "algorithm.h"

uint32_t hashkit_fnv1a_compat (const char *key, size_t key_length, void *context __attribute__((unused)))
{
  uint32_t hash= hashkit_fnv1a_32(key, key_length, NULL);

  hash = (hash >> 16) & 0x7fff;
  hash = hash ? hash : 1;

  return hash;
}
