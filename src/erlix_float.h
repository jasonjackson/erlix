/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_FLOAT_H_
#define ERLIX_FLOAT_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_float_alloc(VALUE klass);
VALUE erlix_float_init(VALUE self,VALUE fixnum);
VALUE erlix_float_to_fix(VALUE self);
VALUE erlix_float_size(VALUE self);
VALUE erlix_float_etype(VALUE self);

void init_erlix_float();

#endif /* ERLIX_FLOAT_H_ */
