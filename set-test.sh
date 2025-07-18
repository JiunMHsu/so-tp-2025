#!/bin/bash

clean_configs() {
  cd ./kernel/
  rm -rf *.config

  cd ../cpu/
  rm -rf *.config

  cd ../memoria/
  rm -rf *.config

  cd ../io
  rm -rf *.config

  cd ..
}

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <command> [subcommand] [third_arg]"
  exit 1
fi

main_cmd="$1"
sub_cmd="$2"
third_arg="$3"

clean_configs

cp ./config/io.config ./io/io.config

case "$main_cmd" in
  plani)
    case "$sub_cmd" in
      corto)
        case "$third_arg" in
          fifo)
            echo "configurando para plani corto FIFO"
            cp ./config/plani_corto/memoria.config ./memoria/memoria.config
            cp ./config/plani_corto/kernel_fifo.config ./kernel/kernel.config
            cp ./config/plani_corto/cpu.config ./cpu/cpu1.config
            cp ./config/plani_corto/cpu.config ./cpu/cpu2.config
            ;;
          sjf)
            echo "configurando para plani corto SJF"
            cp ./config/plani_corto/memoria.config ./memoria/memoria.config
            cp ./config/plani_corto/kernel_sjf.config ./kernel/kernel.config
            cp ./config/plani_corto/cpu.config ./cpu/cpu1.config
            ;;
          srt)
            echo "configurando para plani corto SRT"
            cp ./config/plani_corto/memoria.config ./memoria/memoria.config
            cp ./config/plani_corto/kernel_srt.config ./kernel/kernel.config
            cp ./config/plani_corto/cpu.config ./cpu/cpu1.config
            ;;
          *)
            echo "algoritmo desconocido para plani corto: $third_arg"
            exit 1
            ;;
        esac
        ;;
      lym)
        case "$third_arg" in
          fifo)
            echo "configurando para plani largo/mediano plazo FIFO"
            cp ./config/plani_lym/memoria.config ./memoria/memoria.config
            cp ./config/plani_lym/kernel_fifo.config ./kernel/kernel.config
            cp ./config/plani_lym/cpu.config ./cpu/cpu1.config
            ;;
          pmcp)
            echo "configurando para plani largo/mediano plazo PMCP"
            cp ./config/plani_lym/memoria.config ./memoria/memoria.config
            cp ./config/plani_lym/kernel_pmcp.config ./kernel/kernel.config
            cp ./config/plani_lym/cpu.config ./cpu/cpu1.config
            ;;
          *)
            echo "algoritmo desconocido para plani lym: $third_arg"
            exit 1
            ;;
        esac
        ;;
      *)
        echo "tipo plani desconocido: $sub_cmd"
        exit 1
        ;;
    esac
    ;;

  swap)
    echo "configurando para swap"
    cp ./config/swap/memoria.config ./memoria/memoria.config
    cp ./config/swap/kernel.config ./kernel/kernel.config
    cp ./config/swap/cpu.config ./cpu/cpu1.config
    ;;

  cache)
    case "$sub_cmd" in
      clock)
        echo "configurando para cache CLOCK"
        cp ./config/cache/memoria.config ./memoria/memoria.config
        cp ./config/cache/kernel.config ./kernel/kernel.config
        cp ./config/cache/cpu_clock.config ./cpu/cpu1.config
        ;;
      clockm)
        echo "configurando para cache CLOCK-M"
        cp ./config/cache/memoria.config ./memoria/memoria.config
        cp ./config/cache/kernel.config ./kernel/kernel.config
        cp ./config/cache/cpu_clockm.config ./cpu/cpu1.config
        ;;
      *)
        echo "algoritmo cache desconocido: $sub_cmd"
        exit 1
        ;;
    esac
    ;;

  tlb)
    case "$sub_cmd" in
      fifo)
        echo "configurando para TLB FIFO"
        cp ./config/tlb/memoria.config ./memoria/memoria.config
        cp ./config/tlb/kernel.config ./kernel/kernel.config
        cp ./config/tlb/cpu_fifo.config ./cpu/cpu1.config
        ;;
      lru)
        echo "configurando para TLB LRU"
        cp ./config/tlb/memoria.config ./memoria/memoria.config
        cp ./config/tlb/kernel.config ./kernel/kernel.config
        cp ./config/tlb/cpu_lru.config ./cpu/cpu1.config
        ;;
      *)
        echo "algoritmo TLB desconocido: $sub_cmd"
        exit 1
        ;;
    esac
    ;;

  estabilidad)
    echo "configurando para estabilidad general"
    cp ./config/estabilidad_general/memoria.config ./memoria/memoria.config
    cp ./config/estabilidad_general/kernel.config ./kernel/kernel.config
    cp ./config/estabilidad_general/cpu1.config ./cpu/cpu1.config
    cp ./config/estabilidad_general/cpu2.config ./cpu/cpu2.config
    cp ./config/estabilidad_general/cpu3.config ./cpu/cpu3.config
    cp ./config/estabilidad_general/cpu4.config ./cpu/cpu4.config
    ;;

  *)
    echo "comando desconocido: $main_cmd"
    exit 1
    ;;
esac

echo "configuraciones completadas para $main_cmd $sub_cmd $third_arg"
