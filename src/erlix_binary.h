/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_BINARY_H_
#define ERLIX_BINARY_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_binary_alloc(VALUE klass);
VALUE erlix_binary_init(VALUE self,VALUE string);
VALUE erlix_binary_data(VALUE self);
VALUE erlix_binary_size(VALUE self);
VALUE erlix_binary_etype(VALUE self);

void init_erlix_binary();

#endif /* ERLIX_BINARY_H_ */
