#!/usr/bin/ruby
#
# project : erlix
# author : kdr2
#

require "mkmf"

dir_config("ei")

if have_library("ei","erl_init") and
    have_library("erl_interface","erl_init")
then
  create_makefile("erlix")
else
  puts "error: erl_interface not found!"
end
