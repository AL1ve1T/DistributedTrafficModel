#!/bin/bash
#
#   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
#                               <aliveit.elnur@gmail.com>
# 
docker run -d -p 8080:8000 traffic-model 0
docker run -d -p 8081:8000 traffic-model 1
# docker run -d -p 8082:8000 traffic-model 2
# docker run -d -p 8083:8000 traffic-model 3
