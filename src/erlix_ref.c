/*
 * project : erlix
 * author : kdr2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "erlix_ref.h"

VALUE erlix_cErlixRef;
extern const char *erl_thisnodename(void);
extern short erl_thiscreation(void);

VALUE erlix_ref_alloc(VALUE klass){
  ErlixTerm* eref;
  VALUE obj;
  eref=new_erlix_term();
  eref->type=TYPE_REF;
  obj=Data_Wrap_Struct(klass,0,free_erlix_term,eref);
  return obj;
}

VALUE erlix_ref_init(VALUE self){
  ErlixTerm* eref;
  Data_Get_Struct(self,ErlixTerm,eref);
  eref->term=erl_mk_long_ref(erl_thisnodename(),rand(),rand(),time(NULL),erl_thiscreation());
  return self;
}


VALUE erlix_ref_to_str(VALUE self){
  ErlixTerm* eref;
  Data_Get_Struct(self,ErlixTerm,eref);
  //char buf[64];
  //memset(buf,0,12);
  //sprintf(buf,"#Ref<%d.%d.%d.%d>",ERL_REF_VALUE(eref->term));
  return rb_str_new2("#Ref<...>");
}


VALUE erlix_ref_etype(VALUE self){
  return rb_str_new2("ref");
}

void init_erlix_ref(){
  erlix_cErlixRef=rb_define_class("ErlixRef",rb_cObject);

  rb_define_alloc_func(erlix_cErlixRef,erlix_ref_alloc);
  rb_define_method(erlix_cErlixRef,"initialize",erlix_ref_init,0);
  rb_define_method(erlix_cErlixRef,"to_s",erlix_ref_to_str,0);
  rb_define_method(erlix_cErlixRef,"etype",erlix_ref_etype,0);

  rb_include_module(erlix_cErlixRef,erlix_mErlixTerm);
}
