#!/bin/bash
rm -r /home/rich/A2/linux-5.19.9
echo "removed file"

tar -xf /root/linux-5.19.9.tar.xz
echo "extracted file"

cp /home/rich/practise/config-rev-9-gold.txt /home/rich/A2/linux-5.19.9/.config
echo "copied file"

cd /home/rich/A2/linux-5.19.9
echo "directory changed"

time make -j4 
echo "did make"

echo "completed"
