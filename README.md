[![Build Status](https://travis-ci.com/crylib/cry.svg?branch=master)](https://travis-ci.com/crylib/cry)
[![Coverage Status](https://coveralls.io/repos/github/crylib/cry/badge.svg?branch=master)](https://coveralls.io/github/crylib/cry?branch=master)

CRY
===

CRY is a small, comprehensive and portable cryptographic library prividing
a good collection of ciphers, hash functions, chaining modes, pseudo-random
number generators and checksums.


Multiple precision integers
---------------------------

- Basic arithmentic (add,sub,mul,div,mod,abs,exp,sqr)
- Modular exponentiation
- Modular inverse (Euclidean)
- GCD and LCM
- Probabilistic prime numbers generator (Miller-Rabin)
- Random mpi generator
- Comba, Karatsuba and Toom-Cook-3 multipliers


Symmetric ciphers
-----------------

### Block ciphers

- AES
- DES and Triple DES

### Block cipher mode of operation

- ECB
- CBC
- GCM
- CTR


Public key algorithms
---------------------

### Ciphers

- RSA (PKCS#1 v1.5)

### Secret exchange

- Diffie-Hellman (DH)
- ECDH

### Digital signature

- RSA (PKCS#1 v1.5)
- DSA
- ECDSA


Elliptic Curve
--------------

- Basic arithmetic (add,dbl,mul)
- Load of NIST-P256 EC parameters


Pseudo random numbers generators
--------------------------------

- OS agnostic CSPRNG with AES-CTR
- Weak portable PRNG
- Posix CSPRNG (using '/dev/urandom')
- Windows CSPRNG (using 'CryptGenRandom')

Message authentication code
---------------------------

- CMAC
- HMAC


Hash
----

### Cryptographic hash

- MD5
- SHA-256

### CRC

- CRC16-CCITT
- CRC16-IBM
- CRC32-Ethernet

### Checksums

- LRC
- 8-bit trivial checksum


Classical ciphers
-----------------

### Substitution ciphers

- Hill cipher
- Polyalphabetic Affine cipher


Utilities
---------

- Base64 encoder/decoder

