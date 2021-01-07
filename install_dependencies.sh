#!/bin/bash

EXTERNAL_FOLDER=./src/external
GITHUB_URL=https://raw.githubusercontent.com

mkdir -p $EXTERNAL_FOLDER

curl $GITHUB_URL/HowardHinnant/date/master/include/date/date.h -o $EXTERNAL_FOLDER/date.h
curl $GITHUB_URL/Jamesbarford/datum-type-parser/main/datum_parser.hpp -o $EXTERNAL_FOLDER/datum_parser.hpp
curl $GITHUB_URL/Jamesbarford/datum-type-parser/main/datum_parser.cpp -o $EXTERNAL_FOLDER/datum_parser.cpp
