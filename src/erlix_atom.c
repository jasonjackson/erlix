/*
 * project : erlix
 * author : kdr2
 *
 */

#include "erlix_atom.h"

VALUE erlix_cErlixAtom;

VALUE erlix_atom_alloc(VALUE klass){
  ErlixTerm* atom;
  VALUE obj;
  atom=new_erlix_term();
  atom->type=TYPE_ATOM;
  obj=Data_Wrap_Struct(klass,0,free_erlix_term,atom);
  return obj;
}

VALUE erlix_atom_init(VALUE self,VALUE string){
  ErlixTerm* atom;
  VALUE str=StringValue(string);
  Data_Get_Struct(self,ErlixTerm,atom);
  atom->term=erl_mk_atom(RSTRING(str)->ptr);
  return self;
}


VALUE erlix_atom_size(VALUE self){
  ErlixTerm *atom;
  Data_Get_Struct(self,ErlixTerm,atom);
  int l=ERL_ATOM_SIZE(atom->term);
  return INT2FIX(l);
}

VALUE erlix_atom_inspect(VALUE self){
  VALUE ret=rb_str_new2("#<ErlixAtom:");
  ID concat=rb_intern("concat");
  rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
  rb_funcall(ret,concat,1,rb_str_new2(">"));
  return ret;
}

VALUE erlix_atom_etype(VALUE self){
  return rb_str_new2("atom");
}

void init_erlix_atom(){
  erlix_cErlixAtom=rb_define_class("ErlixAtom",rb_cObject);

  rb_define_alloc_func(erlix_cErlixAtom,erlix_atom_alloc);
  rb_define_method(erlix_cErlixAtom,"initialize",erlix_atom_init,1);
  rb_define_method(erlix_cErlixAtom,"size",erlix_atom_size,0);
  rb_define_method(erlix_cErlixAtom,"etype",erlix_atom_etype,0);
  rb_define_method(erlix_cErlixAtom,"inspect",erlix_atom_inspect,0);

  rb_include_module(erlix_cErlixAtom,erlix_mErlixTerm);
}
