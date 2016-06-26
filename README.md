CRY(T_T)
========

A small crypto library.


Multiple precision integers
---------------------------

- Basic arithmentic (add,sub,mul,div,mod,abs,exp,sqr)
- Modular exponentiation
- Modular inverse (Euclidean)
- GCD and LCM
- Probabilistic prime numbers generator (Miller-Rabin)
- Random mpi generator


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

- Diffie-Hellman


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

### Checksums

- LRC
- 8-bit trivial checksum


Utilities
---------

- Base64 encoder/decoder

