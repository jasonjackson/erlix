/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_NODE_H_
#define ERLIX_NODE_H_

#include "ruby.h"

#include "ei.h"
#include "erl_interface.h"

#include "erlix_term.h"

#define ERLIX_NODE_MOD "ErlixNode"

extern const char *erl_thisnodename(void);
extern short erl_thiscreation(void);

typedef struct _erlix_node{
  unsigned long status;
  int listen_fd;
}ErlixNode;


VALUE erlix_node_init(VALUE self,VALUE sname,VALUE cookie);
VALUE erlix_node_inited(VALUE self);
VALUE erlix_node_name(VALUE self);
VALUE erlix_node_creation(VALUE self);

VALUE erlix_node_publish(VALUE self);

VALUE erlix_node_accept(VALUE self);

void init_erlix_node();

#endif /* ERLIX_CONNECTION_H_ */
