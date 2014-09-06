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
 * @file    cry_base64.h
 * @brief   Base-64 encoder/decoder.
 */

#ifndef _CRY_BASE64_H_
#define _CRY_BASE64_H_

#ifdef __cplusplus
extern "C"{
#endif

int cry_base64_encode(const char *in, int len, char *out);

int cry_base64_decode(const char *in, int len, char *out);

#ifdef __cplusplus
}
#endif

#endif /* _CRY_BASE64_H_ */

