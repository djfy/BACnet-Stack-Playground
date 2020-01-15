# Intro
This repository exists to experiment with the sourceforge BACnet stack and
node-red for personal and non-commercial use. In general, the purpose is to 
interface a node-red server and a BACnet server. This repository provides the necessary Dockerfiles and docker-compose file to build Docker images and run 
them in a controlled environment. 

# Requirements
Docker Desktop is required to do anything interesting. The recommended install 
method on Windows is with [Chocolatey](https://chocolatey.org/install): 

`choco install docker-desktop --pre`

If you want to build either the Golang project (`./go/pipe_test/`) or the 
modifed bacnet-stack (`./bacstack-modified/`) natively on your machine, then 
you need to install the appropriate dependencies. That is left as an excercise
for the reader.

# Building the Docker images
The top level `docker-compose.yaml` requires two images, `bacmod:1` and 
`pipe_test:1`, that are not provided by DockerHub. These must be built 
on your machine. Unless otherwise stated, you should be in the top-level 
directory of this repository.

To build `bacmod:1`:
- `docker image build --tag bacmod:1 ./bacstack-modifed/`

To build `pipe_test:1`:
- `docker image build --tag pipe_test:1 ./go/pipe_test/`

# Using docker-compose.yaml
The docker-compose.yaml file contains three services:

- `bacnet_server`
  - Image: `bacmod:1`
  - Functionality: Uses a modified version of the `bacserv` executable provided
    by the sourceforge BACnet Stack. The executable acts as a barebones BACnet
    server that responds to commands. The `WhoIs` command implementation has 
    been modifed to output a notification to another process over a 
    [named pipe](http://man7.org/linux/man-pages/man7/fifo.7.html).
- `listener`:
  - Image: `pipe_test:1`
  - Functionality: Listens on the other end of the named pipe. It keeps count of
    the notifications received and prints the total count for every new
    notification. The purpose of this behavior was to separate any extraneous
    functionality from the sourceforge BACnet source code.
    In other words, if you want to do something more intense than just printing 
    a message when the BACnet server receives a `WhoIs` command, then you can do 
    that independently of the sourceforge BACnet implementation. While the
    sourceforge BACnet source code is designed to allow for custom 
    implementation, it is more time/cost effective to code in any language 
    besides C for simple prototypes.
- `nodered`:
  - image: `nodered/node-red:latest` (provided from the DockerHub registry)
  - Functionality: To use node-red. Connect with `http://127.0.0.1:1880/`.
    Specifically, this can connect to the BACnet server. The `node-red-contrib-bacnet` palette can communicate with the server. 
    The server address is the name of the BACnet server service in `docker-compose.yaml`, which is `bacnet_server`.
    The BACnet device ID is `5678` (review the `./bacstack-modifed/Dockerfile` 
    for the correct ID; look for something that looks like
    `ENTRYPOINT ["/bin/bacserv", "5678"]`).

To use these services, simply run `docker-compose up`. If everything is correct,
all services shoulds start. The output of each service can be seen in real time.
To kill this process, press `ctrl + c`.
Note: as long as the node-red container created by docker-compose exists, you do
not have to reinstall node-red palettes everytime you run `docker-compose up`.
Otherwise, you do.

