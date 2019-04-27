# Hidden-tear
A ransomware written in C++, achieve the functions of hidden tear.

         _     _     _     _              _                  
        | |   (_)   | |   | |            | |                 
        | |__  _  __| | __| | ___ _ __   | |_ ___  __ _ _ __ 
        | '_ \| |/ _` |/ _` |/ _ \ '_ \  | __/ _ \/ _` | '__|
        | | | | | (_| | (_| |  __/ | | | | ||  __/ (_| | |   
        |_| |_|_|\__,_|\__,_|\___|_| |_|  \__\___|\__,_|_|   
                                                     

## Introduction

It's a ransomware-like file crypter sample which can be modified for specific purposes. It's more extended version of hidden tear.

## Features

* Uses both RSA and AES algorithms.
* Coordinates with a Command&Control server.
* Uses CSPRNG
* Uses phplibsec
* Encrypted files can be decrypted in decryption program with encryption key.
* Changes desktop background.

## Workflows

1. Program runs to get username/CpuHash/VolumeHash, then generates RSA keys and sends private key to the server while saving public key on PC;

2. Generate a random key for AES encrypt using; Run AES algorithm to encrypt target files;

3. Program encrypts aes key with RSA public key and send to server; Sever get the cipher and store it;

4. Delete private key and original files

## Config for development

### Server

The recommend environment is:

* Server version: Apache/2.4.33 (Unix)
* thinkPHP 5.0.24 with PHP 7.3.3 (cli) 
* Server version: 5.7.20 MySQL Community Server (GPL)

### Pc program

Two necessary C++ library used in program:

* boost 1.67.0_1
* cryptopp/8.1.0

If you want to compile or even deveplop the program, you must install then.

If you just want to test or just watch the visualization of workflow, you can just download and use executable files.

## usage

* The hole project of PC is in ./src, just compile the encrypt.cpp and decrypt.cpp to get exe;
* The hole project of server is in ./server, just do like the readme.md in server do;
* To test the aes algorothm, you can use the code in ./test;
* To test the project in pc, you can use the code in ./demo

## Legal Warning

While this may be helpful for some, there are significant risks. Hidden tear may be used only for Educational Purposes. Do not use it as a ransomware! You could go to jail on obstruction of justice charges just for running Hidden tear, even though you are innocent.
