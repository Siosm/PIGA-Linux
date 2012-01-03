#!/bin/bash

function exitWithError
{
	echo "[T1] "$1
	exit 1
}

if [ `id -Z` != "root:sysadm_r:sysadm_t" ]; then
	exitWithError "Are you in the correct role ?"
	exit 1
fi

echo "[T1] Test 1:"

echo "[T1] Disable PIGA"
echo 0 > /sys/fs/piga/status

echo "[T1] Loading testpol.pol"
./ppp < ../pol/testpol.pol || exitWithError "Policy failed to load"
echo "[T1] Policy loaded"

echo 0 > /sys/fs/piga/mode
echo 1 > /sys/fs/piga/status
echo "[T1] PIGA Enabled"

cat /etc/localtime > /dev/null
cat /etc/localtime > /tmp/tmp.log
cat /tmp/tmp.log > /dev/null
echo "[T1] Should have been denied now"

echo "[T1] Stopping PIGA"
echo 0 > /sys/fs/piga/status

echo "[T1] Test 1: End"
