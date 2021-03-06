#Description

The game source code typos were modified so that it would compile with gcc with the flags O3 and standard c++ 17. The modification's included the ./configure file for the removal of debug and addition of the stated gcc flags. The other updates included only typos where the functions required an numerical definition that was missing ie wxsizer(4, 4) was updated to (0, 4, 4, 0).

My goal was to get the best/fastest game code and hopefully pass on a great reference for others projects (Android). An adaptable open source user interface for android could be Xash3d (halflife for android).

The release contains the compiled binary and the zipped up game files are included in the mods folder as .7z

#Compile & Install: Any linux distro (game version 44)

sudo apt-get install debhelper dh-autoreconf gawk libalut-dev libexpat1-dev libfftw3-dev libfreetype6-dev libgl1-mesa-dev libglew-dev libglu1-mesa-dev libjpeg-dev libogg-dev libopenal-dev libpng-dev libsdl-net1.2-dev libsdl1.2-dev libvorbis-dev libwxgtk2.8-dev pkg-config jackd2

#unzip with 7zip data/globalmods/apoc.7z and none.7z.001 none.7z.002

#optionally unzip fprofile-use.tar.gz and add -fprofile-use to all ldflags and cflags option or go through the process of -fprofile-generate, run the program to stress its most common path and then recompile with -fprofile-use. The .gcda files will be hard linked to the path compiled from, this will improve game cpu performance dramatically and the game uses lots of cpu power with many AI players

#For those looking to run a dedicated server or to host games on the internet exposed to random access by others. I'd suggest adding -fstack-protector-all or at a minimum -fstack-protector. The cost of -fstack-protector-all, -fstack-protector-strong, -fstack-protector was stated to be 10 %, 1-5% and < 1% reduced performance to safeguard the computer system, as a side note -fprofile-generate -fprofile-use could eaisly make up for the reduced performance and overall be worthwhile for a dedicated server with many of AI players.

./configure

make -j4

sudo mkdir /usr/local/games/scorched3d/share/

sudo mkdir /usr/local/games/scorched3d/share/data

sudo mkdir /usr/local/games/scorched3d/share/documentation

sudo chown -R yourloginusername:yourloginusername /usr/local/games/scorched3d/

chmod 755 -R /usr/local/games/scorched3d/

make install

chmod 755 /usr/local/games/scorched3d/sorched3d

chmod 755 /usr/local/games/scorched3d/sorched3dc

chmod 755 /usr/local/games/scorched3d/sorched3ds

strip --strip-unneeded --remove-section=.comment /usr/local/games/scorched3d/*

#Reminder that the game expects the unzipped data files to be in /usr/local/games/scorched3d/share (copy the sourcecode data directory)

Run scorched3dc in the terminal to play the game to check for any error outputs. I recommend configuring the game with scorched3d binary first

#

#

#

#

# The rest is optional from here

#This is for the jackd... though sound seems to work in ubuntu without doing the following and the following may be an old security risk from what i read:

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

#check that you're using the most up to date opengl version/Mesa -- (gpu performance improves because the current open source AMD radeon driver is lagging behind with opengl 4+ default support)

sudo apt-get install libgl1-mesa-dev mesa-common-dev driconf indicator-cpufreq

glxinfo | grep core

glxgears -info

#if glx core info shows a high open gl version than glx gears run the folowing command (my max core open gl version was 4.1 and yes COMPAT needs to be there for it to work)

driconf #Set allow a higher compat profile for apps that request it, overide gl version and use extra thread and enable dual source blending
Right click the cpufreq indicator that should have appeared in the right upper hand corner near the shutdown button and set the system to performance mode

MESA_GL_VERSION_OVERRIDE=4.5COMPAT ./scorched3dc

--One AMD/ATI mainline mesa experimental open source radeon gpu out performs 58% better than two windows 10 crossfire radeon gpu's

#now you may have noticed that the GL version is to 4.5, that's because mesa actually supports 4.5, though currently does not do so officially, however your graphics card hardware must support it. I went ahead and attached the AMD drivers to the release https://github.com/jjaxse2017/scorched-earth-3d/releases. See the mainline mesa expermental repository https://github.com/jjaxse2017/Mainline-Mesa-Expermental on how to compile from mesa mainline.

sudo unzip lib32.zip testmesa_x32_x64.zip -d /

export LIBGL_DRIVERS_PATH=/testmesa64:/testmesa32

export LD_LIBRARY_PATH=/testmesa64:/testmesa32:/usr/local/lib/libexpat64:/usr/local/lib/libexpat32

export EGL_DRIVERS_PATH=/testmesa64:/testmesa32

#DRI_PRIME=1 is for your 2nd gpu if you have one, otherwise omit that command

DRI_PRIME=1 MESA_GL_VERSION_OVERRIDE=4.5COMPAT ./scorched3dc

Unigine Heaven Benchmark 4.0 (DX12 installed and using DX11.2 vs Mesa 13.0, GCC 7.0, LLVM 6.0, std C++17, no debug)

#No overclocking enabled

OS     : win10  /    Ubuntu 17.04

FPS    : 56.4   /    88.9 

Score  : 1422   /    2240

Min FPS:	8.6    /    32.6

Max FPS:	99.5   /    157.6

System Platform:	Windows NT 6.2 (build 9200) 64bit / Linux 4.14.0-999-generic x86_64

CPU model:	Intel(R) Core(TM)2 Duo CPU E6850 @ 3.00GHz (3829MHz) x2 / Intel(R) Core(TM)2 Duo CPU E6850 @ 3.00GHz (3829MHz) x2
 
GPU model:	AMD Radeon HD 5800 Series 15.201.1151.1008 (1024MB) x2 / Unknown GPU (256MB) x1


Settings

Render:	Direct3D11 / OpenGL (Special note here on windows open gl the FPS was lower than 10)

Mode:	1680x1050 8xAA fullscreen / 1680x1050 8xAA windowed

Preset:	Custom / Custom

Quality:	High / High

Tessellation:	Disabled / Disabled
