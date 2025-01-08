DB := lldb
SHELL := zsh

.DEFAULT_GOAL := help

.PHONY: clean run debug help build

BUILD_DIR = ./build
ifndef BIN_NAME
	BIN_NAME := wave-function-collapse
endif
ifndef CONFIG
CONFIG=debug
endif

clean:
	rm -rf $(BUILD_DIR);\
	cd external/vk-engine;\
	make clean;\
	cd ../..

build:
	@cd external/vk-engine;\
	make lib CONFIG=$(CONFIG);\
	cd ../..;\
	premake5 gmake2;\
	cd $(BUILD_DIR);\
	make config=$(CONFIG);\
	cd ..

run: build
ifeq ($(CONFIG), debug)
	@./build/bin/Debug/$(BIN_NAME) $(ARGS)
else
	@./build/bin/Release/$(BIN_NAME) $(ARGS)
endif

debug:
	@premake5 gmake2;\
	cd $(BUILD_DIR);\
	make config=debug;\
	$(DB) ./bin/Debug/$(BIN_NAME);\
	cd ..

help:
	@echo "Usage: make { clean | run | debug | help }"
	@echo "    clean - Remove build artifacts"
	@echo "    build - Compile and link the program"
	@echo "    run   - Compile, link and run the program"
	@echo "    debug - Compile, link and run the program in the debugger"
	@echo "            The default debugger is 'lldb', but can be changed"
	@echo "            by setting 'make debug DB=<your preferred debugger>'"
	@echo "    help  - Show this message"
