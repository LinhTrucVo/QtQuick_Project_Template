## Getting Started

Optional: install Qt6 in linux using package manager
```bash
apt install build-essential qt6-base-dev qt6-declarative-dev qt6-tools-dev qt6-qmltooling-plugins
```

### Clone the Repository

To clone this repository with all submodules:

```bash
git clone --recurse-submodules https://github.com/LinhTrucVo/QtQuick_Project_Template.git
cd QtQuick_Project_Template
```

If you already cloned the repository without submodules, initialize them:

```bash
git submodule update --init --recursive
```
or update them:

```bash
git submodule update --remote --recursive
```

After update submodule:

```bash
git add src/lib/QtLib_Project_Template
git commit -m "updare submodule"
git push
```

## Submodules

This project uses the following submodules:

- **QtLib_Project_Template**: Core PyQt threading and messaging library
  - Repository: https://github.com/LinhTrucVo/QtLib_Project_Template.git
  - Path: `src/QtLib_Project_Template`


## Deploy 

```bash
# Windows
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64" -DCMAKE_C_COMPILER="C:/Qt/Tools/mingw1310_64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
#set PATH to qt bin before deploy (example - C:/Qt/6.9.1/mingw_64/bin)
windeployqt --qmldir "../src/Client_Code" src/.
```

```bash
# Linux
mkdir build
cd build
cmake -G "Ninja"  -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6  -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Create Code
```sh
cd src/lib/QtLib_Project_Template/tool
python create_client_code.py
```
<img width="267" height="182" alt="image" src="https://github.com/user-attachments/assets/36189443-605b-42ed-ab66-f52f14fcc2a9" />


## Debugging Tips:
qDebug does not output the message to VSCode Debug Console, qDebug output the message in Qt Creator only.

To print debug message in VSCode Debug Console, use the VSCode LogPoint instead.

Reference: https://code.visualstudio.com/docs/debugtest/debugging
