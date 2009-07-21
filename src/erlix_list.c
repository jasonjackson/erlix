/*
 * project : erlix
 * author : kdr2
 *
 */

#include "erlix_list.h"

VALUE erlix_cErlixList;

VALUE erlix_list_alloc(VALUE klass){
  ErlixTerm *list=new_erlix_term();
  list->type=TYPE_LIST;
  VALUE obj;
  obj=Data_Wrap_Struct(klass,0,free_erlix_term,list);
  return obj;
}

VALUE erlix_list_init(VALUE self,VALUE ary){
  ErlixTerm *list;
  Data_Get_Struct(self,ErlixTerm,list);

  if(NIL_P(ary)){
    //empty list
    list->term=erl_format("[]");
  }else if(TYPE(ary)==T_ARRAY){
    if(RARRAY(ary)->len==0){
      //empty list
      list->term=erl_format("[]");
    }else{
      int i;
      //check: all elements' must be ErlixTerm or auto-convertable Type
      for(i=0;i<RARRAY(ary)->len;i++){
        VALUE e=RARRAY(ary)->ptr[i];
        if(!IS_ETERM(e) && !CAN_AUTO_CONV(e)){
          rb_raise(rb_eTypeError,"all list's elements must be ErlixTerm or Auto-Convertable-Type!");
        }
      }
      ETERM **les=(ETERM**)malloc(sizeof(ETERM*)*(RARRAY(ary)->len));
      for(i=0;i<RARRAY(ary)->len;i++){
        VALUE e=RARRAY(ary)->ptr[i];
        if(IS_ETERM(e)){
          ErlixTerm *ep;
          Data_Get_Struct(e,ErlixTerm,ep);
          *(les+i)=erl_copy_term(ep->term);
        }else{
          *(les+i)=erlix_auto_conv(e);
        }
      }
      list->term=erl_mk_list(les,RARRAY(ary)->len);
      //for(i=0;i<RARRAY(ary)->len;i++){
      //  erl_free_term(*(les+i));
      //}
      free(les);
    }
  }else if(TYPE(ary)==T_STRING){
    list->term=erl_mk_estring(RSTRING(ary)->ptr, RSTRING(ary)->len);
  }
  return self;
}

VALUE erlix_list_create(int argc,VALUE *argv,VALUE klass){
  ETERM *rterm;
  if(argc==0){
    //empty list
    rterm=erl_format("[]");
  }else{
    int i;
    //check: all elements' must be ErlixTerm or auto-convertable Type
    for(i=0;i<argc;i++){
      VALUE e=argv[i];
      if(!IS_ETERM(e) && !CAN_AUTO_CONV(argv[i])){
        rb_raise(rb_eTypeError,"all list's elements must be ErlixTerm or Auto-Convertable-Type!");
      }
    }
    ETERM **les=(ETERM**)malloc(sizeof(ETERM*)*argc);
    for(i=0;i<argc;i++){
      VALUE e=argv[i];
      if(IS_ETERM(e)){
        ErlixTerm *ep;
        Data_Get_Struct(e,ErlixTerm,ep);
        *(les+i)=erl_copy_term(ep->term);
      }else{
        *(les+i)=erlix_auto_conv(e);
      }
    }
    rterm=erl_mk_list(les,argc);
    //for(i=0;i<argc;i++){
    //  erl_free_term(*(les+i));
    //}
    free(les);
  }
  return erlix_term(rterm);
}

VALUE erlix_list_to_ary(VALUE self){
  ErlixTerm *list;
  Data_Get_Struct(self,ErlixTerm,list);
  int len=erl_length(list->term);
  VALUE ret=rb_ary_new2(len);
  ETERM *hd,*tmp,*ep=list->term;
  int i=0;
  while(!ERL_IS_EMPTY_LIST(ep)){
    hd=erl_hd(ep);
    rb_ary_store(ret,i,erlix_term(erl_copy_term(hd)));
    tmp=erl_tl(ep);
    //if(ep!=list->term)erl_free_term(ep);
    ep=tmp;
    //if(ERL_IS_EMPTY_LIST(ep)){
    //  erl_free_term(ep);
    //}
    ++i;
  }
  return ret;
}

VALUE erlix_list_head(VALUE self){
  ErlixTerm *list;
  Data_Get_Struct(self,ErlixTerm,list);
  if(ERL_IS_EMPTY_LIST(list->term)){
    return Qnil;
  }
  ETERM *ep=erl_hd(list->term);
  return erlix_term(erl_copy_term(ep));
}

VALUE erlix_list_tail(VALUE self){
  ErlixTerm *list;
  Data_Get_Struct(self,ErlixTerm,list);
  if(ERL_IS_EMPTY_LIST(list->term)){
    return Qnil;
  }
  ETERM *ep=erl_tl(list->term);
  return erlix_term(erl_copy_term(ep));
}

VALUE erlix_list_cons(VALUE self,VALUE head){
  if(!IS_ETERM(head) && !CAN_AUTO_CONV(head)){
    rb_raise(rb_eTypeError,"the head must be ErlixTerm or Auto-Convertable-Type!");
    return Qnil;
  }
  ErlixTerm *list;
  ETERM *ep;
  Data_Get_Struct(self,ErlixTerm,list);
  if(IS_ETERM(head)){
    ErlixTerm *hd;
    Data_Get_Struct(head,ErlixTerm,hd);
    ep=erl_cons(hd->term,list->term);
  }else{
    ep=erl_cons(erlix_auto_conv(head),list->term);
  }
  VALUE ret= erlix_term(erl_copy_term(ep));
  erl_free_term(ep);
  return ret;
}

VALUE erlix_list_size(VALUE self){
  ErlixTerm *list;
  Data_Get_Struct(self,ErlixTerm,list);
  int len=erl_length(list->term);
  return INT2FIX(len);
}

VALUE erlix_list_inspect(VALUE self){
  VALUE ret=rb_str_new2("#<ErlixList:");
  ID concat=rb_intern("concat");
  rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
  rb_funcall(ret,concat,1,rb_str_new2(">"));
  return ret;
}

VALUE erlix_list_etype(VALUE self){
  return rb_str_new2("list");
}

void init_erlix_list(){
  erlix_cErlixList=rb_define_class("ErlixList",rb_cObject);

  rb_define_alloc_func(erlix_cErlixList,erlix_list_alloc);
  rb_define_method(erlix_cErlixList,"initialize",erlix_list_init,1);
  rb_define_method(erlix_cErlixList,"head",erlix_list_head,0);
  rb_define_method(erlix_cErlixList,"tail",erlix_list_tail,0);
  rb_define_method(erlix_cErlixList,"cons",erlix_list_cons,1);
  rb_define_method(erlix_cErlixList,"to_a",erlix_list_to_ary,0);
  rb_define_method(erlix_cErlixList,"size",erlix_list_size,0);
  rb_define_method(erlix_cErlixList,"inspect",erlix_list_inspect,0);
  rb_define_method(erlix_cErlixList,"etype",erlix_list_etype,0);

  rb_define_singleton_method(erlix_cErlixList,"[]",erlix_list_create,-1);

  rb_include_module(erlix_cErlixList,erlix_mErlixTerm);
}
