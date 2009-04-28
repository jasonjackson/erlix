%% by kdr2

-module(test).
-compile(export_all).

loop()->
    receive
        {From,Msg} ->
            io:format("From:[~w], MSG From Ruby:[~w] ~n",[From,Msg]),
            From ! Msg,
            loop()
    end.

run()->
    Pid=spawn(fun loop/0),
    %%ruby根据注册名my_pid发消息
    register(my_pid,Pid).
