/*
 * project : erlix
 * author : kdr2
 *
 */

#include <stdlib.h>

#include "erlix_term.h"
#include "erlix_int.h"
#include "erlix_uint.h"
#include "erlix_float.h"
#include "erlix_binary.h"
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
  free(eterm);
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


//obj.to_s
static void fill_string(VALUE *str,ETERM *ep);
VALUE erlix_term_to_str(VALUE self){
  ErlixTerm *eterm;
  Data_Get_Struct(self,ErlixTerm,eterm);
  ETERM *ep=eterm->term;
  VALUE ret=rb_str_new2("");
  fill_string(&ret,ep);
  return ret;
}

static int is_printable_list(const ETERM* term)
{
  while (ERL_TYPE(term) == ERL_LIST) {
    ETERM* head = (ETERM*)(ERL_CONS_HEAD(term));
    if (ERL_INT_VALUE(head)<0 || ERL_INT_VALUE(head)>255) {
      return 0;
    }
    if (ERL_INT_VALUE(head) < ' ') {
      switch (ERL_INT_VALUE(head)) {
      case '\n':
      case '\r':
      case '\t':
      case '\v':
      case '\b':
      case '\f':
        break;
      default:
        return 0;
      }
    }
    term = (ETERM*)(ERL_CONS_TAIL(term));
  }
  return ERL_IS_EMPTY_LIST(term);
}

static void fill_printable_list(VALUE *str, const ETERM* ep){
  ID concat=rb_intern("concat");
  rb_funcall(*str,concat,1,rb_str_new2("\""));
  while (ERL_IS_CONS(ep)) {
    int c = ERL_INT_VALUE((ETERM*)ERL_CONS_HEAD(ep));
    if (c >= ' ') {
      char tmp_buf[2];
      tmp_buf[0]=c;
      tmp_buf[1]=0;
      rb_funcall(*str,concat,1,rb_str_new2(tmp_buf));
    }
    else {
      if(c=='\n'){
        rb_funcall(*str,concat,1,rb_str_new2("\\n"));
      }else if(c=='\r'){
        rb_funcall(*str,concat,1,rb_str_new2("\\r"));
      }else if(c=='\t'){
        rb_funcall(*str,concat,1,rb_str_new2("\\t"));
      }else if(c=='\v'){
        rb_funcall(*str,concat,1,rb_str_new2("\\v"));
      }else if(c=='\b'){
        rb_funcall(*str,concat,1,rb_str_new2("\\b"));
      }else if(c=='\f'){
        rb_funcall(*str,concat,1,rb_str_new2("\\f"));
      }else{
        char tmp_buf[8];
        memset(tmp_buf,0,8);
        sprintf(tmp_buf,"\\%o",c);
        rb_funcall(*str,concat,1,rb_str_new2(tmp_buf));
      }
    }
    ep = (ETERM*)ERL_CONS_TAIL(ep);
  }
  rb_funcall(*str,concat,1,rb_str_new2("\""));
}


