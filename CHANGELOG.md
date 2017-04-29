CRY Library ChangeLog
=====================

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com).

Type of changes

- Added: for new features.
- Changed: for changes in existing functionality.
- Deprecated: for soon-to-be removed features.
- Removed: for now removed features.
- Fixed: for any bug fixes.
- Security: in case of vulnerabilities.

This project adheres to [Semantic Versioning](http://semver.org).

Given a version number MAJOR.MINOR.PATCH
- MAJOR incremented for incompatible API changes
- MINOR incremented for new functionalities
- PATCH incremented for bug fixes

[Unreleased]
------------

- ECP core arithmetic (add,dbl,mul)
- Load NIST-P256 EC parameters
- DSA end ECDSA digital signature
- ECP multiplication with sliding window algorithm

[0.0.5] - 2016-10-02
--------------------

- Diffie-Hellman secret exchange
- OS agnostic CSPRNG with AES-CTR
- CRC32 Ethernet

[0.0.4] - 2016-05-07
--------------------

- MPI Comba multiplier
- MPI Karatsuba multiplier
- MPI fast division
- MPI square algorithm
- HMAC with generic hash function

[0.0.3] - 2015-11-28
--------------------

- MPI abs, shl/shr, gcd, lcm, exp, mod-exp
- MPI load/store from string and binary buffer
- MPI inverse (Euclidean algorithm)
- Unit test framework
- Improved build system using non recursive make
- CSPRNG wrapper around OS specific PRNG.
- RSA (PKCS#1 v1.5)

[0.0.2] - 2014-11-22
--------------------

### Added

- LRC
- 8-bit trivial checksum
- CRC16-IBM
- MPI add, sub, mul, div
- MPI print

[0.0.1] - 2014-05-10
--------------------

### Added

- AES block cipher
- DES block cipher
- CBC block cipher mode of operation
- GCM block cipher mode of operation
- CTR block cipher mode of operation
- MD5 message digest
- SHA256 message digest
- CMAC message authentication code
- Base64 encoder/decoder
- CRC16-CCITT

