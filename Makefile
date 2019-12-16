DEBUG_BUILD_DIR:=cmake-build-debug
RELEASE_BUILD_DIR:=cmake-build-release
TARGET:=scpterm
JOBS:=2

#CCACHE_EXISTS := $(shell ccache -V)
#ifdef CCACHE_EXISTS
#	CC := ccache $(CC)
#	CXX := ccache $(CXX)
#endif

build-debug:
	cmake --build ${DEBUG_BUILD_DIR} --target scpterm -- -j 2

build-release:
	cmake --build ${RELEASE_BUILD_DIR} --target scpterm -- -j 2