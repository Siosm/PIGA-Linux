#!/bin/bash

number=`cat /root/test`

if [ $number -ne 30 ]
then
	#echo 0 > /selinux/avc/cache_threshold
	$@ &
	PID=$!
	kill -STOP $PID
	echo $PID

	bash /root/trace-me $PID
	wait $PID

	DEBUGFS=`grep debugfs /proc/mounts | awk '{ print $2; }'`
	echo 0 > $DEBUGFS/tracing/tracing_on

	if [  -e "/test/trace.$PID.txt" ]
	then
		cp $DEBUGFS/tracing/trace /test/trace.$PID.$RANDOM.txt
	else
		cp $DEBUGFS/tracing/trace /test/trace.$PID.txt
	fi
	number=`expr $number + 1`
	echo $number > /root/test
	#reboot
else

	exit 0 
fi
