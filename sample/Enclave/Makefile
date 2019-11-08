ifndef KEYSTONE_SDK_DIR
export KEYSTONE_SDK_DIR = $(KEYSTONE_DIR)/sdk
endif

CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -Wall -fno-builtin-printf -DEDGE_IGNORE_EGDE_RESULT
LINK = riscv64-unknown-linux-gnu-ld
AS = riscv64-unknown-linux-gnu-as

SDK_LIB_DIR = $(KEYSTONE_SDK_DIR)/lib
SDK_ENCLAVE_LIB = $(SDK_LIB_DIR)/libkeystone-eapp.a
SDK_EDGE_LIB = $(SDK_LIB_DIR)/libkeystone-edge.a
SDK_INCLUDE_ENCLAVE_DIR = $(SDK_LIB_DIR)/app/include
SDK_INCLUDE_EDGE_DIR = $(SDK_LIB_DIR)/edge/include

EDGER_INClUDE_DIRS = $(EDGER_DIR)

ifneq (,$(wildcard $(EDGER_DIR)/bin/keyedge))
EDGER_INCLUDE_DIRS += $(EDGER_DIR)/target/include $(EDGER_DIR)/flatcc/include
EDGER_LDFLAGS = -L$(EDGER_DIR)/lib
EXTRA_LIBS = $(EDGER_DIR)/lib/flatccrt.a
endif

CFLAGS += -I$(SDK_INCLUDE_ENCLAVE_DIR) -I$(SDK_INCLUDE_EDGE_DIR)  $(addprefix -I,$(EDGER_INCLUDE_DIRS)) -I../Edge -I.
LDFLAGS = -static -L$(SDK_LIB_DIR)  $(EDGER_LDFLAGS)

ENCLAVE = enclave
OBJS = $(ENCLAVE).o
EOBJS = $(EDGECALLS)_t.o
ENCLAVE_LDS ?= ./Enclave.lds

ENCLAVE_BIN = $(patsubst %,%.eapp_riscv,$(ENCLAVE))

all: $(ENCLAVE_BIN)

%.o: %.c $(EDGECALLS)_t.h
	$(CC) $(CFLAGS) -c $<

$(ENCLAVE_BIN): %.eapp_riscv : $(OBJS) $(EOBJS) $(SDK_ENCLAVE_LIB) $(SDK_EDGE_LIB) $(EXTRA_LIBS)
	$(LINK) $(LDFLAGS) -o $@ $^ -T $(ENCLAVE_LDS)
	chmod -x $@

clean:
	rm -rf $(ENCLAVE_BIN) $(OBJS) $(EOBJS)

.PHONY: copyto

copyto:
	cp -p $(ENCLAVE_BIN) $(KEYSTONE_DIR)/buildroot_overlay/root/edger-sample/
