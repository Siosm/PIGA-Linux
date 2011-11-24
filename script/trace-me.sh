#!/bin/sh
#DEBUGFS=`grep debugfs /proc/mounts | awk '{ print $2; }'`
#echo 0 > $DEBUGFS/tracing/tracing_on;
#echo $$ > $DEBUGFS/tracing/set_ftrace_pid;
#echo function_graph > $DEBUGFS/tracing/current_tracer; 
#echo 1 > $DEBUGFS/tracing/tracing_on"
#exec $*
#echo -1 > $DEBUGFS/tracing/set_ftrace_pid;
#echo 0 > $DEBUGFS/tracing/tracing_on"

DEBUGFS=`grep debugfs /proc/mounts | awk '{ print $2; }'`
echo 0 > $DEBUGFS/tracing/tracing_on
echo $1 > $DEBUGFS/tracing/set_ftrace_pid
echo function_graph > $DEBUGFS/tracing/current_tracer 
echo 1 > $DEBUGFS/tracing/tracing_on
TOTO=$1

kill -18  $TOTO
