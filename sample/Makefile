ifndef KEYSTONE_DIR
$(error KEYSTONE_DIR is not set)
endif
ifndef EDGER_DIR
$(error EDGER_DIR is not set)
endif

EDGER=

ifneq (,$(wildcard $(EDGER_DIR)/bin/keyedge))
	EDGER=$(EDGER_DIR)/bin/keyedge
	EDGER_TYPE=KEYEDGE
else ifneq (,$(wildcard $(EDGER_DIR)/keedger8r))
	EDGER=$(EDGER_DIR)/keedger8r
	EDGER_TYPE=KEEDGER8R
else
	$(error no available edger found)
endif

EDGE_DIR=Edge
ENCLAVE_DIR=Enclave
APP_DIR=App

.PHONY: all gen app enclave clean cleangen

all: gen app enclave
	@echo $(EDGER_TYPE)

EDGECALLS=Enclave

ifeq ($(EDGER_TYPE), KEEDGER8R)
EDGE_FILE=$(EDGECALLS).edl

GENFILES=$(ENCLAVE_DIR)/$(EDGECALLS)_t.c $(ENCLAVE_DIR)/$(EDGECALLS)_t.h \
	$(ENCLAVE_DIR)/$(EDGECALLS)_args.h \
	$(APP_DIR)/$(EDGECALLS)_u.c $(APP_DIR)/$(EDGECALLS)_u.h \
	$(APP_DIR)/$(EDGECALLS)_args.h \
	$(EDGE_DIR)/edger_macros.h

gen:
	(cd $(APP_DIR); $(EDGER) --untrusted ../$(EDGE_DIR)/$(EDGE_FILE))
	(cd $(ENCLAVE_DIR); $(EDGER) --trusted ../$(EDGE_DIR)/$(EDGE_FILE))
	cp $(EDGER_DIR)/edger_macros.h $(EDGE_DIR)

else ifeq ($(EDGER_TYPE), KEYEDGE)

EDGE_FILE=ocalls.h
EDGE_BASE=$(basename $(EDGE_FILE))
ENC_T_C="\#include \"common_header.h\"\n\#include \"ocalls_eapp.h\"\n"
ENC_T_H="\#include \"ocalls_header.h\"\n\#include \"ocalls.h\"\n"
ENC_U_C="// generated empty file\n"
ENC_U_H="\#include \"ocalls_host.h\"\n\#define EDGE_EXTERNC_BEGIN\n\#define EDGE_EXTERNC_END\n"

GENFILES=$(ENCLAVE_DIR)/$(EDGECALLS)_t.c $(ENCLAVE_DIR)/$(EDGECALLS)_t.h \
	$(APP_DIR)/$(EDGECALLS)_u.c $(APP_DIR)/$(EDGECALLS)_u.h \
	$(EDGE_DIR)/$(EDGE_BASE).fbs \
	$(EDGE_DIR)/$(EDGE_BASE)_*.h \
	$(EDGE_DIR)/flatbuffers_common_builder.h \
	$(EDGE_DIR)/flatbuffers_common_reader.h

gen:
	export KEYEDGE_DIR=$EDGER_DIR
	(cd $(EDGE_DIR); $(EDGER) $(EDGE_FILE))
	(cd $(EDGE_DIR); $(EDGER_DIR)/flatcc/bin/flatcc -a $(EDGE_FILE:.h=.fbs))
	echo $(ENC_T_C) > $(ENCLAVE_DIR)/$(EDGECALLS)_t.c
	echo $(ENC_T_H) > $(ENCLAVE_DIR)/$(EDGECALLS)_t.h
	echo $(ENC_U_C) > $(APP_DIR)/$(EDGECALLS)_u.c
	echo $(ENC_U_H) > $(APP_DIR)/$(EDGECALLS)_u.h

endif

app:
	make -C $(APP_DIR) KEYSTONE_DIR=$(KEYSTONE_DIR) EDGER_DIR=$(EDGER_DIR) EDGECALLS=$(EDGECALLS) 

enclave:
	make -C $(ENCLAVE_DIR) KEYSTONE_DIR=$(KEYSTONE_DIR) EDGER_DIR=$(EDGER_DIR) EDGECALLS=$(EDGECALLS)

cleangen:
	rm -rf $(GENFILES)

clean:
	make -C $(APP_DIR) clean EDGECALLS=$(EDGECALLS)
	make -C $(ENCLAVE_DIR) clean EDGECALLS=$(EDGECALLS)
	rm -rf $(GENFILES)

.PHONY: copyto run

copyto:
	mkdir -p $(KEYSTONE_DIR)/buildroot_overlay/root/edger-sample
	make -C $(APP_DIR) copyto KEYSTONE_DIR=$(KEYSTONE_DIR)
	make -C $(ENCLAVE_DIR) copyto KEYSTONE_DIR=$(KEYSTONE_DIR)
	cp -p $(KEYSTONE_DIR)/sdk/rts/eyrie/eyrie-rt $(KEYSTONE_DIR)/buildroot_overlay/root/edger-sample/
	make -C $(KEYSTONE_DIR) image

run:
	./run-sample.sh
