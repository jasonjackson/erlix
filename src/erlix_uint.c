/*
 * project : erlix
 * author : kdr2
 *
 */

#include <stdio.h>
#include <string.h>

#include "erlix_uint.h"

VALUE erlix_cErlixUInt;

VALUE erlix_uint_alloc(VALUE klass){
  ErlixTerm* euint;
  VALUE obj;
  euint=new_erlix_term();
  euint->type=TYPE_UINT;
  obj=Data_Wrap_Struct(klass,0,free_erlix_term,euint);
  return obj;
}

VALUE erlix_uint_init(VALUE self,VALUE fix){
  ErlixTerm* euint;
  unsigned int i=FIX2UINT(fix);
  Data_Get_Struct(self,ErlixTerm,euint);
  euint->term=erl_mk_uint(i);
  return self;
}


VALUE erlix_uint_to_fix(VALUE self){
  ErlixTerm *euint;
  Data_Get_Struct(self,ErlixTerm,euint);
  unsigned int l=ERL_INT_VALUE(euint->term);
  return INT2FIX(l);
}

VALUE erlix_uint_inspect(VALUE self){
  VALUE ret=rb_str_new2("#<ErlixUInt:");
  ID concat=rb_intern("concat");
  rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
  rb_funcall(ret,concat,1,rb_str_new2(">"));
  return ret;
}

VALUE erlix_uint_etype(VALUE self){
  return rb_str_new2("unsigned int");
}

void init_erlix_uint(){
  erlix_cErlixUInt=rb_define_class("ErlixUInt",rb_cObject);

  rb_define_alloc_func(erlix_cErlixUInt,erlix_uint_alloc);
  rb_define_method(erlix_cErlixUInt,"initialize",erlix_uint_init,1);
  rb_define_method(erlix_cErlixUInt,"to_i",erlix_uint_to_fix,0);
  rb_define_method(erlix_cErlixUInt,"inspect",erlix_uint_inspect,0);
  rb_define_method(erlix_cErlixUInt,"etype",erlix_uint_etype,0);

  rb_include_module(erlix_cErlixUInt,erlix_mErlixTerm);
}
