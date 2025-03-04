# SG-Attacks

## GOOSE Fuzzer

`sudo docker run -it --name goose-fuzzer --network ieee14_transportNetwork --ip 10.0.99.99 -e N_BATCHES=1000 javiersande/goose-fuzzer:latest`

## GOOSE Packet Injector

`sudo docker run -it --name goose-packet-injector --network ieee14_transportNetwork --ip 10.0.99.99 -e N_BATCHES=1000 javiersande/goose-packet-injector:latest`

## MSS Scanner

`sudo docker run -it --name mss-scanner --network ieee14_transportNetwork --ip 10.0.99.99 javiersande/mss-scanner:latest`