/*
 * project : erlix
 * author : kdr2
 *
 */

#include <stdio.h>
#include <string.h>

#include "erlix_int.h"

VALUE erlix_cErlixInt;

VALUE erlix_int_alloc(VALUE klass){
  ErlixTerm* eint;
  VALUE obj;
  eint=new_erlix_term();
  eint->type=TYPE_INT;
  obj=Data_Wrap_Struct(klass,0,free_erlix_term,eint);
  return obj;
}

VALUE erlix_int_init(VALUE self,VALUE fix){
  ErlixTerm* eint;
  int i=FIX2INT(fix);
  Data_Get_Struct(self,ErlixTerm,eint);
  eint->term=erl_mk_int(i);
  return self;
}


VALUE erlix_int_to_str(VALUE self){
  ErlixTerm* eint;
  Data_Get_Struct(self,ErlixTerm,eint);
  char buf[12];
  memset(buf,0,12);
  sprintf(buf,"%d",ERL_INT_VALUE(eint->term));
  return rb_str_new2(buf);
}

VALUE erlix_int_to_fix(VALUE self){
  ErlixTerm *eint;
  Data_Get_Struct(self,ErlixTerm,eint);
  int l=ERL_INT_VALUE(eint->term);
  return INT2FIX(l);
}

VALUE erlix_int_etype(VALUE self){
  return rb_str_new2("int");
}

void init_erlix_int(){
  erlix_cErlixInt=rb_define_class("ErlixInt",rb_cObject);

  rb_define_alloc_func(erlix_cErlixInt,erlix_int_alloc);
  rb_define_method(erlix_cErlixInt,"initialize",erlix_int_init,1);
  rb_define_method(erlix_cErlixInt,"to_s",erlix_int_to_str,0);
  rb_define_method(erlix_cErlixInt,"to_i",erlix_int_to_fix,0);
  rb_define_method(erlix_cErlixInt,"etype",erlix_int_etype,0);

  rb_include_module(erlix_cErlixInt,erlix_mErlixTerm);
}
