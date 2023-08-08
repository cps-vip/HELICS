clean:
	cd build && \
	make clean

clean-build-config:
	rm -rf build/ && \
	rm -rf install/

class-targets-build:
	mkdir -p build/ && \
	cd build/ && \
	cmake -DCMAKE_INSTALL_PREFIX=../install -DHELICS_BUILD_CXX_SHARED_LIB=ON -DHELICS_BUILD_EXAMPLES=ON -DHELICS_BUILD_TESTS=ON .. && \
	make -j$(nproc)

class-targets-install: class-targets-build
	make -j$(nproc) install

class-targets-combofed1-test:
	cd examples/comboFederates1 && \
	./run3.sh
