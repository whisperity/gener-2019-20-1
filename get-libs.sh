#!/bin/bash

if [ ! -d "./boost" ]
then
    git clone http://github.com/boostorg/boost \
        --origin upstream \
        --recursive
        --jobs $(nproc)

    pushd ./boost
    git checkout boost-1.71.0
    git submodule foreach --recursive "git checkout boost-1.71.0"

    ./bootstrap.sh
    ./b2 headers
    popd
fi

if [ ! -d "./include" ]
then
    mkdir include
fi

if [ ! -e "./include/boost" ]
then
    pushd include
    ln -s ../boost/boost boost
    popd
fi
