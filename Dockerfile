#
#   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
#                               <aliveit.elnur@gmail.com>
# 
# ------------------------------------------------------------
# Build

FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y \
      libboost-system-dev libboost-date-time-dev \
      libboost-serialization-dev \
      libboost-thread-dev \
      g++ \
      cmake 

ADD . /app

WORKDIR /app

RUN cmake . && \
    make

# ------------------------------------------------------------
# Run 

RUN useradd -m docker && echo \ 
  "docker:docker" | chpasswd && adduser docker sudo
USER docker

ENTRYPOINT [ "/app/build/debug/app" ]