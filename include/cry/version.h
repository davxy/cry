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

/**
 * @file    cry_version.h
 * @brief   Library version value.
 */

#ifndef _CRY_VERSION_H_
#define _CRY_VERSION_H_

/** Library version major number */
#define CRY_MAJOR   0
/** Library version minor number */
#define CRY_MINOR   0
/** Library version patch number */
#define CRY_PATCH   4
/** Library version number */
#define CRY_VERSION (((CRY_MAJOR << 8) | (CRY_MINOR << 8)) | CRY_PATCH)

/**
 * Compile-time library version.
 *
 * For consistency, should be equal to the CRY_VERSION value.
 *
 * @return	compile time library version.
 */
long cry_version(void);

#endif /* _CRY_VERSION_H_ */

