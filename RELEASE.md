# Release

## Tests
1. Bump firmware version, path: `src/inc/report_protocol.h:24`, e.g.
   ```
   #define FIRMWARE_VERSION (0x0D)
   ```
2. Tag release version:
    - the used commit ideally is on the default branch - either `main` or `master`, but not required;
    - tag name should note this is a pre-release version, and currently under tests; 
    - tag name should be in format `v0.xx-RCy`, e.g. `v0.13-RC1`;
    - tag should be annotated and signed;
    - tag message should describe major changes only, in a single sentence (whenever possible).
3. Run tests from the test list
4. Attach test reports

### Test list

1. libnitrokey - test_pro.py
1. Brief manual Nitrokey App test
2. GNUK tests for OpenPGP card -> NK Start /tests
3. Firmware update tests:
   - from A -> B
   - from B -> B
   
   where:
     - A is the current latest stable firmware
     - B is the firmware under test


The test for the firmware update is the most important one, as without the ability to update the firmware in the field there will be no possibility to correct other firmware issues.

## Release
1. Bump firmware version (if not done already during tests), path: `src/inc/report_protocol.h:24`, e.g. 
   ```
   #define FIRMWARE_VERSION (0x0D)
   ```
2. Tag release version: 
    - the used commit must be on the default branch - either `main` or `master`;
    - make sure the used commit for tagging is signed;
    - tag name should be in format `v0.xx`, e.g. `v0.13`;
    - tag should be annotated and signed;
    - tag message should describe major changes only, in a single sentence (whenever possible);
    - the same commit can be used for the RC and final release tags.
3. Push tag to the repository
4. Run build and publish binaries on the Github
5. Create change log notes (e.g. through `github-changelog-generator`) and publish them under Github tag

## Build

Run `make release` to prepare binaries using the default `arm-none-eabi-gcc` installed compiler. This will result in:
   - binaries are in the `release/` directory, and packed as well to `nitrokey-pro-firmware-_VERSION_.tar.gz`
   - binaries are unsigned unless `SIGN=1` argument is provided - this will make the signature using the  user's default GnuPG key;
   - file names are named after current Git version of the repository, but can be overriden with `VERSION=v0.xx` argument;
   - Git repository version is not stored in the firmware file, but only saved in the "buildinfo" file.

Compiler can be changed by supplying the `CC=` argument.

The "buildinfo" file contains:
- compiler name and version
- actually used Git repository version
- used build flags

Full call:
```bash
make release SIGN=1 VERSION=v0.13
```