/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_MESSAGE_H_
#define ERLIX_MESSAGE_H_

#include "ruby.h"

#include "ei.h"
#include "erl_interface.h"

#include "erlix_term.h"
#include "erlix_node.h"


ErlMessage* new_erlix_message();

void free_erlix_message(void *msg);

VALUE erlix_message_alloc(VALUE klass);
VALUE erlix_message_init_copy(VALUE copy,VALUE orig);
VALUE erlix_message_mtype(VALUE self);
VALUE erlix_message_etype(VALUE self);
VALUE erlix_message_message(VALUE self);
VALUE erlix_message_from(VALUE self);
VALUE erlix_message_to(VALUE self);



void init_erlix_message();

#endif /* ERLIX_MESSAGE_H_ */
