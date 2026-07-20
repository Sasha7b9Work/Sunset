#!/bin/bash

cat << EOF | sudo tee /etc/ssh/sshd_config > /dev/null
Include /etc/ssh/sshd_config.d/*.conf
Port 22
AddressFamily inet
ListenAddress 0.0.0.0
PermitRootLogin yes
PasswordAuthentication yes
UsePAM yes
AllowTcpForwarding yes
X11Forwarding no
AcceptEnv LANG LC_*
Subsystem sftp /usr/lib/ssh/sftp-server
EOF

sudo ip addr flush dev enP3p49s0
sudo ip link set enP3p49s0 up
sudo ip addr add 192.168.1.100/24 dev enP3p49s0
sudo ip route add default via 192.168.1.1
echo "nameserver 8.8.8.8" | sudo tee /etc/resolv.conf > /dev/null

sudo systemctl restart ssh
