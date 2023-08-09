# SCAPI

This project is designed to better understand the API's from Rockstar Games used in Grand Theft Auto V.

## Features

Listing all commands and keeping them here up-to-date would be pain. Just go to src/commands

### Libraries

This project was builded on Microsoft Visual Studio Community 2022 (64bit)

Dependencies via CMake: 
- [Botan](https://github.com/randombit/botan)
- [cpr](https://github.com/libcpr/cpr)
- [pugixml](https://github.com/libcpr/cpr)
- Python for Botan

Build Instructions (Win64)
```bash
git clone https://github.com/BugisoftRSG/SCAPI.git
cd SCAPI
mkdir build && cd build
cmake ..
```

## Known Issues
- Some endpoints may return an error since they are not available for the platform or no longer used.
