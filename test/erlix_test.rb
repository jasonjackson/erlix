#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require "erlix"

# init ErlixNode,
#   *the first argument is the short-name of the ErlixNode
#   *the second argument is the erlang cookie, use nil it's will read ~/.erlang.cookie
# after init, my ErlixNode's name is inited to "ruby@kdr2-pc"
ErlixNode.init("ruby",nil)

# connect to the real Erlang-Node:
c=ErlixConnection.new("foo@kdr2-pc")
puts "connect ok"


#rpc call
fmt=ErlixList.new("abc~n")
tmp=ErlixList.new(nil)
args=ErlixList.new([fmt,tmp])
ret=c.rpc("io","format",args)
puts ret;
puts ret.class

# create a new Pid with the connection
# we will use this Pid as the FromPid
p=ErlixPid.new(c)

# make a ErlixTuple {Pid,test_atom} and send it to the real erlang-node
c.esend("my_pid",ErlixTuple.new([p,ErlixAtom.new("test_atom")]))
puts "send ok"

# start a new thread to receive the msg from the real erlang-node
puts "receiving"
t=Thread.start("thread recv"){ |name|
  while true do
    puts "+++++"
    m=c.erecv
    puts m.mtype
    m.message.puts
    puts m.class
    puts m.from
    puts m.to
    puts "-----"
  end
}

#send more test:
3.times{
c.esend("my_pid",ErlixTuple.new([p,ErlixAtom.new("test_atom")]))
}

t.join
