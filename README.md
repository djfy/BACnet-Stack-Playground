# Simple compose files
The simple compose files (`simple-` directory prefix) use a BACnet server 
docker image from [fh1ch/bacstack-compliance-docker](https://hub.docker.com/r/fh1ch/bacstack-compliance-docker).

Run:
```docker-compose up```
in any `simple-` directory to start the services.

# Scaleable compose files
The scaleable compose files (`scaleable-` directory prefix) allow the BACnet 
server service to scale with unique* IDs. It requires a special image 
built from the `bacstack-with-rand.dockerfile` provided somewhere in this 
repository.

*<sup>The random number generation method does not guarantee all server
instances will have the same ID, just that they probably will.</sup>

To run multiple BACnet server services, run:
```docker-compose up --scale bacnet_server=<n>``` 
where `<n>` equal the number of unique instances of the BACnet server you want.
For example:
```docker-compose up --scale bacnet_server=5```

# Connecting to Node-Red service
In your browser, navigate to the URL:
`http://127.0.0.1:1880/`
All of the docker-compose files should create a docker volume 
named `nodered-data` and associate the node-red `/data` directory with it.
Therefore, any installed palettes or other server data is persistent between
sessions.

# Connecting to Wireshark service
Run wireshark in browser.
Connect to:
```https://127.0.0.1:14500/?username=wireshark&password=wireshark```
Or:
```https://127.0.0.1:14500```
with username `wireshark` and password `wireshark`.
Your browser may try to protect your from going to that link, feel free to 
ignore it.

Connect to `eth0` to access the network traffic. There will be traffic from all
services, so filter as needed. 

Display Filters for BACnet:
```bacnet || bvlc || bacapp1```

Capture filters for BACnet:
```udp port 47808```
