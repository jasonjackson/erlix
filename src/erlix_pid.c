/*
 * project : erlix
 * author : kdr2
 *
 */
#include <stdlib.h>
#include <string.h>

#include "erlix_pid.h"
#include "erlix_node.h"
#include "erlix_connection.h"

VALUE erlix_cErlixPid;

extern const char *erl_thisnodename(void);
extern short erl_thiscreation(void);
extern ErlixNode *erlix_node;

VALUE erlix_pid_alloc(VALUE klass){
  ErlixTerm* pid;
  VALUE obj;
  pid=new_erlix_term();
  pid->type=TYPE_PID;
  obj=Data_Wrap_Struct(klass,0,free_erlix_term,pid);
  return obj;
}

VALUE erlix_pid_init(VALUE self,VALUE econn){
  if(erlix_node==NULL){
    rb_raise(rb_eException,"call ErlixNode.init(name,cookie) first please!");
    return Qnil;
  }
  ErlixTerm* pid;
  ErlixConnection* conn;
  if((TYPE(econn)!=T_DATA || RDATA(econn)->dfree!=(RUBY_DATA_FUNC)free_erlix_connection)){
    rb_raise(rb_eTypeError,"the argument is not an ErlixConnection");
  }
  Data_Get_Struct(self,ErlixTerm,pid);
  Data_Get_Struct(econn,ErlixConnection,conn);
  pid->term=erl_mk_pid(erl_thisnodename(),conn->sock_fd,rand()%16,erl_thiscreation());
  return self;
}


VALUE erlix_pid_to_str(VALUE self){
  ErlixTerm* pid;
  Data_Get_Struct(self,ErlixTerm,pid);
  char name[10];
  ETERM *p=pid->term;
  memset(name,0,10);
  sprintf(name,"<%d.%d.%d>",ERL_PID_NUMBER(p),ERL_PID_SERIAL(p),ERL_PID_CREATION(p));
  return rb_str_new2(name);
}


VALUE erlix_pid_etype(VALUE self){
  return rb_str_new2("pid");
}

void init_erlix_pid(){
  erlix_cErlixPid=rb_define_class("ErlixPid",rb_cObject);

  rb_define_alloc_func(erlix_cErlixPid,erlix_pid_alloc);
  rb_define_method(erlix_cErlixPid,"initialize",erlix_pid_init,1);
  rb_define_method(erlix_cErlixPid,"to_s",erlix_pid_to_str,0);
  rb_define_method(erlix_cErlixPid,"etype",erlix_pid_etype,0);

  rb_include_module(erlix_cErlixPid,erlix_mErlixTerm);
}
