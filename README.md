**dtlsproxy**
----------
Lightweight DTLS reverse proxy server based on [tinydtls](https://projects.eclipse.org/projects/iot.tinydtls) and [libev](https://github.com/enki/libev).

Similar with [goldy](https://github.com/ibm-security-innovation/goldy), but using [tinydtls](https://projects.eclipse.org/projects/iot.tinydtls) instead of [mbed TLS](https://tls.mbed.org/), using pre-shared key (PSK) instead of certificate/private key files.

```shell
usage: dtlsproxy -l <host:port> -b <hosts:ports> -k <key maps>
        -l listen    listen on specified host and port
        -b backends  backend servers (host1:port1,host2:port2,...)
        -k keys      psk identities (id1:key1,id2:key2,...)
```
sample usage:
```shell
    $ dtlsproxy -l 0.0.0.0:5684 -b 127.0.0.1:15683 -k Client_identity:secretPSK
```
Tested on Cygwin and Ubuntu hosts with a [libcoap](https://github.com/obgm/libcoap) server (on port 15683) as the backend.

**Docker use**
---

To build and push the Docker image on specific source tag:

    TAG=v0.2 make docker-build

    or

    TAG=v0.2 make docker-push

To build the Docker image:

	docker build . -t dtlsproxy:latest

The Docker image has an entrypoint script which makes easy to pass the PSK keys, without requiring the use of a volume. To do so, run:

    docker run -e LISTEN=<host:port> \
               -e BACKEND=<host1:port1,host2:port2,..> \
               -e KEYS=<base64(id1:key1,id2:key2,...)> \
               <extra dtlsproxy arguments>

In general, proxy will listen on port 5684 for DTLS packets.