
#Compile & Install: Any linux distro (game version 44)

sudo apt-get install debhelper dh-autoreconf gawk libalut-dev libexpat1-dev libfftw3-dev libfreetype6-dev libgl1-mesa-dev libglew-dev libglu1-mesa-dev libjpeg-dev libogg-dev libopenal-dev libpng-dev libsdl-net1.2-dev libsdl1.2-dev libvorbis-dev libwxgtk2.8-dev pkg-config

./configure

make -j4

make install
