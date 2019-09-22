#!/bin/bash
mkdir -p old/usr/src/include
cp usr/src/include/unistd.h old/usr/src/include/unistd.h

mkdir -p old/usr/src/minix/include/minix
cp usr/src/minix/include/minix/callnr.h old/usr/src/minix/include/minix/callnr.h
cp usr/src/minix/include/minix/config.h old/usr/src/minix/include/minix/config.h

mkdir -p old/usr/include/
cp usr/include/unistd.h old/usr/include/unistd.h
cp usr/include/minix/config.h old/usr/include/minix/config.h

mkdir -p old/usr/include/minix
cp usr/include/minix/callnr.h old/usr/include/minix/callnr.h
cp usr/include/minix/config.h old/usr/include/minix/config.h

mkdir -p old/usr/src/minix/servers/pm
cp usr/src/minix/servers/pm/proto.h old/usr/src/minix/servers/pm/proto.h
cp usr/src/minix/servers/pm/table.c old/usr/src/minix/servers/pm/table.c
cp usr/src/minix/servers/pm/Makefile old/usr/src/minix/servers/pm/Makefile

mkdir -p old/usr/src/minix/servers/sched
cp usr/src/minix/servers/sched/schedule.c old/usr/src/minix/servers/sched/schedule.c

mkdir -p old/usr/src/lib/libc/misc/
cp usr/src/lib/libc/misc/Makefile.inc old/usr/src/lib/libc/misc/Makefile.inc

mkdir -p old/usr/src/minix/kernel/
cp usr/src/minix/kernel/proc.h old/usr/src/minix/kernel/proc.h
cp usr/src/minix/kernel/proc.c old/usr/src/minix/kernel/proc.c
