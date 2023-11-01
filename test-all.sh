#! /bin/bash

unset ONEAPI_DEVICE_SELECTOR
export I_MPI_OFFLOAD=1

pass=()
fail=()

for node in 01 02 03 04 05 06 08 09 10 11 12 13 14 15 16 17 18 19 21
do
    name=idc-beta-batch-pvc-node-${node}
    command="srun -w $name --pty mpirun -n 2 --launcher fork ./put-get device"
    echo $command
    $command
    if (($?)); then
        echo Fail
        fail+=($name)
    else
        echo Pass
        pass+=($name)
    fi
    echo
done

echo Pass: ${pass[*]}
echo Fail: ${fail[*]}
