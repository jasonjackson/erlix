#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require "erlix"

#将ruby进程作为erlang node,先初始化
#第一个参数是erl-node-name(-sname 参数,)第二个是cookie,nil时会读~/.erlang.cookie
ErlixNode.init("ruby",nil)

#连接一个真实的erl-node(用"erl -sname foo"启动的,我的hostname是kdr2-pc)
c=ErlixConnection.new("foo@kdr2-pc")
puts "connect ok"

#新建一个pid,发给foo@kdr2-pc,对方拿到这个pid可以回信
p=ErlixPid.new(c)

#发消息,ErlixTuple.new([p,ErlixAtom.new("test_atom")])就是{<Pid>,test_atom)}
c.esend("my_pid",ErlixTuple.new([p,ErlixAtom.new("test_atom")]))

puts "send ok"

#新起线程收消息
puts "receiving"
t=Thread.new{
  while true do
    m=c.erecv
    puts m.mtype
    m.message.puts
    puts m.class
    puts m.from
    puts m.to
  end
}

t.join
