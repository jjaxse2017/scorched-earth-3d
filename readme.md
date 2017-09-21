
#Compile & Install: Any linux distro (game version 44)

sudo apt-get install debhelper dh-autoreconf gawk libalut-dev libexpat1-dev libfftw3-dev libfreetype6-dev libgl1-mesa-dev libglew-dev libglu1-mesa-dev libjpeg-dev libogg-dev libopenal-dev libpng-dev libsdl-net1.2-dev libsdl1.2-dev libvorbis-dev libwxgtk2.8-dev pkg-config

#unzip with 7zip data/globalmods/apoc.7z and none.7z.001 none.7z.002

./configure

make -j4

make install

run the scorched3d or scorched3dc in console (I had to configure with scorched3d and play with scorched3dc)

#the game expects the data files to be in /usr/local/games/scorched3d/share just copy the data directory over and it should work
