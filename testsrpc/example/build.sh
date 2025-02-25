#!/bin/bash
g++ -o server server.cc example.pb.cc -std=c++11 -lsrpc -lprotobuf -lgtest -lworkflow
g++ -o client client.cc example.pb.cc -std=c++11 -lsrpc -lprotobuf -lgtest -lworkflow
