# Using a different Compiler

The isotactics project needs at least c++14 to compile which requires the GNU g++ compiler 4.9+.

In case you are on a system with older default C/C++ compilers like gruenau4.informatik.hu-berlin.de you need to tell cmake where it can find the newer ones.

### csh
If using the csh shell (default on gruenauX):

    sh build.sh clean
    setenv CC=/usr/bin/gcc-5
    setenv CXX=/usr/bin/g++-5
    sh build.sh

or as one liner

    sh build.sh clean && setenv CC /usr/bin/gcc-5 && setenv CXX /usr/bin/g++-5 && sh build.sh

This will tell cmake to use '/usr/bin/gcc-5' as c-compiler and '/usr/bin/g++-5' as c++-compiler. If your compilers are located elsewhere adjust as needed.

### bash
If you are using a different shell like bash, use export instead of setenv:

    sh build.sh clean
    export CC=/usr/bin/gcc-5
    export CXX=/usr/bin/g++-5
    sh build.sh
