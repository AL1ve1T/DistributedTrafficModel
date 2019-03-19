# ------------------------------------------------------------
# Build

FROM gcc:latest as build

WORKDIR /usr/src/app

RUN apt-get update && \
    apt-get install -y \
      libboost-dev libboost-filesystem-dev \
      libgtest-dev \
      cmake \
    && \
    cmake -DCMAKE_BUILD_TYPE=Release /usr/src/gtest && \
    cmake --build . && \
    mv lib*.a /usr/lib

ADD ./src /app/src

WORKDIR /app/build

RUN cmake ../src && \
    cmake --build . 

# ------------------------------------------------------------
# Run 

FROM ubuntu:latest

RUN groupadd -r sample && useradd -r -g sample sample
USER sample

WORKDIR /app

COPY --from=build /app/build/build/debug/app.exe .

ENTRYPOINT ["./app.exe"]