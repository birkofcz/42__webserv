#!/bin/bash

(
echo "OPEN www.google.com 80"
sleep 2
echo "HEAD / HTTP/1.1"
echo "Host: www.google.com"
echo ""
sleep 2
) | telnet

