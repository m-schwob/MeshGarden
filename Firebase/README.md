## environment tools

run all commands in powershell
there may be issue with firestore command in powershell. there is solution one of those command
 Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope menas
 Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

### environment setup

links:
https://codewithandrea.com/articles/firebase-cloud-functions/
https://jsmobiledev.com/article/firebase-emulator-guide/
https://jsmobiledev.com/article/firebase-emulator/

- install node.js

npm install -g firebase-tools

firebase login  
firebase init      
firebase deploy --only functions

firebase init emulators
firebase emulators:export emulator_ex_data
firebase emulators:start --import emulator_ex_data_db

### import/export from production database

npm install -g node-firestore-import-export

firestore-export --accountCredentials meshgarden-iot-firebase-adminsdk-o9fjg-0ca4c4061f.json --backupFile firestore_seed.json
firebase emulators:start --only firestore
$env:FIRESTORE_EMULATOR_HOST = "0.0.0.0:8080"
firestore-import --accountCredentials meshgarden-iot-firebase-adminsdk-o9fjg-0ca4c4061f.json --backupFile firestore_seed.json
firebase emulators:export emulator_ex_data_db


### kill port tool
npm install --global kill-port
kill-port --port 8080