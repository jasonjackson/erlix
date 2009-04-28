/*
 * project : erlix
 * author : kdr2
 *
 */

#include <unistd.h>
#include <stdlib.h>

#include "erlix_message.h"
#include "erlix_connection.h"

extern ErlixNode* erlix_node;
extern VALUE erlix_cErlixMessage;

VALUE erlix_cErlixConnection;

ErlixConnection* new_erlix_connection(){
  ErlixConnection* ret=(ErlixConnection*)malloc(sizeof(ErlixConnection));
  ret->status=0;
  return ret;
}

void free_erlix_connection(void *econ){
  ErlixConnection* ep=econ;
  ERLIX_CONNECTION_DEAD(ep);
  close(ep->sock_fd);
  free(ep);
}

static VALUE erlix_connection_alloc(VALUE klass){
  ErlixConnection* ec=new_erlix_connection();
  VALUE obj;
  obj=Data_Wrap_Struct(klass,0,free_erlix_connection,ec);
  return obj;
}

static VALUE erlix_connection_init(VALUE self,VALUE pnode){
  if(erlix_node==NULL){
    rb_raise(rb_eException,"call ErlixNode.init(name,cookie) first please!");
    return Qnil;
  }
  VALUE node=StringValue(pnode);
  int sockfd;
  if((sockfd = erl_connect(RSTRING(node)->ptr))<0){
    rb_raise(rb_eException,"erl_connect failed");
    return Qnil;
  }

  ErlixConnection *conn;
  Data_Get_Struct(self,ErlixConnection,conn);
  conn->peer_node=rb_str_new2(RSTRING(node)->ptr);
  conn->sock_fd=sockfd;
  ERLIX_CONNECTION_BORN(conn);
  return self;
}


static VALUE erlix_connection_init_copy(VALUE copy,VALUE orig){
  if(copy==orig)return copy;
  if(TYPE(orig)!=T_DATA||RDATA(orig)->dfree!=(RUBY_DATA_FUNC)free_erlix_connection){
    rb_raise(rb_eTypeError,"wrong argument type!");
  }
  ErlixConnection *tcopy,*tsrc;
  if(!ERLIX_CONNECTION_ALIVE(tsrc)){
    rb_raise(rb_eException,"the connection has been disconnected!");
    return Qnil;
  }
  Data_Get_Struct(copy,ErlixConnection,tcopy);
  Data_Get_Struct(orig,ErlixConnection,tsrc);
  tcopy->status=tsrc->status;
  tcopy->peer_node=rb_str_new2(RSTRING(tsrc->peer_node)->ptr);
  tcopy->sock_fd=tsrc->sock_fd;

  return copy;
}


static VALUE erlix_connection_to_str(VALUE self);

//peer node name
static VALUE erlix_connection_peer(VALUE self){
  ErlixConnection *con;
  if(!ERLIX_CONNECTION_ALIVE(con)){
    rb_raise(rb_eException,"the connection has been disconnected!");
    return Qnil;
  }
  Data_Get_Struct(self,ErlixConnection,con);
  return con->peer_node;
}

static VALUE erlix_connection_etype(VALUE self){
  return rb_str_new2("erlix_connection");
}

static VALUE erlix_connection_mkpid(VALUE self);

static VALUE erlix_connection_send(VALUE self,VALUE pid,VALUE term){
  if(!IS_ETERM(term)){
    rb_raise(rb_eTypeError,"wrong argument type,the data is not an ErlixTerm!");
    return Qnil;
  }
  ErlixConnection *con;
  Data_Get_Struct(self,ErlixConnection,con);
  ErlixTerm * ep;
  Data_Get_Struct(term,ErlixTerm,ep);
  if(!ERLIX_CONNECTION_ALIVE(con)){
    rb_raise(rb_eException,"the connection has been disconnected!");
    return Qnil;
  }
  if(TYPE(pid)==T_STRING){
    if(erl_reg_send(con->sock_fd,RSTRING(pid)->ptr,ep->term)==1){
      return term;
    }else if(erl_errno==EIO){
      ERLIX_CONNECTION_DEAD(con);
      rb_raise(rb_eException,"IO Error!");
      return Qnil;
    }
  }
  return Qnil;
}

static VALUE erlix_connection_recv(VALUE self){
  ErlixConnection *con;
  Data_Get_Struct(self,ErlixConnection,con);

  if(!ERLIX_CONNECTION_ALIVE(con)){
    rb_raise(rb_eException,"the connection has been disconnected!");
    return Qnil;
  }
  ErlMessage emsg;
  for(;;){
    int ret=erl_receive_msg(con->sock_fd,NULL,0,&emsg);
    if (ret == ERL_MSG) {
      VALUE m=erlix_message_alloc(erlix_cErlixMessage);
      ErlMessage *newm;
      Data_Get_Struct(m,ErlMessage,newm);
      newm->type=emsg.type;
      if(emsg.type==ERL_SEND){
        newm->msg=erl_copy_term(emsg.msg);
        newm->to=erl_copy_term(emsg.to);
      }else if(emsg.type==ERL_REG_SEND){
        newm->msg=erl_copy_term(emsg.msg);
        newm->from=erl_copy_term(emsg.from);
      }else if(emsg.type==ERL_LINK ||emsg.type==ERL_UNLINK){
        newm->from=erl_copy_term(emsg.from);
        newm->to=erl_copy_term(emsg.to);
      }else if(emsg.type==ERL_EXIT){
        newm->msg=erl_copy_term(emsg.msg);
        newm->from=erl_copy_term(emsg.from);
        newm->to=erl_copy_term(emsg.to);
      }
      memcpy(newm->to_name,emsg.to_name,MAXREGLEN);
      return m;
    }else if(ret == ERL_TICK){
      continue;
    }else{
      rb_raise(rb_eException,"receive error!");
      return Qnil;
    }
  }
  return Qnil;
}


void init_erlix_connection(){
  erlix_cErlixConnection=rb_define_class("ErlixConnection",rb_cObject);
  rb_define_alloc_func(erlix_cErlixConnection,erlix_connection_alloc);
  rb_define_method(erlix_cErlixConnection,"initialize",erlix_connection_init,1);
  rb_define_method(erlix_cErlixConnection,"initialize_copy",erlix_connection_init_copy,1);
  rb_define_method(erlix_cErlixConnection,"esend",erlix_connection_send,2);
  rb_define_method(erlix_cErlixConnection,"erecv",erlix_connection_recv,0);
}
