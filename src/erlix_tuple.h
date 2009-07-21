/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_TUPLE_H_
#define ERLIX_TUPLE_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_tuple_alloc(VALUE klass);
VALUE erlix_tuple_init(VALUE self,VALUE ary);
VALUE erlix_tuple_create(int argc,VALUE *argv,VALUE klass);
VALUE erlix_tuple_nth(VALUE self,VALUE index);
VALUE erlix_tuple_to_ary(VALUE self);
VALUE erlix_tuple_size(VALUE self);
VALUE erlix_tuple_inspect(VALUE self);
VALUE erlix_tuple_etype(VALUE self);


void init_erlix_tuple();

#endif /* ERLIX_TUPLE_H_ */
