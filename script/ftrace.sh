#!/bin/bash

mount -t debugfs nodev /sys/kernel/debugfs

ln -sf /sys/kernel/debug /debug

cat /debug/tracing/available_tracers

echo function_graph >/debug/tracing/current_tracer

echo 1 >/debug/tracing/tracing_enabled
