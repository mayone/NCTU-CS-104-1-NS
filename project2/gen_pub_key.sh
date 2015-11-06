#!/bin/bash
#Generate public key from private key
openssl rsa -in private.pem -out public.pem -outform PEM -pubout