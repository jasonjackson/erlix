/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_PID_H_
#define ERLIX_PID_H_

#include "ruby.h"

#include "erlix_term.h"


VALUE erlix_pid_alloc(VALUE klass);
VALUE erlix_pid_init(VALUE self,VALUE econn);
VALUE erlix_pid_to_str(VALUE self);
VALUE erlix_pid_size(VALUE self);
VALUE erlix_pid_etype(VALUE self);

void init_erlix_pid();

#endif /* ERLIX_PID_H_ */
