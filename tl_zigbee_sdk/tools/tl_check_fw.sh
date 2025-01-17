#!/bin/bash 
echo "*****************************************************"
echo "this is post build!! current configure is :$1"

if [[ $2 = tc32 ]]; then
	script_dir=../../../tools
elif [[ $2 = riscv ]]; then
	script_dir=$(dirname $(realpath "$0"))
elif [[ $2 = iot_riscv ]]; then
	script_dir=$(dirname $(realpath "$0"))
fi

if [[ "$OSTYPE" = "linux-gnu"* ]]; then
    echo "Current system is Linux"
	tool=${script_dir}/tl_check_fw2.out
elif [[ "$OSTYPE" = "darwin"* ]]; then
    echo "Current system is macOS"
	echo "Not support"
	exit
elif [[ "$OSTYPE" = "msys"* || "$OSTYPE" == "cygwin"* ]]; then
    echo "Current system is Windows"
	tool=${script_dir}/tl_check_fw2.exe
else
    echo "Unknown operating system: $OSTYPE"
	exit
fi

if [[ $2 = tc32 ]]; then
	tc32-elf-objcopy -v -O binary $1.elf $1.bin
	${tool} $1.bin
elif [[ $2 = riscv ]]; then
	riscv32-elf-objcopy -S -O binary $1.elf output/$1.bin
	${tool} output/$1.bin
elif [[ $2 = iot_riscv ]]; then
	riscv32-elf-objcopy -S -O binary $1.elf $1.bin
	${tool} $1.bin
fi

echo "**************** end of post build ******************"
