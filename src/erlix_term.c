/*
 * project : erlix
 * author : kdr2
 *
 */

#include <stdlib.h>

#include "erlix_term.h"
#include "erlix_pid.h"
#include "erlix_atom.h"
#include "erlix_list.h"
#include "erlix_tuple.h"

VALUE erlix_mErlixTerm;

ErlixTerm* new_erlix_term(){
  ErlixTerm* ret=(ErlixTerm*)malloc(sizeof(ErlixTerm));
  return ret;
}

void free_erlix_term(void* ptr){
  ErlixTerm* eterm=ptr;
  //TODO
  //flags
  //erl_free_term(eterm->term);
  erl_free_compound(eterm->term);
}

//initialize_copy
VALUE erlix_term_init_copy(VALUE copy,VALUE orig){
  if(copy==orig)return copy;
  if(!IS_ETERM(orig)){
    rb_raise(rb_eTypeError,"wrong argument type!");
  }
  ErlixTerm *tcopy,*tsrc;
  Data_Get_Struct(copy,ErlixTerm,tcopy);
  Data_Get_Struct(orig,ErlixTerm,tsrc);
  tcopy->flags=tsrc->flags;
  tcopy->type=tsrc->type;
  tcopy->term=erl_copy_term(tsrc->term);

  return copy;
}

//obj.puts
VALUE erlix_term_puts(VALUE self){
  ErlixTerm *eterm;
  Data_Get_Struct(self,ErlixTerm,eterm);
  erl_print_term(stdout,eterm->term);
  printf("\n");
  return Qnil;
}

VALUE erlix_term_eql(VALUE left,VALUE right){
  ErlixTerm *el,*er;
  Data_Get_Struct(left,ErlixTerm,el);
  Data_Get_Struct(right,ErlixTerm,er);
  if(erl_match(el->term, er->term)){
    return Qtrue;
  }
  return Qfalse;
}

VALUE erlix_term_match(VALUE self,VALUE string){
  VALUE ret;
  VALUE pattern=StringValue(string);
  ErlixTerm *et;
  Data_Get_Struct(self,ErlixTerm,et);
  ETERM *epattern = erl_format(RSTRING(pattern)->ptr);
  if(erl_match(epattern,et->term)){
    ret=Qtrue;
  }else{
    ret=Qfalse;
  }
  erl_free_term(epattern);
  return ret;
}

VALUE erlix_term_mget(VALUE self,VALUE string,VALUE e){
  VALUE ret;
  VALUE pattern=StringValue(string);
  VALUE t=StringValue(e);
  ErlixTerm *et;
  Data_Get_Struct(self,ErlixTerm,et);
  ETERM *epattern = erl_format(RSTRING(pattern)->ptr);
  if(erl_match(epattern,et->term)){
    ETERM *ep = erl_var_content(epattern,RSTRING(e)->ptr);
    if(ep==NULL){
      ret=Qnil;
    }else{
      ret=erlix_term(ep);
    }
  }else{
    ret=Qnil;
  }
  erl_free_term(epattern);
  return ret;;
}


unsigned long erlix_term_type(ETERM *t){
  if(ERL_IS_INTEGER(t))return TYPE_INT;
  if(ERL_IS_UNSIGNED_INTEGER(t))return TYPE_UINT;
  if(ERL_IS_FLOAT(t))return TYPE_FLOAT;
  if(ERL_IS_ATOM(t))return TYPE_ATOM;
  if(ERL_IS_PID(t))return TYPE_PID;
  if(ERL_IS_PORT(t))return TYPE_PORT;
  if(ERL_IS_REF(t))return TYPE_REF;
  if(ERL_IS_TUPLE(t))return TYPE_TUPLE;
  if(ERL_IS_BINARY(t))return TYPE_BINARY;
  if(ERL_IS_LIST(t))return TYPE_LIST;
  if(ERL_IS_EMPTY_LIST(t))return TYPE_EMPTY_LIST;
  if(ERL_IS_CONS(t))return TYPE_CONS;
  return 0;
}

VALUE erlix_term(ETERM *term){
  unsigned long type=erlix_term_type(term);
  VALUE ret;
  if(type==TYPE_INT){
    ret=erlix_pid_alloc(erlix_cErlixInt);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_UINT){
    ret=erlix_pid_alloc(erlix_cErlixUInt);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_FLOAT){
    ret=erlix_pid_alloc(erlix_cErlixFloat);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_PID){
    ret=erlix_pid_alloc(erlix_cErlixPid);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_REF){
    ret=erlix_pid_alloc(erlix_cErlixRef);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_ATOM){
    ret=erlix_atom_alloc(erlix_cErlixAtom);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_LIST){
    ret=erlix_list_alloc(erlix_cErlixList);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_TUPLE){
    ret=erlix_tuple_alloc(erlix_cErlixTuple);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_BINARY){
    ret=erlix_tuple_alloc(erlix_cErlixBinary);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else{
    //TODO OTHER TYPE!!!
    return Qnil;
  }
}


void init_erlix_term(){
  erlix_mErlixTerm=rb_define_module("ErlixTerm");
  rb_define_method(erlix_mErlixTerm,"initialize_copy",erlix_term_init_copy,1);
  rb_define_method(erlix_mErlixTerm,"eql?",erlix_term_eql,1);
  rb_define_method(erlix_mErlixTerm,"match",erlix_term_match,1);
  rb_define_method(erlix_mErlixTerm,"mget",erlix_term_mget,2);
  rb_define_method(erlix_mErlixTerm,"puts",erlix_term_puts,0);
  rb_define_method(erlix_mErlixTerm,"==",erlix_term_eql,1);
}
