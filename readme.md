#Description

The game source code typos were modified so that it would compile with gcc with the flags O3 and standard c++ 17. The modification's included the ./configure file for the removal of debug and addition of the stated gcc flags. The other updates included only typos where the functions required an numerical definition that was missing ie wxsizer(4, 4) was updated to (4, 4, 0, 0).

My goal was to get the best/fastest game code and hopefully pass on a great reference for others projects (Android). An adaptable open source user interface for android could be Xash3d (halflife for android).

The release contains the compiled binary and the zipped up game files are included in the mods folder as .7z

#Compile & Install: Any linux distro (game version 44)

sudo apt-get install debhelper dh-autoreconf gawk libalut-dev libexpat1-dev libfftw3-dev libfreetype6-dev libgl1-mesa-dev libglew-dev libglu1-mesa-dev libjpeg-dev libogg-dev libopenal-dev libpng-dev libsdl-net1.2-dev libsdl1.2-dev libvorbis-dev libwxgtk2.8-dev pkg-config jackd2

#unzip with 7zip data/globalmods/apoc.7z and none.7z.001 none.7z.002

./configure

make -j4

make install

Run scorched3dc in the terminal to play the game (Configure with scorched3d first, you'll get errors during configure just hit continue)

#the game expects the unzipped data files to be in /usr/local/games/scorched3d/share (copy the sourcecode data directory)

#This is for the jackd... though sound seems to work in ubuntu without doing the following:

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

#log out and login to X (you may have to restart) and then run the jackd in a seperate x terminal (again sounds works without jackd in ubuntu)

jackd -r -d alsa -r 44100

# check that you're using the most up to date open gl version (gpu performance improves because the current open source AMD radeon driver is lagging behind with opengl 4+ default support)

sudo apt-get install libgl1-mesa-dev mesa-common-dev driconf

glxinfo | grep core

glxgears -info

#if glx core info shows a high open gl version than glx gears run the folowing command (my core open gl version was 4.1 and yes COMPAT needs to be there for it to work)

driconf #Set to overide gl version and use extra thread
MESA_GL_VERSION_OVERRIDE=4.1COMPAT ./scorched3dc


Unigine Heaven Benchmark 4.0 (Windows DX11/Updated Mesa 13.0 GCC 7.0/LLVM 6.0 std C++17, no debug)


FPS: 56.4/72.5 

Score: 1422 /1825

Min FPS:	8.6 / 9.7

Max FPS:	99.5 / 123.9

System Platform:	Windows NT 6.2 (build 9200) 64bit / Linux 4.14.0-999-generic x86_64

CPU model:	Intel(R) Core(TM)2 Duo CPU E6850 @ 3.00GHz (3829MHz) x2 / Intel(R) Core(TM)2 Duo CPU E6850 @ 3.00GHz (3829MHz) x2
 
GPU model:	AMD Radeon HD 5800 Series 15.201.1151.1008 (1024MB) x2 / Unknown GPU (256MB) x1


Settings

Render:	Direct3D11 / OpenGL (Special note here on windows open gl the FPS was lower than 10)

Mode:	1680x1050 8xAA fullscreen / 1680x1050 8xAA windowed

Preset:	Custom / Custom

Quality:	High / High

Tessellation:	Disabled / Disabled
