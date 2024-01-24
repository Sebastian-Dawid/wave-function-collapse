DB := lldb
SHELL := zsh

.DEFAULT_GOAL := help

.PHONY: clean run debug help

BUILD_DIR = ./build
ifndef BIN_NAME
	BIN_NAME := wave-function-collapse
endif

clean:
	rm -rf $(BUILD_DIR)

run:
	premake5 gmake2;\
	cd $(BUILD_DIR);\
	make config=debug;\
	cd ..;\
	./build/bin/Debug/$(BIN_NAME) $(ARGS)

debug:
	premake5 gmake2;\
	cd $(BUILD_DIR);\
	make config=debug;\
	$(DB) ./bin/Debug/$(BIN_NAME);\
	cd ..

help:
	@echo "Usage: make { clean | run | debug | help }"
	@echo "    clean - Remove build artifacts"
	@echo "    run   - Compile, link and run the program"
	@echo "    debug - Compile, link and run the program in the debugger"
	@echo "            The default debugger is 'lldb', but can be changed"
	@echo "            by setting 'make debug DB=<your preferred debugger>'"
	@echo "    help  - Show this message"
