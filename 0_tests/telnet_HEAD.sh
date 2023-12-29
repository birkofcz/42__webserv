#!/bin/bash

(
echo "OPEN www.irozhlas.cz 80"
sleep 2
echo "HEAD / HTTP/1.1"
echo "Host: www.irozhlas.cz"
echo ""
sleep 2
) | telnet

