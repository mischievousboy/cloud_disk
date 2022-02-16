ARG BUILD_OS=ubuntu
ARG BUILD_TAG=20.04
#
FROM ${BUILD_OS}:${BUILD_TAG} AS base
#RUN set -ex;         \
#    apt-get update;  \
#    apt-get install -y liblog4cplus-1.1-9
#
#FROM base AS builder
#
#RUN set -ex;                                                                      \
#    apt-get install -y g++ build-essential autoconf libtool pkg-config git curl cmake liblog4cplus-dev;                                \
#    mkdir -p /usr/src;                                                            \
#    cd /usr/src;                                                                  \
#    git config --global http.proxy ${HTTP_PROXY};   \
#    git clone -b v1.42.0 --depth=1 https://github.com/grpc/grpc.git;  \
#    cd /usr/src/grpc; git submodule update --init;  mkdir build ; cd build; \
#    cmake  .. ; make; make install
#

FROM build-os:v2.0 AS builder

WORKDIR /opt/src

COPY ./src  .

RUN set -ex;   \
    cd /opt/src; mkdir build; cd build ; cmake ..; make ; mkdir bin; cp greeter_server bin; cd bin ; cp $(ldd ../greeter_server | awk '{if (match($3,"/")){ printf("%s "),$3 } }') .



FROM base AS runtime
ENV LD_LIBRARY_PATH=/app
COPY --from=builder /opt/src/build/bin/* /app/

ENTRYPOINT ["/app/greeter_server"]
