set -e    # Stop script when en error occurs

apt-get update
apt-get -y upgrade

apt-get -y install nginx htop openssh-server openssh-client libssh-dev iptables-persistent mc links
apt-get -y install cmake gcc make python3 python3-pip python3-venv build-essential gdb
apt-get -y install libssl-dev libx11-dev libxrandr-dev mesa-common-dev libgl1-mesa-dev libasound2-dev
