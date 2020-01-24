# Based on https://hub.docker.com/r/fh1ch/bacstack-compliance-docker/dockerfile
FROM debian:9-slim

# Environment variables
ENV BACNET_STACK_VERSION 0.8.5
ENV BACNET_DEVICE_INSTANCE 1234

# Install dependencies
RUN apt-get update && apt-get -y \
    install \
    build-essential wget && \
    rm -rf /var/lib/apt/lists/*

# Install bacnet-stack and tidy-up sources
# ADD bacnet-stack-$BACNET_STACK_VERSION.tgz .
RUN wget -qO- "https://sourceforge.net/projects/bacnet/files/bacnet-stack/bacnet-stack-$BACNET_STACK_VERSION/bacnet-stack-$BACNET_STACK_VERSION.tgz/download" | tar -xzf - && \
    cd bacnet-stack-$BACNET_STACK_VERSION && \
    make clean all && \
    cd .. && \
    mv bacnet-stack-$BACNET_STACK_VERSION/bin/* bin && \
    rm -rf bacnet-stack-$BACNET_STACK_VERSION

# Run bacnet-stack simulator
CMD bin/bacserv $(od -An -N2 -i /dev/random)