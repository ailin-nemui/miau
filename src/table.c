/*
 * -------------------------------------------------------
 * Copyright (C) 2002-2003 Tommi Saviranta <tsaviran@cs.helsinki.fi>
 *	(C) 1998-2002 Sebastian Kienzl <zap@riot.org>
 * -------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "table.h"

/* #define DEBUG */


#ifdef _NEED_TABLE


void **add_item(
		void	**data,
		int	elementsize,
		int	*entries,
		int	*indx
	       )
{
	int	i;
	int	ind = -1;

	for (i = 0; i < *entries; i++) {
		if (! data[i]) {
			ind = i; /* xfree pointer found */
#ifdef DEBUG
			printf("using empty pointer at index %d.\n", ind);
#endif
		}
	}
	if (ind < 0) {
		/* Allocate new pointer. */
		++(*entries);
		data = (void **) xrealloc(data, *entries * sizeof(void *));
		ind = *entries - 1;
#ifdef DEBUG
		printf("allocating new pointer. %d entries, index %d\n",
				*entries, ind);
#endif
	}

	data[ind] = (void *) xmalloc(elementsize);

	*indx = ind;

	return data;
}



void **compact_table(
		void	**data,
		int	*entries
		)
{
#ifdef DEBUG
	int	x = 0;
#endif
	int	i = *entries - 1;

	while (i >= 0 && data[i] == NULL) { i--; }
	i++;
	if (*entries != 0 && i == 0) {
		xfree(data);
		data = NULL;
	} else {
		data = (void **) xrealloc(data, i * sizeof(void *));
	}
	*entries = i;

#ifdef DEBUG
	printf("reduced %d (size %d)\n", x, *entries);
	if (*entries == 0) {
		printf("ignore: pointer-array is now empty\n");
	}
#endif
	return data;
}



void **rem_item(
		void	**data,
		int	number,
		int	*entries
	       )
{
	if (number >= 0 && number < *entries) {
#ifdef DEBUG
		printf("deleting entry %d, compacting table: ", number);
#endif
		xfree(data[number]);
		data[number] = NULL;
	}
	data = compact_table(data, entries);
	return data;
}



void **free_table(
		void	**data,
		int	*entries,
		int	clear
		)
{
	int	i;

	for (i = 0; i < *entries; i++) {
		if (data[i]) {
			xfree(data[i]);
		}
	}

	xfree(data);

	if (clear) {
		*entries = 0;
	}

	return 0;
}

#endif /* _NEED_TABLE */