static void fill_string(VALUE *str,ETERM *ep){
  ID concat=rb_intern("concat");

  int j,i,doquote;
  if (!ep) return;

  j = i = doquote = 0;

  if(ERL_IS_ATOM(ep)){
    /* FIXME: what if some weird locale is in use? */
    if (!islower((int)ERL_ATOM_PTR(ep)[0]))
      doquote = 1;
    for (i = 0; !doquote && i < ERL_ATOM_SIZE(ep); i++){
      doquote = !(isalnum((int)ERL_ATOM_PTR(ep)[i])
                  || (ERL_ATOM_PTR(ep)[i] == '_'));
    }
    if (doquote) {
      rb_funcall(*str,concat,1,rb_str_new2("\'"));
    }
    rb_funcall(*str,concat,1,rb_str_new2(ERL_ATOM_PTR(ep)));
    if(doquote){
      rb_funcall(*str,concat,1,rb_str_new2("\'"));
    }
  }else if(ERL_IS_PID(ep)){
    char tmp_buf_pid[24];
    memset(tmp_buf_pid,0,24);
    sprintf(tmp_buf_pid,"<%s.%d.%d>",ERL_PID_NODE(ep),ERL_PID_NUMBER(ep), ERL_PID_SERIAL(ep));
    rb_funcall(*str,concat,1,rb_str_new2(tmp_buf_pid));
  }else if(ERL_IS_PORT(ep)){
    rb_funcall(*str,concat,1,rb_str_new2("#Port"));
  }else if(ERL_IS_REF(ep)){
    rb_funcall(*str,concat,1,rb_str_new2("#Ref"));
  }else if(ERL_IS_EMPTY_LIST(ep)){
    rb_funcall(*str,concat,1,rb_str_new2("[]"));
  }else if(ERL_IS_LIST(ep)){
    if (is_printable_list(ep)) {
      fill_printable_list(str,ep);
    } else {
      rb_funcall(*str,concat,1,rb_str_new2("["));
      ETERM *tl=ep,*hd,*tmp;
      while (ERL_IS_CONS(tl)){
        hd=erl_hd(tl);
        fill_string(str,hd);
        //erl_free_term(hd);
        tmp = erl_tl(tl);
        //if(tl!=ep)erl_free_term(tl);
        tl=tmp;
        if (ERL_IS_CONS(tl)) {
          rb_funcall(*str,concat,1,rb_str_new2(","));
        }
      }
      if (!ERL_IS_EMPTY_LIST(tl)) {
        rb_funcall(*str,concat,1,rb_str_new2("|"));
        fill_string(str, tl);
      }else{
        //erl_free_term(tl);
      }
      rb_funcall(*str,concat,1,rb_str_new2("]"));
    }
  }else if(ERL_IS_TUPLE(ep)){
    rb_funcall(*str,concat,1,rb_str_new2("{"));
    for (i=0; i < ERL_TUPLE_SIZE(ep); i++) {
      ETERM *e=erl_element(i+1,ep);
      fill_string(str,e);
      //erl_free_term(e);
      if (i != ERL_TUPLE_SIZE(ep)-1) {
        rb_funcall(*str,concat,1,rb_str_new2(","));
      }
    }
    rb_funcall(*str,concat,1,rb_str_new2("}"));
  }else if(ERL_IS_BINARY(ep)){
      rb_funcall(*str,concat,1,rb_str_new2("#Bin"));
  }else if(ERL_IS_INTEGER(ep)){
      char tmp_buf_num[24];
      memset(tmp_buf_num,0,24);
      sprintf(tmp_buf_num,"%d", ERL_INT_VALUE(ep));
      rb_funcall(*str,concat,1,rb_str_new2(tmp_buf_num));
  }else if(ERL_IS_UNSIGNED_INTEGER(ep)){
    char tmp_buf_unum[24];
    memset(tmp_buf_unum,0,24);
    sprintf(tmp_buf_unum,"%u", ERL_INT_UVALUE(ep));
    rb_funcall(*str,concat,1,rb_str_new2(tmp_buf_unum));
  }else if(ERL_IS_FLOAT(ep)){
    char tmp_buf_float[24];
    memset(tmp_buf_float,0,24);
    sprintf(tmp_buf_float,"%f", ERL_FLOAT_VALUE(ep));
    rb_funcall(*str,concat,1,rb_str_new2(tmp_buf_float));
  }else{
    rb_funcall(*str,concat,1,rb_str_new2("*Unknow*"));
    //rb_raise(rb_eException,"ErlixTerm.to_s: Bad type of term !");
  }
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
  return ret;
}

ETERM *erlix_auto_conv(VALUE v){
  if(SYMBOL_P(v)){
    return erl_mk_atom(rb_id2name(SYM2ID(v)));
  }else if(FIXNUM_P(v)){
    return erl_mk_int(FIX2INT(v));
  }else if(TYPE(v)==T_FLOAT){
    return erl_mk_float(NUM2DBL(v));
  }else if(TYPE(v)==T_STRING){
    return erl_mk_estring(RSTRING(v)->ptr,RSTRING(v)->len);
  }
  return NULL;
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
    ret=erlix_int_alloc(erlix_cErlixInt);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_UINT){
    ret=erlix_uint_alloc(erlix_cErlixUInt);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else if(type==TYPE_FLOAT){
    ret=erlix_float_alloc(erlix_cErlixFloat);
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
    ret=erlix_ref_alloc(erlix_cErlixRef);
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
    ret=erlix_binary_alloc(erlix_cErlixBinary);
    ErlixTerm *eterm;
    Data_Get_Struct(ret,ErlixTerm,eterm);
    eterm->term=term;
    return ret;
  }else{
    rb_raise(rb_eException,"Unknow Erlang Type!");
    //TODO OTHER TYPE!!!
    return Qnil;
  }
}


void init_erlix_term(){
  erlix_mErlixTerm=rb_define_module("ErlixTerm");
  rb_define_method(erlix_mErlixTerm,"initialize_copy",erlix_term_init_copy,1);
  rb_define_method(erlix_mErlixTerm,"eql?",erlix_term_eql,1);
  rb_define_method(erlix_mErlixTerm,"to_s",erlix_term_to_str,0);
  rb_define_method(erlix_mErlixTerm,"match",erlix_term_match,1);
  rb_define_method(erlix_mErlixTerm,"mget",erlix_term_mget,2);
  rb_define_method(erlix_mErlixTerm,"==",erlix_term_eql,1);
}
