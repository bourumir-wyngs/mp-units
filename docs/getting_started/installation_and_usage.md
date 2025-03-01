# Installation And Usage

!!! info

    **mp-units** library tries to provide the best user experience possible with the C++ language.
    To achieve that, it extensively uses C++20 features and the
    [explicit object parameter](https://en.cppreference.com/w/cpp/language/member_functions#Explicit_object_parameter)
    from C++23.

    Even though the library benefits from C++23 (if available), C++20 is enough to compile and
    use all of the library's functionality. C++23 features are hidden behind a preprocessor macro
    providing a backward-compatible way to use it.

    Sadly, as of today, not many compilers provide full C++20 support. The library compiles fine
    on the following compilers (or newer):

    - gcc-12.2

    In the upcoming weeks, we will be actively working to extend the support to other compilers as well.


## Repository Structure and Dependencies

This repository contains three independent CMake-based projects:

- _./src_

    - header-only project containing whole **mp-units** library
    - _./src/CMakeList.txt_ file is intended as an **entry point for library users**
    - in case this library becomes part of the C++ standard, it will have no external dependencies
      but until then, it depends on the following:

        - [gsl-lite](https://github.com/gsl-lite/gsl-lite) to verify runtime contracts with
          the `gsl_Expects` macro,
        - [{fmt}](https://github.com/fmtlib/fmt) to provide text formatting of quantities
          (if `std::format` is not supported yet on a specific compiler),
        - [only for clang < 14 with libc++] [range-v3](https://github.com/ericniebler/range-v3)
          to provide needed C++20 concepts and utilities.

- _._

    - project used as an **entry point for library development and CI/CD**
    - it wraps _./src_ project together with usage examples and tests
    - additionally to the dependencies of _./src_ project, it uses:

        - `Catch2 <https://github.com/catchorg/Catch2>`_ library as a unit tests framework,
        - `linear algebra <https://github.com/BobSteagall/wg21/tree/master/include>`_
          library based on proposal `P1385 <https://wg21.link/P1385>`_ used in some examples
          and tests.

    - in case you also want to generate the project's documentation, you will need:

        - [Material for MkDocs](https://squidfunk.github.io/mkdocs-material)

- *./test_package*

    - library installation and Conan package verification.


!!! tip

    Top level _CMakeLists.txt_ file should only be used by **mp-units** developers and contributors
    as an entry point for the project's development. We want to ensure that everyone will build **ALL**
    the code correctly before pushing a commit. Having such options would allow unintended issues to
    leak to PRs and CI.

    This is why our projects have two entry points:

    - _./CMakeLists.txt_ is to be used by projects developers to build **ALL** the project code with really
    restrictive compilation flags,
    - _./src/CMakeLists.txt_ contains only a pure library definition and should be used by the customers
    that prefer to use CMake's `add_subdirectory()` to handle the dependencies.

    To learn more about the rationale, please check our
    [FAQ](faq.md#why-dont-you-have-cmake-options-to-disable-building-of-tests-and-examples).


## Obtaining Dependencies

This library assumes that most of the dependencies will be provided by the
[Conan Package Manager](https://conan.io/). If you want to obtain required
dependencies by other means, some modifications to the library's CMake files might be needed.
The rest of the dependencies responsible for documentation generation are provided by
`python3-pip`.


### Conan Quick Intro

In case you are not familiar with Conan, to install it (or upgrade) just do:

```shell
pip3 install -U conan
```

After that, you might need to add a custom profile file for your development environment
in _~/.conan2/profiles_ directory. An example profile can look as follows:

```ini hl_lines="5" title="~/.conan2/profiles/gcc12"
[settings]
arch=x86_64
build_type=Release
compiler=gcc
compiler.cppstd=20
compiler.libcxx=libstdc++11
compiler.version=12
os=Linux

[conf]
tools.build:compiler_executables={"c": "gcc-12", "cpp": "g++-12"}
```

!!! tip

    Please note that the **mp-units** library requires C++20 to be set in a Conan profile or forced
    via the Conan command line. If you do the former, you will not need to provide `-s compiler.cppstd=20`
    every time your run a Conan command line (as provided in the command line instructions below).

Additionally, it is recommended to set Ninja as a CMake generator for Conan. To do so, you should create
a _~/.conan2/global.conf_ file that will set `tools.cmake.cmaketoolchain:generator` to one of Ninja
generators. For example:

```text title="~/.conan2/global.conf"
tools.cmake.cmaketoolchain:generator="Ninja Multi-Config"
```

!!! note

    _~/.conan2/global.conf_ file may also set `tools.cmake.cmake_layout:build_folder_vars` which
    [makes working with several compilers or build configurations easier](https://docs.conan.io/2/reference/tools/cmake/cmake_layout.html#multi-setting-option-cmake-layout).
    For example, the below line will force Conan to generate separate CMake presets and folders for
    each compiler:

    ```text title="~/.conan2/global.conf"
    tools.cmake.cmake_layout:build_folder_vars=["settings.compiler", "settings.compiler.version"]
    ```

    In such a case, you will need to use a configuration-specific preset name in the Conan instructions
    provided below rather then just `conan-default` and `conan-release`
    (i.e. `conan-gcc-11` and `conan-gcc-11-release`)


## Build Options

### Conan Configuration Properties

[`user.build:all`](#user-build-all){ #user-build-all }

:   [:octicons-tag-24: 0.8.0][build all support] · :octicons-milestone-24: `True`/`False` (Default: `False`)

    Enables compilation of all the source code including tests and examples. To support
    this it requires some additional Conan build dependencies described in
    [Repository Structure and Dependencies](#repository-structure-and-dependencies).
    It also runs unit tests during Conan build (unless `tools.build:skip_test`
    configuration property is set to `True`).

[build all support]: https://github.com/mpusz/mp-units/releases/tag/v0.8.0


[`user.build:skip_la`](#user-skip-la){ #user-skip-la }

:   [:octicons-tag-24: 0.8.0][skip la support] · :octicons-milestone-24: `True`/`False` (Default: `False`)

    If `user.build:all` is enabled, among others, Conan installs the external
    [wg21-linear_algebra](https://conan.io/center/wg21-linear_algebra)
    dependency and enables the compilation of linear algebra-based tests and usage examples.
    Such behavior can be disabled with this option.

    [skip la support]: https://github.com/mpusz/mp-units/releases/tag/v0.8.0


### CMake Options

[`MP_UNITS_AS_SYSTEM_HEADERS`](#MP_UNITS_AS_SYSTEM_HEADERS){ #MP_UNITS_AS_SYSTEM_HEADERS }

:   [:octicons-tag-24: 2.0.0][as system headers support] · :octicons-milestone-24: `ON`/`OFF` (Default: `OFF`)

    Exports library as system headers.

    [as system headers support]: https://github.com/mpusz/mp-units/releases/tag/v2.0.0


[`MP_UNITS_BUILD_LA`](#MP_UNITS_BUILD_LA){ #MP_UNITS_BUILD_LA }

:   [:octicons-tag-24: 2.0.0][build la support] · :octicons-milestone-24: `ON`/`OFF` (Default: `ON`)

    Enables building code depending on the linear algebra library.

    [build la support]: https://github.com/mpusz/mp-units/releases/tag/v2.0.0


[`MP_UNITS_IWYU`](#MP_UNITS_IWYU){ #MP_UNITS_IWYU }

:   [:octicons-tag-24: 2.0.0][iwyu support] · :octicons-milestone-24: `ON`/`OFF` (Default: `OFF`)

    Enables `include-what-you-use` when compiling with a clang compiler.
    Additionally turns on [`MP_UNITS_AS_SYSTEM_HEADERS`](#MP_UNITS_AS_SYSTEM_HEADERS).

    [iwyu support]: https://github.com/mpusz/mp-units/releases/tag/v2.0.0


[`MP_UNITS_USE_LIBFMT`](#MP_UNITS_USE_LIBFMT){ #MP_UNITS_USE_LIBFMT }

:   [:octicons-tag-24: 2.0.0][use libfmt support] · :octicons-milestone-24: `ON`/`OFF` (Default: `ON`)

    Enables usage of [{fmt}](https://github.com/fmtlib/fmt) library instead of the C++20 Standard
    Library feature.

    [use libfmt support]: https://github.com/mpusz/mp-units/releases/tag/v2.0.0


## CMake with Presets Support

It is recommended to use at least CMake 3.23 to build this project as this version introduced support
for CMake Presets schema version 4 used now by Conan to generate presets files. All build instructions
below assume that you have such support. If not, your CMake invocations have to be replaced with something
like:

```shell
mkdir build && cd build
cmake .. -G "Ninja Multi-Config" -DCMAKE_TOOLCHAIN_FILE=<path_to_generators_dir>/conan_toolchain.cmake
cmake --build . --config Release
```

!!! note

    In case you can't use CMake 3.23 but you have access to CMake 3.20 or later, you can append
    `-c tools.cmake.cmaketoolchain.presets:max_schema_version=2` to the `conan install` command
    which will force Conan to use an older version of the CMake Presets schema.


## Installation and Reuse

There are many different ways of installing/reusing **mp-units** in your project. Below we mention
only a few of many options possible.

!!! info

    The easiest and most recommended way to obtain **mp-units** is with the Conan package manager.
    See [Conan + CMake (release)](#conan-cmake-release) for a detailed instruction.


### Copy

As **mp-units** is a C++ header-only library you can simply copy all needed _src/*/include_ subdirectories
to your source tree.

!!! note

    In such a case, you are on your own to ensure all the dependencies are installed, and their header
    files can be located during the build. Please also note that some compiler-specific flags are needed
    to make the code compile without issues.


### Copy + CMake

If you copy the whole **mp-units** repository to your project's file tree, you can reuse CMake targets
defined by the library. To do so, you should use _CMakeLists.txt_ file from the _./src_ directory:

```cmake
add_subdirectory(<path_to_units_folder>/src)
# ...
target_link_libraries(<your_target> <PUBLIC|PRIVATE|INTERFACE> mp-units::mp-units)
```

!!! note

    You are still on your own to make sure all the dependencies are installed and their header and CMake
    configuration files can be located during the build.


### Conan + CMake (release)

!!! tip

    If you are new to the Conan package manager, it is highly recommended to read
    [Obtaining Dependencies](#obtaining-dependencies) and refer to
    [Consuming packages](https://docs.conan.io/2/tutorial/consuming_packages.html)
    chapter of the official Conan documentation for more information.

**mp-units** releases are hosted on [Conan-Center](https://conan.io/center/mp-units).
To obtain an official library release, the following steps may be performed:

1. Create Conan configuration file (either _conanfile.txt_ or _conanfile.py_) in your
   project's top-level directory and add **mp-units** as a dependency of your project.
   For example, the simplest file may look as follows:

    ```ini title="conanfile.txt"
    [requires]
    mp-units/0.8.0

    [layout]
    cmake_layout

    [generators]
    CMakeToolchain
    CMakeDeps
    ```

2. Import **mp-units** and its dependencies definitions to your project's build procedure
   with `find_package`:

    ```cmake
    find_package(mp-units CONFIG REQUIRED)
    ```

3. Link your CMake targets with **mp-units**:

    ```cmake
    target_link_libraries(<your_target> <PUBLIC|PRIVATE|INTERFACE> mp-units::mp-units)
    ```

4. Download, build, and install Conan dependencies before running the CMake configuration step:

    ```shell
    conan install . -pr <your_conan_profile> -s compiler.cppstd=20 -b=missing
    cmake --preset conan-default
    cmake --build --preset conan-release
    ```

### Conan + CMake (Live At Head)

This chapter describes the procedure to Live At Head, which means using the latest version
of **mp-units** all the time.

!!! note

    Please note that even though the Conan packages that you will be using are generated **ONLY**
    for builds that are considered stable (passed our CI tests) some minor regressions may happen
    (our CI and C++20 build environment is not perfect yet). Also, please expect that the library
    interface might, and probably will, change occasionally. Even though we do our best, such
    changes might not be reflected in the project's documentation right away.

The procedure is similar to the one described in [Conan + CMake (release)](#conan-cmake-release)
with the following differences:

1. Before starting the previous procedure, add **mp-units** remote to your Conan configuration:

    ```shell
    conan remote add conan-mpusz https://mpusz.jfrog.io/artifactory/api/conan/conan-oss
    ```

2. In your Conan configuration file, provide the package identifier of the `mpusz/testing` stream:

    ```ini title="conanfile.txt" hl_lines="2"
    [requires]
    mp-units/2.0.0@mpusz/testing

    [layout]
    cmake_layout

    [generators]
    CMakeToolchain
    CMakeDeps
    ```

    !!! tip

        The identifiers of the latest packages can always be found in
        [the project's README file](https://github.com/mpusz/mp-units/blob/master/README.md) or on
        [the project's Artifactory](https://mpusz.jfrog.io/ui/packages/conan:%2F%2Fmp-units).

3. Force Conan to check for updated recipes with `-u`:

    ```shell
    conan install . -pr <your_conan_profile> -s compiler.cppstd=20 -b=missing -u
    ```


### Install

In case you don't want to use Conan in your project and just want to install the **mp-units**
library on your file system and use `find_package(mp-units)` from another repository
to find it, it is enough to perform the following steps:

```shell
conan install . -pr <your_conan_profile> -s compiler.cppstd=20 -b=missing
mv CMakeUserPresets.json src
cd src
cmake --preset conan-default -DCMAKE_INSTALL_PREFIX=<your_installation_path>
cmake --build --preset conan-release --target install
```


## Contributing (or just building all the tests and examples)

In case you would like to build all the **mp-units** source code (with unit tests and examples),
you should:

1. Use the _CMakeLists.txt_ from the top-level directory.
2. Run Conan with [`user.build:all`](#user-build-all) = `True`.

```shell
git clone https://github.com/mpusz/mp-units.git && cd units
conan build . -pr <your_conan_profile> -s compiler.cppstd=20 -c user.build:all=True -b missing
```

The above will download and install all of the dependencies needed for the development of the library,
build all of the source code, and run unit tests.

If you prefer to build the project via CMake rather than Conan, then you should replace
the `conan build` with `conan install` command and then follow with a regular CMake build:

```shell
cmake --preset conan-default
cmake --build --preset conan-release
cmake --build --preset conan-release --target test
```


## Building documentation

Starting from **mp-units 2.0** we are using [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/)
to build our documentation. The easiest way to install all the required dependencies
is with `pip`:

```shell
pip install -U mkdocs-material
```

After that, you can either:

- easily [start a live server to preview the documentation as you write](https://squidfunk.github.io/mkdocs-material/creating-your-site/#previewing-as-you-write)

    ```shell
    mkdocs serve
    ```

- [build the documentation](https://squidfunk.github.io/mkdocs-material/creating-your-site/#building-your-site)

    ```shell
    mkdocs build
    ```


## Packaging

To test CMake installation and Conan packaging or create a Conan package run:

```shell
conan create . <username>/<channel> -pr <your_conan_profile> -s compiler.cppstd=20 -c user.build:all=True -b missing
```

The above will create a Conan package and run tests provided in _./test_package_ directory.


## Uploading **mp-units** Package to the Conan Server

```shell
conan upload -r <remote-name> --all mp-units/2.0.0@<user>/<channel>
```
