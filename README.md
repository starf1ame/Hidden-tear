# Hidden-tear
A ransomware written in C++, achieve the functions of hidden tear.

## Workflow

* Program sends a POST request to the C&C server with pc_name and user_name variables.
* C&C server creates RSA public/private key pair. Sends public key to the program, saves private key inside the Mysql database
* Program creates a random key for AES algorithm
* Program encrypts files with AES algorithm
* Program encrypts AES key with RSA public key and sends it to the C&C server with POST request
* C&C server saves encrypted AES key inside the Mysql Database

## TODO list

1. Local program to encrpt files with AES algorithm
2. Build a C&C server, make interaction (local program sends post to server)
3. Achieve creating RSA keys and interact with Mysql database

## goal

At last, we can achieve functions like: https://www.youtube.com/watch?v=PD16u1Rz2QI
