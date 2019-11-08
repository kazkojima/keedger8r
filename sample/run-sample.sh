#!/bin/bash

expect -c '
    set timeout 60
    cd $::env(KEYSTONE_DIR)
    spawn ./scripts/run-qemu.sh
    expect "*?ogin" { send "root\r" }
    expect "*?assword" { send "sifive\r" }

    expect "# " { send "insmod keystone-driver.ko\r" }

    expect "# " { send "cd edger-sample\r" }
    expect "# " { send "./app  enclave.eapp_riscv eyrie-rt\r" }

    expect "# " { send "poweroff\r" }
    expect eof
    '
