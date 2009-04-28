/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_TERM_H_
#define ERLIX_TERM_H_

#include "ruby.h"

#include "erl_interface.h"
#include "ei.h"

/*********** erlix_term  ***********/

#define TYPE_ATOM        (1<<0)  //DONE
#define TYPE_INT         (1<<1)  //DONE bug?
#define TYPE_UINT        (1<<2)  //DONE bug?
#define TYPE_FLOAT       (1<<3)  //DONE
#define TYPE_PID         (1<<4)  //DONE
#define TYPE_PORT        (1<<5)  //TODO now?
#define TYPE_REF         (1<<6)  //DONE
#define TYPE_TUPLE       (1<<7)  //DONE
#define TYPE_BINARY      (1<<8)  //DONE
#define TYPE_LIST        (1<<9)  //DONE
#define TYPE_EMPTY_LIST  (1<<10) //NONE
#define TYPE_CONS        (1<<11) //NONE

typedef struct _erlix_term{
  unsigned long flags;
  unsigned long type;
  ETERM *term;
}ErlixTerm;


extern VALUE erlix_cErlixInt;
extern VALUE erlix_cErlixUInt;
extern VALUE erlix_cErlixFloat;
extern VALUE erlix_cErlixPid;
extern VALUE erlix_cErlixRef;
extern VALUE erlix_cErlixAtom;
extern VALUE erlix_cErlixList;
extern VALUE erlix_cErlixTuple;
extern VALUE erlix_cErlixBinary;
extern VALUE erlix_cErlixConnection;

extern VALUE erlix_mErlixTerm;
extern VALUE erlix_mErlixNode;

ErlixTerm* new_erlix_term();
void free_erlix_term(void* p);

#define IS_ETERM(v) (TYPE(v)==T_DATA && RDATA(v)->dfree==(RUBY_DATA_FUNC)free_erlix_term)

VALUE erlix_term(ETERM *term);
VALUE erlix_term_init_copy(VALUE copy,VALUE orig);
VALUE erlix_term_puts(VALUE self);
VALUE erlix_term_eql(VALUE left,VALUE right);
VALUE erlix_term_match(VALUE left,VALUE string);
VALUE erlix_term_mget(VALUE left,VALUE string,VALUE e);

unsigned long erlix_term_type(ETERM *term);

void init_erlix_term();

#endif /* ERLIX_TERM_H_ */
