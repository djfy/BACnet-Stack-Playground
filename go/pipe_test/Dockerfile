FROM docker.io/golang:1.13-alpine3.10 AS builder
WORKDIR /build
# Copy relevant directories within the current context to the workdir.
COPY . .
# Get external modules.
RUN go mod vendor
RUN GOOS=linux go build ./cmd/pipe_test/main.go


FROM docker.io/alpine:3.10
COPY --from=builder /build/main /pipe_test/main

ENTRYPOINT ["/pipe_test/main"]





