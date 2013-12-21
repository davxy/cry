/*
 * Copyright (c) 2013, Davide Galassi. All rights reserved.
 *
 * This file is part of CRY software.
 *
 * CRY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CRY; if not, see <http://www.gnu/licenses/>.
 */

/*
 * A bounch of helper macros and functions meant for internal use.
 */

#ifndef _CRY_MISC_H_
#define _CRY_MISC_H_

/** Statically get array number of elements */
#define CRY_ARRAY_LEN(ar) (sizeof(ar)/sizeof((ar)[0]))

/** Macro used to compute the minimum of two integral values. */
#define CRY_MIN(a, b)     (((a) < (b)) ? (a) : (b))
/** Macro used to compute the maximum of two integral values. */
#define CRY_MAX(a, b)     (((a) > (b)) ? (a) : (b))

#endif /* _CRY_MISC_H_ */

