# BeatzPer

Clone this repo then create a music folder:
`mkdir -p www/MusicFiles`

Drop a bunch of music files in there, or do it later - the server picks up on changes to the filesystem.

To build the code:
```
git submodule init

git submodule update

mkdir build

cd build

conan install ..

cmake ..

cmake --build .

cd ..
```

To Run it:
`./build/bin/BeatzPer`

Either `curl http://localhost:8080/files_info.json` directly or visit
`http://localhost:8080` for a link to the json file.
