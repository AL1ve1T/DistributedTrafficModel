#!/bin/bash

#
#   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
#                               <aliveit.elnur@gmail.com>
# 

docker run -d -p 8080:80 traffic-model
docker run -d -p 8081:80 traffic-model
docker run -d -p 8082:80 traffic-model
docker run -d -p 8083:80 traffic-model
