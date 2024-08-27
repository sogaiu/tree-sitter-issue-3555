#! /bin/sh

gcc \
  -Itree-sitter/lib/include -Itree-sitter/lib/src tree-sitter/lib/src/lib.c \
  -Itree-sitter-vhdl/src tree-sitter-vhdl/src/parser.c \
  main.c
