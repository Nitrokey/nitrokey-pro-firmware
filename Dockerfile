FROM ubuntu:latest
MAINTAINER Nitrokey <info@nitrokey.com>

# Install necessary packages
RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    ca-certificates \
    make \
    wget \
    srecord \
    bzip2 \
    git \
    python3 \
    python3-pip \
    python-is-python3 \
    xz-utils \
  && rm -rf /var/lib/apt/lists/*

ENV URL    https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz
ENV URLSHA https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz.sha256asc
ENV FILENAME gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz
ENV GCC_NAME gcc-arm-11.2-2022.02-x86_64-arm-none-eabi
RUN set -eux; \
  wget "${URL}" && \
  wget "${URLSHA}" && \
        sha256sum -c < ${FILENAME}.sha256asc && \
  tar -C /opt -xf ${FILENAME} && \
  rm ${FILENAME};

ENV PATH="/opt/${GCC_NAME}/bin/:${PATH}"