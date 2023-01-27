FROM ubuntu:22.04

ARG ZSDK_VERSION=0.15.2
ARG CMAKE_VERSION=3.24.1
ARG WGET_ARGS="-q --show-progress --progress=bar:force:noscroll --no-check-certificate"

ARG UID=1000
ARG GID=1000

ENV DEBIAN_FRONTEND noninteractive

RUN dpkg --add-architecture i386 && \
	apt-get -y update && \
	apt-get -y upgrade && \
	apt-get install --no-install-recommends -y \
	gnupg \
	ca-certificates && \
	apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF && \
	echo "deb https://download.mono-project.com/repo/ubuntu stable-bionic main" | tee /etc/apt/sources.list.d/mono-official-stable.list && \
	apt-get -y update && \
	apt-get install --no-install-recommends -y \
	software-properties-common \
	lsb-release \
	autoconf \
	automake \
	bison \
	build-essential \
	ccache \
	chrpath \
	cpio \
	device-tree-compiler \
	dfu-util \
	diffstat \
	dos2unix \
	doxygen \
	file \
	flex \
	g++ \
	gawk \
	gcc \
	gcc-multilib \
	g++-multilib \
	git \
	git-core \
	help2man \
	iproute2 \
	lcov \
	libglib2.0-dev \
	liblocale-gettext-perl \
	libncurses5-dev \
	libpopt0 \
	libsdl2-dev:i386 \
	libsdl1.2-dev \
	libsdl2-dev \
	libssl-dev \
	libtool \
	libtool-bin \
	locales \
	make \
	minicom \
	nano \
	net-tools \
	ninja-build \
	pkg-config \
	protobuf-compiler \
	python3-dev \
	python3-pip \
	python3-ply \
	python3-setuptools \
	python3-tk \
	python3-wheel \
	python-is-python3 \
	qperf \
	screen \
	socat \
	srecord \
	sudo \
	texinfo \
	tmux \
	unzip \
	udev \
	wget \
	vim \
	xz-utils && \
	rm -rf /var/lib/apt/lists/*

RUN locale-gen en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# install cmake 
RUN cd /tmp && wget ${WGET_ARGS} https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.sh && \
	chmod +x cmake-${CMAKE_VERSION}-Linux-x86_64.sh && \
	./cmake-${CMAKE_VERSION}-Linux-x86_64.sh --skip-license --prefix=/usr/local && \
	rm -f ./cmake-${CMAKE_VERSION}-Linux-x86_64.sh

# Zephyr and python dependencies
RUN pip3 install wheel pip west 
ENV PATH=/.local/bin:$PATH
RUN west init /workingdir && cd /workingdir && west update
RUN cd /workingdir && west zephyr-export && pip3 install -r /workingdir/zephyr/scripts/requirements.txt

# install ESP32 toolchain
RUN cd /workingdir && west blobs fetch hal_espressif && west update && west zephyr-export
ENV ESPRESSIF_TOOLCHAIN_PATH="/root/.espressif/tools/zephyr/xtensa-esp32-elf"
ENV PATH=$ESPRESSIF_TOOLCHAIN_PATH/bin:$PATH

# Install toolchain
RUN cd /opt && \
        wget -qO - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZSDK_VERSION}/zephyr-sdk-${ZSDK_VERSION}_linux-x86_64.tar.gz \
        | tar -xz && \
        chmod +x zephyr-sdk-${ZSDK_VERSION}/setup.sh && \
        zephyr-sdk-${ZSDK_VERSION}/setup.sh -h -c -t arm-zephyr-eabi 


WORKDIR /workingdir

