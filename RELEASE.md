# Release Guide
## Pre-release (RC) and tests
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
Firstly the automated tests should be executed, and after passing them the manual ones.

1. libnitrokey: 
   - `test_pro.py` 
   - `test_pro_bootloader.py` 
   - `test_issues.py` if needed
2. GNUK tests for OpenPGP card -> NK Start's `/tests`
3. Firmware update tests:
   - prepare device 
     - populate device (e.g. by running tests)
   - from S -> D (check if user's data are retained)
     - test device, check for the retained data
   - from D -> D (check upgrade possibility for the future releases)
   - from D -> S (check rollback possibility)
     - test device, check for the retained data
  
   where:
     - S stands for the current latest **S**table firmware
     - D is the firmware under test and **D**evelopment
4. Brief manual Nitrokey App test
5. Other manual tests according to the test sheet


The test for the firmware update is the most important one, as without the ability to update the firmware in the field there will be no possibility to correct other firmware issues.

## Final Release
1. Bump the firmware version (if not done already during tests), path: `src/inc/report_protocol.h:24`, excerpt: 
   ```
   #define FIRMWARE_VERSION (0x0D)
   ```
2. Tag release version: 
    - the used commit must be:
      - on the default branch - either `main` or `master`;
      - GPG signed;
    - make sure the used commit for tagging is signed;
    - tag name should be in format `v0.xx`, e.g. `v0.13`;
    - tag should be annotated and signed;
    - tag message should describe major changes only, in a single sentence (whenever possible);
    - the same commit can be used for the RC and final release tags.
3. Push tag to the repository
4. Run build and publish binaries on the Github 
   - use CI where available;
   - if manual execution is needed, run release routines (create them if needed);
5. Create change log notes (e.g. through `github-changelog-generator`) and publish them under Github and proper tag

## Build

Run `make release` to prepare binaries using the default `arm-none-eabi-gcc` installed compiler. This will result in:
   - binaries are in the `release/` directory, and packed as well to `nitrokey-pro-firmware-_VERSION_.tar.gz`
   - binaries are unsigned unless `SIGN=1` argument is provided - this will make the signature using the  user's default GnuPG key;
   - file names are named after current Git version of the repository, but can be overridden with `VERSION=v0.xx` argument;
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

TODO: make the build containerized
