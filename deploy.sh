#!/bin/bash

rm -rf /home/utnso/scripts/
rm -rf /home/utnso/swapfile.bin
rm -rf /home/utnso/dump_files/

git clone https://github.com/sisoputnfrba/revenge-of-the-cth-pruebas.git /home/utnso/scripts
touch /home/utnso/swapfile.bin
mkdir /home/utnso/dump_files

change_config() {
    KEY=${1:?}
    VALUE=${2:?}

    grep -Rl "^\s*$KEY\s*=" \
        | grep -E '\.config|\.cfg' \
        | tee >(xargs -n1 sed -i "s|^\($KEY\s*=\).*|\1$VALUE|")
}

KERNEL="127.0.0.1"
MEMORIA="127.0.0.1"

for arg in "$@"; do
  case $arg in
    --kernel=*)
      KERNEL="${arg#*=}"
      ;;
    --memoria=*)
      MEMORIA="${arg#*=}"
      ;;
    *)
      echo "Unknown argument: $arg"
      exit 1
      ;;
  esac
done

change_config "IP_KERNEL" "$KERNEL"
change_config "IP_MEMORIA" "$MEMORIA"

bash makeall.sh

echo ""
echo "Kernel IP: $KERNEL"
echo "Memoria IP: $MEMORIA"
