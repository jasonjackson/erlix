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
#include "erlix_pid.h"
#include "erlix_ref.h"
#include "erlix_atom.h"
#include "erlix_list.h"
#include "erlix_node.h"
#include "erlix_tuple.h"
#include "erlix_binary.h"
#include "erlix_message.h"
#include "erlix_connection.h"



void Init_erlix(){
  erl_init(NULL,0);
  init_erlix_term();
  init_erlix_int();
  init_erlix_uint();
  init_erlix_float();
  init_erlix_pid();
  init_erlix_ref();
  init_erlix_atom();
  init_erlix_list();
  init_erlix_node();
  init_erlix_tuple();
  init_erlix_binary();
  init_erlix_message();
  init_erlix_connection();
}
