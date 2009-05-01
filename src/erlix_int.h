/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_INT_H_
#define ERLIX_INT_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_int_alloc(VALUE klass);
VALUE erlix_int_init(VALUE self,VALUE fixnum);
VALUE erlix_int_to_fix(VALUE self);
VALUE erlix_int_size(VALUE self);
VALUE erlix_int_etype(VALUE self);

void init_erlix_int();

#endif /* ERLIX_INT_H_ */
