![image](https://bitbucket.com/ultranoteiNetwork/ultranotei-assets/blob/master/splash.png)
![](https://bitbucket.com/bomb-on/ultranotei-core/workflows/Ubuntu%2016.04/badge.svg)  
![](https://bitbucket.com/bomb-on/ultranotei-core/workflows/Ubuntu%2018.04/badge.svg)  
![](https://bitbucket.com/bomb-on/ultranotei-core/workflows/Windows/badge.svg)  
![](https://bitbucket.com/bomb-on/ultranotei-core/workflows/macOS/badge.svg)

# ultranotei Core (CLI)
Latest Release: v1.0.0
Maintained by ultranotei Developers.

## Information
ultranotei is a decentralized banking platform with encrypted messages and own privacy protected cryptocurrency.

ultranotei is based on the Cryptonote protocol and runs on a secure peer-to-peer network technology to operate with no central authority. You control your private keys to your funds, you control your destiny. ultranotei Network is accessible by anyone in the world regardless of his/her geographic location or status. Our blockchain is resistant to any kind of analysis. All your xuni transactions and messages are anonymous. ultranotei Network provides an instant secure, untraceable and unlinkable way of encrypted communication - crypto messages.

ultranotei is open-source, community driven and truly decentralized. No one owns it, everyone can take part.

## Resources
- Web: [http://ultranotei.network](https://ultranotei.network/)
- bitbucket: [https://bitbucket.com/ultranoteiNetwork/ultranotei-core](https://bitbucket.com/ultranoteiNetwork/ultranotei-core)
- Wiki: [https://ultranotei.network/wiki](https://ultranotei.network/wiki/doku.php)
- Explorer: [https://explorer.ultranotei.network](https://explorer.ultranotei.network)
- Paperwallet: [https://ultranotei.network/paperwallet](https://ultranotei.network/paperwallet/)
- Discord: [https://discord.gg/YbpHVSd](https://discord.gg/YbpHVSd)
- Twitter: [https://twitter.com/ultranoteiNetwork](https://twitter.com/ultranoteiNetwork)
- Medium: [https://medium.com/@ultranoteiNetwork](https://medium.com/@ultranoteiNetwork)
- Reddit: [https://www.reddit.com/r/ultranoteiNetwork/](https://www.reddit.com/r/ultranoteiNetwork/)
- Bitcoin Talk: [https://bitcointalk.org/index.php?topic=4515873](https://bitcointalk.org/index.php?topic=4515873)


## Compiling ultranotei from source

### Linux / Ubuntu

##### Prerequisites

- You will need the following dependencies to build the ultranotei CLI: boost, cmake, git, gcc, g++, python, and make.
- On Ubuntu: `sudo apt-get install -y build-essential python-dev gcc g++ git cmake libboost-all-dev`

#### Building

- `git clone https://bitbucket.com/ultranoteiNetwork/ultranotei-core`
- `cd ultranotei-core`
- `mkdir build && cd $_`
- `cmake ..`
- `make`

If the build is successful the binaries will be in the src folder.

### Windows 10

##### Prerequisites

- Install [Visual Studio 2019 Community Edition](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16)
- Install [CMake](https://cmake.org/download/)
- When installing Visual Studio, you need to install **Desktop development with C++** and the **MSVC v142 - VS 2019 C++ x64/x86 build tools** components. The option to install the v142 build tools can be found by expanding the "Desktop development with C++" node on the right. You will need this for the project to build correctly.
- Use the premade binaries for [MSVC 14.2](https://bitbucket.com/ultranoteiNetwork)
- Install [Boost 1.70.0](https://sourceforge.net/projects/boost/files/boost-binaries/1.70.0/boost_1_70_0-msvc-14.1-64.exe/download), ensuring you download the installer for MSVC 14.2.

##### Building

- From the start menu, open 'x64 Native Tools Command Prompt for vs2019' or run "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsMSBuildCmd.bat" from any command prompt.

- `git clone https://bitbucket.com/ultranoteiNetwork/ultranotei-core`
- `cd ultranotei-core`
- `mkdir build`
- `cd build`
- `cmake -G "Visual Studio 16 2017" -A x64 -DBOOST_LIBRARYDIR="c:\local\boost_1_65_1\lib64-msvc-14.2 ..` (Or your boost installed dir.)
- `msbuild ultranoteI.sln /p:Configuration=Release /m`

If the build is successful the binaries will be in the src/Release folder.

### macOS

#### Prerequisites

In order to install prerequisites, [XCode](https://developer.apple.com/xcode/) and [Homebrew](https://brew.sh/) needs to be installed.
Once both are ready, open terminal app, run following command:

```bash
$ xcode-select --install
```

and install all tools. On newer macOS versions (v10.14 and higher) this step is done through Software Update app.

After that, proceed with installing dependencies:

```bash
$ brew install git python cmake gcc boost@1.60
```

Ensure that Boost is properly linked:

```bash
$ brew link --force boost@1.60
```

When all dependencies are installed, build ultranotei Core binaries:

```bash
$ git clone https://bitbucket.com/ultranoteiNetwork/ultranotei-core
$ cd ultranotei-core
$ mkdir build && cd $_
$ cmake ..
$ make
```

If the build is successful the binaries will be located in `src` directory.

#### Special Thanks
Special thanks goes out to the developers from Cryptonote, Bytecoin, Monero, Forknote, TurtleCoin, Karbo and Masari.
