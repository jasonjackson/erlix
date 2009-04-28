/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_CONNECTION_H_
#define ERLIX_CONNECTION_H_

#include "ruby.h"

#include "ei.h"
#include "erl_interface.h"

#include "erlix_term.h"
#include "erlix_node.h"

#define ERLIX_CONNECTION_ALIVE(cp) ((cp->status&0x01)==0x01)
#define ERLIX_CONNECTION_DEAD(cp)  (cp->status&=0xfe)
#define ERLIX_CONNECTION_BORN(cp)  (cp->status|=0x01)

typedef struct _erlix_connection{
  unsigned char status;
  VALUE peer_node;
  int sock_fd;
}ErlixConnection;


ErlixConnection* new_erlix_connection();
void free_erlix_connection(void *econ);

static VALUE erlix_connection_alloc(VALUE klass);
static VALUE erlix_connection_init(VALUE self,VALUE pnode);
static VALUE erlix_connection_init_copy(VALUE copy,VALUE orig);
static VALUE erlix_connection_to_str(VALUE self);
static VALUE erlix_connection_peer(VALUE self);
static VALUE erlix_connection_etype(VALUE self);
static VALUE erlix_connection_mkpid(VALUE self);
static VALUE erlix_connection_send(VALUE self,VALUE pid,VALUE term);
static VALUE erlix_connection_recv(VALUE self);



void init_erlix_connection();

#endif /* ERLIX_CONNECTION_H_ */
