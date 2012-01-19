#!/bin/bash


EXPECTED_ARGS=2
E_BADARGS=-1


if [ $# -ne $EXPECTED_ARGS ]; then
	echo "Usage: `basename $0` policy script_to_test"
	exit $E_BADARGS
fi


SCRIPT_NAME="$0"
SCRIPT_POL="$1"
SCRIPT_TEST="$2"


function printText
{
	echo "[" $SCRIPT_POL "] " $1
}


function exitWithError
{
	printText $1
	exit 1
}


if [ `id -Z` != "root:sysadm_r:sysadm_t" ]; then
	exitWithError "Are you in the correct role ?"
	exit 1
fi


printText "Begin"

printText "Disable PIGA"
echo 0 > /sys/fs/piga/status

printText "Loading "$SCRIPT_POL
./ppp < $SCRIPT_POL || exitWithError "Policy failed to load"
printText "Policy loaded"

echo 0 > /sys/fs/piga/mode
echo 1 > /sys/fs/piga/status
printText "PIGA Enabled"

cat /etc/localtime > /dev/null
cat /etc/localtime > /tmp/tmp.log
cat /tmp/tmp.log > /dev/null
printText "Should have been denied now"

printText "Stopping PIGA"
echo 0 > /sys/fs/piga/status

printText "End"

dmesg | tail -n30

