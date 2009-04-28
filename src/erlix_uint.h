/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_UINT_H_
#define ERLIX_UINT_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_uint_alloc(VALUE klass);
VALUE erlix_uint_init(VALUE self,VALUE fixnum);
VALUE erlix_uint_to_str(VALUE self);
VALUE erlix_uint_to_fix(VALUE self);
VALUE erlix_uint_size(VALUE self);
VALUE erlix_uint_etype(VALUE self);

void init_erlix_uint();

#endif /* ERLIX_UINT_H_ */
