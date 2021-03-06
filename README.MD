# Setting up my C++ 

1) Install CMAKE from https://cmake.org/download/
https://github.com/Kitware/CMake/releases/download/v3.17.1/cmake-3.17.1-win64-x64.msi

Add it to the PATH

## Setting up CLANG on Windows

Donloaded LLVM from https://github.com/llvm/llvm-project
git clone https://github.com/llvm/llvm-project llvm

Ran the CMAKE UI on the llvm cmake folder
Ran the CMAKE UI on the clang cmake folder 

Generated the Visual studio solutions
Opened in Visual studio both solution, changed to Release and selected BUILD ALL.

## Compiling with clang
```
clang ../main.cpp -o main.exe
```

## Compiling with msvcpp 
```
cl.exe ../main.cpp -o main.exe
```

## Setting up the paths for CL 
The environment variables for CL must be set with the correct paths to your visual studio development environment, of course, if you use the visual studio development command line, there are already set for you. But I prefer to use Microsoft Terminal or the Intergrated terminal in Visual Studio code, so I'm setting them up in my paths. 

LIB = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.25.28610\lib\x64;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x64;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x64;" 
INCLUDE = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.25.28610\include\;C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt;"

# Setting up vcpkg

git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

## Acquiring an ampq library to communicate with rabbitMQ

vcpkg install amqpcpp --triplet x64-windows-static
vcpkg install sqlite3 --triplet x64-windows-static

# cli-sqlite interface 

```cli-sqlite\main.cpp```

To test out the vcpkg sqlite3, I decided to follow the vcpkg readme to check that I can include and link to the sqlite3 files. I tried using the cmake method described, but kept running into issues. So I decided to cut short the investigation into CMAKE and just compile everything directly from cl.exe using a command file: ```cl @cl-params.txt``` which reads and applies all the commands of the file to the compiler. 

```
/O2 /Fe: build\hellosql.exe
/I C:\Repos\vcpkg\packages\sqlite3_x64-windows-static\include\
src\main.cpp 
C:\Repos\vcpkg\packages\sqlite3_x64-windows-static\lib\sqlite3.lib
/link /machine:X64
```

This compiles the hellosql.exe and checks that it all runs. 

I also added a example of parameter binding SQL Statement to the SQLite3 code. 

## Reference
SQLITE3 CPP : https://www.tutorialspoint.com/sqlite/sqlite_insert_query.htm
BIND INSERT : http://www.adp-gmbh.ch/sqlite/bind_insert.html

# cli-rabbitmq

*NOT WORKING*

*Currently not working due to issues with compilation and tcp connection*

```
C:\Repos\vcpkg\packages\amqpcpp_x64-windows-static\include\amqpcpp/libboostasio.h(83): error C2039: 'posix': is not a member of 'boost::asio' 
```

vcpkg install amqpcpp --triplet x64-windows-static
vcpkg install boost-asio --triplet x64-windows-static 

reference: https://github.com/CopernicaMarketingSoftware/AMQP-CPP

# cli-redis 

*WORKS*

There are a few redis clients in vcpkg, I was going to use hiredis for simplicity(* Nope / had an issue with uresolved dependencies to win32_interop.lib in hiredis.lib*). So I've got with [cpp_redis](https://github.com/cpp-redis/cpp_redis) due the fact it has no dependencies. 

```
vcpkg install cpp-redis  --triplet x64-windows-static
```

And finally used the [Examples](https://github.com/cpp-redis/cpp_redis/wiki/Examples) directly because of the initialisation of the Windows network dll causing issues. 

```
#ifdef _WIN32
  //! Windows network DLL init
  WORD version = MAKEWORD(2, 2);
  WSADATA data;

  if (WSAStartup(version, &data) != 0) {
    std::cerr << "WSAStartup() failure" << std::endl;
    return -1;
  }
#endif /* _WIN32 */
```

cpp_redis_ref: https://github.com/cpp-redis/cpp_redis 

# cli-sql-db-pg 

*NOT WORKING*

Didn't work due to compilation issues after installing : vcpkg install libpqxx --triplet x64-windows-static

Didn't work due to compilation issues after installing : vcpkg install libodb-pgsql --triplet x64-windows-static

Even ```vcpkg install libpq[client] --triplet x64-windows-static --recurse``` _failed_
```
Error: Building package libpq:x64-windows-static failed with: BUILD_FAILED
Please ensure you're using the latest portfiles with `.\vcpkg update`
  Package: libpq:x64-windows-static
  Vcpkg version: 2020.02.04-nohash
```

# REST Server and client

For the REST Server and client, I'm using the [cpprestsdk from Microsoft](https://github.com/Microsoft/cpprestsdk) which makes it really easy. 

```
vcpkg install cpprestsdk cpprestsdk:x64-windows
```

I'm using the code from https://mariusbancila.ro/blog/2017/11/19/revisited-full-fledged-client-server-example-with-c-rest-sdk-2-10/ and it directly works! 

This time, I'm not using static build, so I have to include the DLLs in the Build directory to launch the applications, but they work out of the box in Administrator mode. 