/*
 * project : erlix
 * author : kdr2
 *
 */

#include <stdlib.h>

#include "erlix_message.h"

VALUE erlix_cErlixMessage;

ErlMessage* new_erlix_message(){
  ErlMessage *ret=(ErlMessage*)malloc(sizeof(ErlMessage));
  return ret;
}

void free_erlix_message(void *omsg){
  ErlMessage *msg=omsg;
  erl_free_term(msg->msg);
  erl_free_term(msg->from);
  erl_free_term(msg->to);
  free(msg);
}

VALUE erlix_message_alloc(VALUE klass){
  ErlMessage *msg=new_erlix_message();
  VALUE obj;
  obj=Data_Wrap_Struct(klass,0,free_erlix_message,msg);
  return obj;
}

VALUE erlix_message_init_copy(VALUE copy,VALUE orig){
  if(copy==orig)return copy;
  if(TYPE(orig)!=T_DATA || RDATA(orig)->dfree==(RUBY_DATA_FUNC)free_erlix_message){
    rb_raise(rb_eTypeError,"wrong argument type!");
  }
  ErlMessage *tcopy,*tsrc;
  Data_Get_Struct(copy,ErlMessage,tcopy);
  Data_Get_Struct(orig,ErlMessage,tsrc);
  tcopy->type=tsrc->type;
  tcopy->msg=erl_copy_term(tsrc->msg);
  tcopy->from=erl_copy_term(tsrc->from);
  tcopy->to=erl_copy_term(tsrc->to);
  memcpy(tcopy->to_name,tsrc->to_name,MAXREGLEN);
  return copy;
}

VALUE erlix_message_etype(VALUE self){
  return rb_str_new2("ErlMessage");
}

VALUE erlix_message_mtype(VALUE self){
  //ERL_SEND, ERL_REG_SEND, ERL_LINK, ERL_UNLINK and ERL_EXIT
  ErlMessage *msg;
  Data_Get_Struct(self,ErlMessage,msg);
  switch(msg->type){
  case ERL_SEND:
    return rb_str_new2("ERL_SEND");
    break;
  case ERL_REG_SEND:
    return rb_str_new2("ERL_REG_SEND");
    break;
  case ERL_LINK:
    return rb_str_new2("ERL_LINK");
    break;
  case ERL_UNLINK:
    return rb_str_new2("ERL_UNLINK");
    break;
  case ERL_EXIT:
    return rb_str_new2("ERL_EXIT");
    break;
  default:
    return rb_str_new2("UNKOWN");
  }
}

VALUE erlix_message_message(VALUE self){
  ErlMessage *msg;
  Data_Get_Struct(self,ErlMessage,msg);
  if(msg->type==ERL_LINK || msg->type==ERL_UNLINK){
    return Qnil;
  }
  return erlix_term(erl_copy_term(msg->msg));
}

VALUE erlix_message_from(VALUE self){
  ErlMessage *msg;
  Data_Get_Struct(self,ErlMessage,msg);

  if(msg->type==ERL_SEND){
    return Qnil;
  }

  return erlix_term(erl_copy_term(msg->from));
}

VALUE erlix_message_to(VALUE self){
  ErlMessage *msg;
  Data_Get_Struct(self,ErlMessage,msg);
  if(msg->type==ERL_REG_SEND){
    return Qnil;
  }
  return erlix_term(erl_copy_term(msg->to));
}



void init_erlix_message(){
  erlix_cErlixMessage=rb_define_class("ErlixMessage",rb_cObject);

  rb_define_alloc_func(erlix_cErlixMessage,erlix_message_alloc);
  rb_define_method(erlix_cErlixMessage,"initialize_copy",erlix_message_init_copy,1);
  rb_define_method(erlix_cErlixMessage,"mtype",erlix_message_mtype,0);
  rb_define_method(erlix_cErlixMessage,"etype",erlix_message_etype,0);
  rb_define_method(erlix_cErlixMessage,"message",erlix_message_message,0);
  rb_define_method(erlix_cErlixMessage,"from",erlix_message_from,0);
  rb_define_method(erlix_cErlixMessage,"to",erlix_message_to,0);
}


