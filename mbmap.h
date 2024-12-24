#ifndef MBMAP_H

#ifndef MBBASE_H
#include "mbbase.h"
#endif

#define Map_Make(key_type, value_type, map_type, hash_function, compare_function, invalid_key, nil_value) \
	typedef struct map_type { \
		struct { \
			key_type key; \
			value_type value; \
		} *cells; \
		usize length; \
		usize number_of_items; \
	} map_type; \
	\
	force_inline value_type map_type##_get(map_type *map, key_type search) { \
		assert((map->length & (map->length-1)) == 0); \
		usize hash = (usize)hash_function(search); \
		usize index = hash & (map->length - 1); \
		usize probe = 1; \
		\
		while (!compare_function(map->cells[index].key, invalid_key)) { \
			if (compare_function(map->cells[index].key, search)) { \
				return map->cells[index].value; \
			} \
			index = (index + probe) & (map->length - 1); \
			probe += 1; \
		} \
		\
		return nil_value; \
	} \
	\
	force_inline void map_type##_add(map_type *map, key_type k, value_type v) { \
		assert((map->length & (map->length-1)) == 0); \
		usize hash = (usize)hash_function(k); \
		usize index = hash & (map->length - 1); \
		usize probe = 1; \
		\
		while (!compare_function(map->cells[index].key, invalid_key)) { \
			if (compare_function(map->cells[index].key, k)) { \
				map->cells[index].value = v; \
				return; \
			} \
			index = (index + probe) & (map->length - 1); \
			probe += 1; \
		} \
		\
		map->cells[index].key = k; \
		map->cells[index].value = v; \
		map->number_of_items += 1; \
		return; \
	} \
	\
	force_inline void map_type##_delete(map_type *map, key_type k) { \
		assert((map->length & (map->length-1)) == 0); \
		usize hash = (usize)hash_function(k); \
		usize index = hash & (map->length - 1); \
		usize probe = 1; \
		\
		while (!compare_function(map->cells[index].key, invalid_key)) { \
			if (compare_function(map->cells[index].key, k)) { \
				map->cells[index].key = invalid_key; \
				map->cells[index].value = nil_value; \
				return; \
			} \
			index = (index + probe) & (map->length - 1); \
			probe += 1; \
		} \
	}

#define MBMAP_H
#endif
