/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_REF_H_
#define ERLIX_REF_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_REF_alloc(VALUE klass);
VALUE erlix_REF_init(VALUE self);
VALUE erlix_ref_inspect(VALUE self);
VALUE erlix_REF_etype(VALUE self);

void init_erlix_REF();

#endif /* ERLIX_REF_H_ */
