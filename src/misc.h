/*
 * Copyright (c) 2015 Red Hat, Inc.
 * Author: Nathaniel McCallum <npmccallum@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "asn1.h"
#include <stdbool.h>

/* Callback signature for petera_getopt(). Return false on an error. */
typedef bool (*petera_parse)(char c, const char *arg, void *misc);

/* Validates the certificate chain using the specified trust anchors.
 * If anchors is NULL or empty, the default system trust store is used. */
bool
petera_validate(const STACK_OF(X509) *anchors, STACK_OF(X509) *chain);

/* Loads all certificates from the file and adds them to certs. */
bool
petera_load(FILE *fp, STACK_OF(X509) *certs);

/* Sends a request to the server and receives the response. */
PETERA_MSG *
petera_request(const STACK_OF(X509) *anchors, const ASN1_UTF8STRING *target,
               const PETERA_MSG *req);

/* Callback function for petera_getopt() for parsing anchor arguments. */
bool
petera_anchors(char c, const char *arg, STACK_OF(X509) **misc);

/* Implement consistent getopt style parsing.
 *
 * This function works like getopt(), and even uses getopt() internally.
 * A few differences are worth noting.
 *
 * First, options specified in both opt and keep are processed. However,
 * the options listed in keep are moved to the end of the argv array after
 * optind.
 *
 * Second, option processing is done via callbacks. This is done similar to
 * printf. Each option specified (in either opt or keep) requires two
 * variable arguments. The first is a pointer to a callback function with the
 * signature of petera_parse. The second is a misc value to pass to the
 * callback. If the first variable argument for an option is NULL, no
 * callback will be called. In this case, if the option specified is provided
 * the function will return false;
 */
bool
petera_getopt(int argc, char **argv, const char *opt, const char *keep, ...);