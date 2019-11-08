A tiny sample for keystone edgers
=================================

The purpose of this tiny sample is to give an example which builds keystone enclave application with edgers.

The sample tries to absorb the difference of edgers as far as possible. The same host and enclave codes are used with two edgers keyedge and keedger8r. This would make migration from keedger8r to keyedge easy.

## build and test

Two environment variables KEYSTONE_DIR and EDGER_DIR are required to be set.
```
export KEYSTONE_DIR=path_to_keystone_enclave_source_directory
export EDGER_DIR=path_to_keyedge_or_keedger8r_source_directory
```
Then
```
make
```
will build the sample.  You can import the sample binaries to keystone root fs with
```
make copyto
```
and run the sample with
```
make run
```

## source tree

```
.
├── App
│   ├── Makefile
│   └── app.cpp
├── Edge
│   ├── Enclave.edl
│   └── ocalls.h
├── Enclave
│   ├── Enclave.lds
│   ├── Makefile
│   └── enclave.c
├── Makefile
└── run-sample.sh
```

Only Enclave.edl and ocalls.h are specific to each edger.


