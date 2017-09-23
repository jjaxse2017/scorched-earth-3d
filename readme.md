#Description

The game source code typos were modified so that it would compile with gcc with the flags O3 and standard c++ 17. The modification's included the ./configure file for the removal of debug and addition of the stated gcc flags. The other updates included only typos where the functions required an numerical definition that was missing ie wxsizer(4, 4) was updated to (4, 4, 0, 0).

My goal was to get the best/fastest game code and hopefully pass on a great reference for others projects (android/raspberry pi). A great example is Xash3d released a very nice android game of halflife/counterstrike 1.6.

The release contains the compiled binary and the zipped up game files are included in the mods folder as .7z

#Compile & Install: Any linux distro (game version 44)

sudo apt-get install debhelper dh-autoreconf gawk libalut-dev libexpat1-dev libfftw3-dev libfreetype6-dev libgl1-mesa-dev libglew-dev libglu1-mesa-dev libjpeg-dev libogg-dev libopenal-dev libpng-dev libsdl-net1.2-dev libsdl1.2-dev libvorbis-dev libwxgtk2.8-dev pkg-config jackd2

#unzip with 7zip data/globalmods/apoc.7z and none.7z.001 none.7z.002

./configure

make -j4

make install

#add the following to /etc/security/limits.conf

@audio - rtprio 99

@audio - memlock 83274202

@audio - nice -10

#add the following to /etc/security/limits.d/99-realtime.conf

@realtime   -  rtprio     99

@realtime   -  memlock    unlimited


sudo groupadd realtime

#be sure to use your login id for 'yourUserID'

sudo usermod -a -G realtime yourUserID

#Run the jackd in a seperate terminal

jackd -r -d alsa -r 44100

run the scorched3d to setup and the scorched3dc in the terminal (I had to configure with scorched3d and play with scorched3dc)

#the game expects the unzipped data files to be in /usr/local/games/scorched3d/share (copy the sourcecode data directory)
