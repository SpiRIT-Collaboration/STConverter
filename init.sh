#!/bin/bash

git submodule init
git submodule update
cd Source/GETDecoder
git pull origin master
