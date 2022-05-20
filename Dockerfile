FROM debian:11-slim as build
MAINTAINER Nitrokey <info@nitrokey.com>

RUN apt-get update  \
  && apt-get install -y --no-install-recommends \
    ca-certificates \
    wget \
    xz-utils \
  && rm -rf /var/lib/apt/lists/*

ENV URL    https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz
ENV URLSHA https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz.sha256asc
ENV FILENAME gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz

RUN set -eux; \
	wget "${URL}" && \
	wget "${URLSHA}" && \
    sha256sum -c < ${FILENAME}.sha256asc && \
	tar -C /opt -xf ${FILENAME} && \
	rm ${FILENAME};

FROM debian:11-slim as target
COPY --from=build /opt /
RUN apt-get update  \
  && apt-get install -y --no-install-recommends \
    make \
    git \
  && rm -rf /var/lib/apt/lists/*