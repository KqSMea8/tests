#!/bin/bash
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${RED}test linked with mkl${NC}"
./mkl_exe

echo -e "${RED}test linked with mklml${NC}"
./mklml_exe
