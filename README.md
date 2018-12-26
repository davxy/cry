[![Build Status](https://travis-ci.com/crylib/cry.svg?branch=master)](https://travis-ci.com/crylib/cry)
[![Coverage Status](https://coveralls.io/repos/github/crylib/cry/badge.svg?branch=master)](https://coveralls.io/github/crylib/cry?branch=master)


CRY
===

CRY is a small, comprehensive and portable cryptographic library prividing
a good collection of ciphers, hash functions, pseudo-random number generators
and multi-precision integers algorithms.


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

- AES (Rijndael)
- DES and Triple DES

### Block ciphers mode of operation

- ECB
- CBC
- GCM
- CTR

### Stream ciphers

- Trivium


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
- NIST-P256 EC parameters


Pseudo random numbers generators
--------------------------------

- AES-CTR CSPRNG
- Posix CSPRNG (using '/dev/urandom')
- Windows CSPRNG (using 'CryptGenRandom')
- Weak portable PRNG

Message authentication code
---------------------------

- CMAC
- HMAC


Hash
----

### Secure hash

- MD5
- SHA-256

### CRC

- CRC16-CCITT
- CRC16-IBM
- CRC32-Ethernet

### Checksums

- LRC
- Trivial checksum


Classical ciphers
-----------------

### Substitution ciphers

- Hill cipher
- Polyalphabetic Affine cipher


Utilities
---------

- Base64 encoder/decoder
- Memxor
