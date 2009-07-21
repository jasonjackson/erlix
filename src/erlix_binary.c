/*
 * project : erlix
 * author : kdr2
 *
 */

#include "erlix_binary.h"

VALUE erlix_cErlixBinary;

VALUE erlix_binary_alloc(VALUE klass){
  ErlixTerm* binary;
  VALUE obj;
  binary=new_erlix_term();
  binary->type=TYPE_BINARY;
  obj=Data_Wrap_Struct(klass,0,free_erlix_term,binary);
  return obj;
}

VALUE erlix_binary_init(VALUE self,VALUE string){
  ErlixTerm* binary;
  VALUE str=StringValue(string);
  Data_Get_Struct(self,ErlixTerm,binary);
  binary->term=erl_mk_binary(RSTRING(str)->ptr,RSTRING(str)->len);
  return self;
}


VALUE erlix_binary_data(VALUE self){
  ErlixTerm* binary;
  Data_Get_Struct(self,ErlixTerm,binary);
  return rb_str_new(ERL_BIN_PTR(binary->term),ERL_BIN_SIZE(binary->term));
}

VALUE erlix_binary_size(VALUE self){
  ErlixTerm *binary;
  Data_Get_Struct(self,ErlixTerm,binary);
  int l=ERL_BIN_SIZE(binary->term);
  return INT2FIX(l);
}

VALUE erlix_binary_inspect(VALUE self){
  VALUE ret=rb_str_new2("#<ErlixBinary:");
  ID concat=rb_intern("concat");
  rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
  rb_funcall(ret,concat,1,rb_str_new2(">"));
  return ret;
}

VALUE erlix_binary_etype(VALUE self){
  return rb_str_new2("binary");
}

void init_erlix_binary(){
  erlix_cErlixBinary=rb_define_class("ErlixBinary",rb_cObject);

  rb_define_alloc_func(erlix_cErlixBinary,erlix_binary_alloc);
  rb_define_method(erlix_cErlixBinary,"initialize",erlix_binary_init,1);
  rb_define_method(erlix_cErlixBinary,"data",erlix_binary_data,0);
  rb_define_method(erlix_cErlixBinary,"size",erlix_binary_size,0);
  rb_define_method(erlix_cErlixBinary,"inspect",erlix_binary_inspect,0);
  rb_define_method(erlix_cErlixBinary,"etype",erlix_binary_etype,0);

  rb_include_module(erlix_cErlixBinary,erlix_mErlixTerm);
}
