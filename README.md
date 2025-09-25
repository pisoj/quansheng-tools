### Features (that people care about):
- Converting **from Chirp's** CSV to Quansheng **Docks's CHAN** files
- **GUI and CLI** applications for the mentioned task
- **Library abstraction** for parsing thous two file formats to make it easier to **integrate into other tools**

### Features only I care about:
- Changing **font** in the GUI application
- Minimal dependencies - only [IUP](https://www.tecgraf.puc-rio.br/iup) for GUI
- Super light - about 2 MiB with the GUI application and all dependencies

![Screenshoot of the GUI utility for converting CSV to CHAN files](./screenshoot.avif)

## Windows
### Download your usual [`exe`](https://github.com/pisoj/quansheng-tools/releases/latest)
The installer gives you both **system-wide** and **portable** installation options

## Linux
### You can also download precompiled [binaries for Linux](https://github.com/pisoj/quansheng-tools/releases/latest).

### From source
```shell
git clone https://github.com/pisoj/quansheng-tools.git
cd quansheng-tools
make -j8
sudo make install
sudo ldconfig
```
If you are **on Fedora**, also run this to include allow loading **locally compiled** libraries:
```shell
sudo sh -c 'echo "/usr/local/lib64/" > /etc/ld.so.conf.d/local-lib64.conf && ldconfig'
```
**Note:** You will need gtk, which you probably already have but:
```shell
Debian: sudo apt-get install libgtk-3-dev
Fedora: sudo dnf install gtk3
```

#### Running without installing
If you want to run the application **without installing** it you will need to specify where the libraries are located before you run it:
```shell
cd build/quansheng-channels_gui
LD_LIBRARY_PATH=../libquansheng-channels:../lib/iup ./quansheng-channels_gui
```

## Compiling for Windows
Compiling for Windows is done on Linux by cross-compiling with [MinGW](https://fedoraproject.org/wiki/MinGW/Tutorial).
To install `MinGW` on Fedora run:
```shell
sudo dnf install mingw64-gcc (or mingw32-gcc if you want a 32-bit Windows binary)
```
Then, **instead of** normal `make`, use:
```shell
mingw64-make clean
mingw64-make -j8
```
> Note: If you have previously compiled for Linux with the normal `make`
> you should clean before rebuilding: **`make clean`**.
> When changing between 32 and 64-bit windows a `make clean` is also required.

### Packaging Windows installer
If you want to package the GUI application into a self contained **installer** you will need [NSIS](https://nsis.sourceforge.io/Main_Page).

I have only ever used `NSIS` on Windows so I **won't** describe how to do make the installer **completely on Linux**.
1. Copy all dependencies into a single folder
```shell
cd src/quansheng-channels_gui/
mingw64-make prepare_windows_packaging
```

2. Now go to `build/quansheng-channels_gui/packaging/windows` and copy that folder on a **Windows machine** with `NSIS` installed. 

3. Open `NSIS` and click _**`Compile NSI script`**_
4. Go to _**`File -> Load script`**_ and select the **`insteller.nsi`** file inside the **folder** you **previously copied**.
5. The installer should be in the **same folder** under the name **`Quansheng channel tool setup.exe`**

## Contributions
I am making everything based around shared libraries to allow **other projects** to **benefit** as well.
It would be nice if this software could **program Quansheng directly**.
