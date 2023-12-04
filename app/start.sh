#!/bin/bash
conda activate GongXun
cd /home/jetson/GongXun2/app
sudo chmod 777 /dev/ttyTHS1
/home/jetson/archiconda3/envs/GongXun/bin/python /home/jetson/GongXun2/app/main.py
exit 0
