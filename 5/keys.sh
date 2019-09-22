#!/usr/bin/expect
# send the key from host to qemu
spawn ssh-copy-id root@localhost -p 10022
expect "password"
send "root\r"
interact

# generate keys at qemu
spawn ssh root@localhost -p 10022
expect "# "
send "ssh-keygen -f /root/.ssh/id_rsa -t rsa -N ''\r"
expect "# "
send "exit\r"
interact

# send the key from qemu to host
spawn scp -P 10022 root@localhost:/root/.ssh/id_rsa.pub qemu_id_rsa.pub
interact

# add the qemu key to host's authorized keys and clean up
spawn bash
send "cat qemu_id_rsa.pub >> ~/.ssh/authorized_keys\r"
send "rm qemu_id_rsa.pub\r"
interact
