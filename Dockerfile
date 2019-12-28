FROM debian:9-slim AS base

RUN set -x \
	&& apt-get update \
	&& apt-get upgrade \
	&& apt-get -y install build-essential \
	&& apt-get -y install wget


FROM base AS builder
# Environment variables
WORKDIR /bacnet_build
# Copy all source files.
COPY . .
# Build.
RUN set -x \
    && make clean all


FROM base AS final
ENV BACNET_STACK_VERSION 0.8.5
ENV BACNET_DEVICE_INSTANCE 5678
ENV PIPE_PATH /tmp/test_vol/TEST_FIFO
COPY --from=builder /bacnet_build/bin/* /bin/

ENTRYPOINT ["/bin/bacserv", "5678"]
#CMD /bin/bacserv $BACNET_DEVICE_INSTANCE