FROM debian:11-slim as build
MAINTAINER Nitrokey <info@nitrokey.com>

RUN apt-get update  \
  && apt-get install -y --no-install-recommends \
    ca-certificates \
    wget \
    bzip2 \
    xz-utils \
  && rm -rf /var/lib/apt/lists/*

ENV URL    https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
#ENV URLSHA https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2.sha256asc
ENV FILENAME gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2

RUN set -eux; \
	wget "${URL}" && \
#	wget "${URLSHA}" && \
#    sha256sum -c < ${FILENAME}.sha256asc && \
  ls && \
	tar -C /opt -xf ./${FILENAME} && \
	rm ${FILENAME};

FROM debian:11-slim as target
COPY --from=build /opt /
RUN apt-get update  \
  && apt-get install -y --no-install-recommends \
    make \
    git \
    openocd \
    python \
    srecord \
    dfu-util \
  && rm -rf /var/lib/apt/lists/*
ENV PATH="/gcc-arm-none-eabi-8-2018-q4-major/bin:${PATH}"
