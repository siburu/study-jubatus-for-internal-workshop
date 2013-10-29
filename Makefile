.PHONY: all
all: update analyze

update: update.cpp
	g++ -O2 -o $@ $< `pkg-config jubatus --libs --cflags` `pkg-config pficommon --libs --cflags` -lmsgpack -ljubatus_mpio -ljubatus_msgpack-rpc -lboost_filesystem -lboost_system

analyze: analyze.cpp
	g++ -O2 -o $@ $< `pkg-config jubatus --libs --cflags` `pkg-config pficommon --libs --cflags` -lmsgpack -ljubatus_mpio -ljubatus_msgpack-rpc -lboost_filesystem -lboost_system
