/*
 * project : erlix
 * author : kdr2
 *
 */

#include "erlix_tuple.h"

VALUE erlix_cErlixTuple;

VALUE erlix_tuple_alloc(VALUE klass){
  ErlixTerm *tuple=new_erlix_term();
  tuple->type=TYPE_TUPLE;
  VALUE obj;
  obj=Data_Wrap_Struct(klass,0,free_erlix_term,tuple);
  return obj;
}

VALUE erlix_tuple_init(VALUE self,VALUE ary){
  ErlixTerm *tuple;
  Data_Get_Struct(self,ErlixTerm,tuple);

  VALUE array=rb_check_array_type(ary);
  if(NIL_P(array)||RARRAY(array)->len==0){
    //empty tuple
    tuple->term=erl_format("{}");
    return self;
  }
  int i;
    //check: all elements' must be ErlixTerm or auto-convertable Type
  for(i=0;i<RARRAY(array)->len;i++){
    VALUE e=RARRAY(array)->ptr[i];
    if(!IS_ETERM(e) && !CAN_AUTO_CONV(e)){
      rb_raise(rb_eTypeError,"all tuple's elements must be ErlixTerm or Auto-Convertable-Type!");
    }
  }
  ETERM **tes=(ETERM**)malloc(sizeof(ETERM*)*(RARRAY(array)->len));
  for(i=0;i<RARRAY(array)->len;i++){
    VALUE e=RARRAY(array)->ptr[i];
    if(IS_ETERM(e)){
      ErlixTerm *ep;
      Data_Get_Struct(e,ErlixTerm,ep);
      *(tes+i)=erl_copy_term(ep->term);
    }else{
      *(tes+i)=erlix_auto_conv(e);
    }
  }
  tuple->term=erl_mk_tuple(tes,RARRAY(array)->len);
  //for(i=0;i<RARRAY(array)->len;i++){
  //  erl_free_term(*(tes+i));
  //}
  free(tes);
  return self;
}

VALUE erlix_tuple_create(int argc,VALUE *argv,VALUE klass){
  ETERM *rterm=NULL;
  if(argc==0){
    //empty tuple
    rterm=erl_format("{}");
  }else if(argc>0){
    int i;
    //check: all elements' must be ErlixTerm
    //or automatic conversions
    for(i=0;i<argc;i++){
      if(!IS_ETERM(argv[i]) && !CAN_AUTO_CONV(argv[i])){
        rb_raise(rb_eTypeError,"all tuple's elements must be ErlixTerm!");
      }
    }
    ETERM **tes=(ETERM**)malloc(sizeof(ETERM*)*argc);
    for(i=0;i<argc;i++){
      if(IS_ETERM(argv[i])){
        ErlixTerm *ep;
        Data_Get_Struct(argv[i],ErlixTerm,ep);
        *(tes+i)=erl_copy_term(ep->term);
      }else{
        *(tes+i)=erlix_auto_conv(argv[i]);
      }
    }
    rterm=erl_mk_tuple(tes,argc);
    //for(i=0;i<argc;i++){
    //  erl_free_term(*(tes+i));
    //}
    free(tes);
  }
  return erlix_term(rterm);
}

VALUE erlix_tuple_nth(VALUE self,VALUE index){
  if(TYPE(index)!=T_FIXNUM){
    rb_raise(rb_eTypeError,"index must be a fixnum(1..len)!");
    return Qnil;
  }
  ErlixTerm *tuple;
  Data_Get_Struct(self,ErlixTerm,tuple);
  if(ERL_TUPLE_SIZE(tuple->term)<FIX2INT(index)||FIX2INT(index)<=0){
    return Qnil;
  }
  ETERM *e=erl_element(FIX2INT(index),tuple->term);
  return erlix_term(erl_copy_term(e));
}

VALUE erlix_tuple_to_ary(VALUE self){
  ErlixTerm *tuple;
  Data_Get_Struct(self,ErlixTerm,tuple);
  int len=ERL_TUPLE_SIZE(tuple->term);
  VALUE ret=rb_ary_new2(len);
  int i=0;
  for(;i<len;i++){
    ETERM *e=erl_element(i+1,tuple->term);
    rb_ary_store(ret,i,erlix_term(erl_copy_term(e)));
  }
  return ret;
}


VALUE erlix_tuple_size(VALUE self){
  ErlixTerm *tuple;
  Data_Get_Struct(self,ErlixTerm,tuple);
  int len=ERL_TUPLE_SIZE(tuple->term);
  return INT2FIX(len);
}

VALUE erlix_tuple_inspect(VALUE self){
  VALUE ret=rb_str_new2("#<ErlixTuple:");
  ID concat=rb_intern("concat");
  rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
  rb_funcall(ret,concat,1,rb_str_new2(">"));
  return ret;
}

VALUE erlix_tuple_etype(VALUE self){
  return rb_str_new2("tuple");
}

void init_erlix_tuple(){
  erlix_cErlixTuple=rb_define_class("ErlixTuple",rb_cObject);

  rb_define_alloc_func(erlix_cErlixTuple,erlix_tuple_alloc);
  rb_define_method(erlix_cErlixTuple,"initialize",erlix_tuple_init,1);
  rb_define_method(erlix_cErlixTuple,"nth",erlix_tuple_nth,1);
  rb_define_method(erlix_cErlixTuple,"[]",erlix_tuple_nth,1);
  rb_define_method(erlix_cErlixTuple,"to_a",erlix_tuple_to_ary,0);
  rb_define_method(erlix_cErlixTuple,"size",erlix_tuple_size,0);
  rb_define_method(erlix_cErlixTuple,"inspect",erlix_tuple_inspect,0);
  rb_define_method(erlix_cErlixTuple,"etype",erlix_tuple_etype,0);

  rb_define_singleton_method(erlix_cErlixTuple, "[]", erlix_tuple_create, -1);

  rb_include_module(erlix_cErlixTuple,erlix_mErlixTerm);
}
