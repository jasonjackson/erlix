/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_ATOM_H_
#define ERLIX_ATOM_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_atom_alloc(VALUE klass);
VALUE erlix_atom_init(VALUE self,VALUE string);
VALUE erlix_atom_to_str(VALUE self);
VALUE erlix_atom_size(VALUE self);
VALUE erlix_atom_etype(VALUE self);

void init_erlix_atom();

#endif /* ERLIX_ATOM_H_ */
