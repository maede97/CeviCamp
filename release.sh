set -e
cd build;
cmake .. -DCMAKE_BUILD_TYPE=RELEASE;
make;
cd ..;
mkdir -p release/res
cp build/main release/Linux
cp -r res/* release/res/
cd release/
zip -r release.zip *
cp release.zip ..
cd ..
rm -r release/