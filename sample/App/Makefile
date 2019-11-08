ifndef KEYSTONE_SDK_DIR
export KEYSTONE_SDK_DIR = $(KEYSTONE_DIR)/sdk
endif

CC = riscv64-unknown-linux-gnu-gcc
CXX = riscv64-unknown-linux-gnu-g++
CFLAGS = -Wall
LINK = riscv64-unknown-linux-gnu-ld
AS = riscv64-unknown-linux-gnu-as

SDK_LIB_DIR = $(KEYSTONE_SDK_DIR)/lib
SDK_HOST_LIB = $(SDK_LIB_DIR)/libkeystone-host.a
SDK_EDGE_LIB = $(SDK_LIB_DIR)/libkeystone-edge.a
SDK_VERIFIER_LIB = $(SDK_LIB_DIR)/libkeystone-verifier.a

SDK_INCLUDE_HOST_DIR = $(SDK_LIB_DIR)/host/include
SDK_INCLUDE_EDGE_DIR = $(SDK_LIB_DIR)/edge/include
SDK_INCLUDE_VERIFIER_DIR = $(SDK_LIB_DIR)/verifier

EDGER_INClUDE_DIRS = $(EDGER_DIR)

ifneq (,$(wildcard $(EDGER_DIR)/bin/keyedge))
EDGER_INCLUDE_DIRS += $(EDGER_DIR)/target/include $(EDGER_DIR)/flatcc/include
EDGER_LDFLAGS = -L$(EDGER_DIR)/lib
EXTRA_LIBS = $(EDGER_DIR)/lib/flatccrt.a
endif

CCFLAGS += -I$(SDK_INCLUDE_HOST_DIR) -I$(SDK_INCLUDE_EDGE_DIR) -I$(SDK_INCLUDE_VERIFIER_DIR) $(addprefix -I,$(EDGER_INCLUDE_DIRS)) -I../Edge -I.
LDFLAGS = -L$(SDK_LIB_DIR) $(EDGER_LDFLAGS)

APP = app
OBJS = $(APP).o
EOBJS = $(EDGECALLS)_u.o

all: $(APP)

$(APP): $(OBJS) $(EOBJS) $(SDK_HOST_LIB) $(SDK_EDGE_LIB) $(SDK_VERIFIER_LIB) $(EXTRA_LIBS)
	$(CXX) $(CCFLAGS) $(LDFLAGS) -o $(APP) $^

$(OBJS): %.o: %.cpp $(EDGECALLS)_u.h
	$(CXX) $(CCFLAGS) -c $<

$(EOBJS): %.o: %.c $(EDGECALLS)_u.h
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(EOBJS) $(APP)

.PHONY: copyto

copyto:
	cp -p $(APP) $(KEYSTONE_DIR)/buildroot_overlay/root/edger-sample/
