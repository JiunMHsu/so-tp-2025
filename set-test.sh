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
}

# Ensure at least one argument is provided
if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <command> [subcommand] [third_arg]"
  exit 1
fi

main_cmd="$1"
sub_cmd="$2"
third_arg="$3"

# clean_configs

case "$main_cmd" in
  plani)
    case "$sub_cmd" in
      corto)
        case "$third_arg" in
          fifo)
            echo "Corto plazo con algoritmo FIFO"
            ;;
          sjf)
            echo "Corto plazo con algoritmo SJF"
            ;;
          srt)
            echo "Corto plazo con algoritmo SRT"
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
            echo "Largo/Mediano plazo con algoritmo FIFO"
            ;;
          pmcp)
            echo "Largo/Mediano plazo con algoritmo PMCP"
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
    echo "Ejecutando lógica de swap..."
    ;;

  cache)
    case "$sub_cmd" in
      clock)
        echo "Cache con algoritmo CLOCK"
        ;;
      clockm)
        echo "Cache con algoritmo CLOCK-M"
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
        echo "TLB con algoritmo FIFO"
        ;;
      lru)
        echo "TLB con algoritmo LRU"
        ;;
      *)
        echo "algoritmo TLB desconocido: $sub_cmd"
        exit 1
        ;;
    esac
    ;;

  estabilidad)
    echo "Analizando estabilidad..."
    ;;

  *)
    echo "comando desconocido: $main_cmd"
    exit 1
    ;;
esac