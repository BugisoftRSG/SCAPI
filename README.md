# SCAPI

This project is designed to better understand the API's from Rockstar Games used in Grand Theft Auto V.

## Features

### UGC
- Copy Job (From other/same platforms to PC, but not vice versa)
- Copy Local Job (Copy your own jobs)
- Delete Content (Can delete any job you created)
- Query Content (Get the job meta info of a job)
- Check Text (Checks if a given text is a profanity)
- Query Content Creators (Actual usage unknown)

### Auth
- Create Ticket

### Geo Location
- Get Location Info from IP (Looks like its Max Mind DB)

### Matchmaking
- Get Matchmaking (Lists all Sessions available to join)

### Inbox
- Send Email (May also corrupt the target's inbox if base64'd gamer handle is wrong)
- Send Bounty completed Message

### Presence
- Send Invite (IIRC only works on friends since the ballstortue incursion)

### Misc
- Decrypt (Allows you to decrypt an http request/response and get the raw xml)

### Libraries

This project was builded on Microsoft Visual Studio Community 2022 (64bit)

Dependencies: 
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
- GetTopRatedContent returns SqlException
