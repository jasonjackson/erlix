/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_LIST_H_
#define ERLIX_LIST_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_list_alloc(VALUE klass);
VALUE erlix_list_init(VALUE self,VALUE string);
VALUE erlix_list_head(VALUE self);
VALUE erlix_list_tail(VALUE self);
VALUE erlix_list_cons(VALUE self,VALUE head);
VALUE erlix_list_to_str(VALUE self);
VALUE erlix_list_size(VALUE self);
VALUE erlix_list_etype(VALUE self);

void init_erlix_list();

#endif /* ERLIX_LIST_H_ */
