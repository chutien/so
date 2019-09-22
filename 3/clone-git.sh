#!/usr/bin/expect
# clone git repository
spawn ssh root@localhost -p 10022
expect "# "
send "git clone ssh://"
expect "tien@10.0.2.2's password: "
send "1c9H6o8y!"
expect "# "
send "exit\r"
interact
