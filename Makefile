DB := lldb
SHELL := zsh

.DEFAULT_GOAL := help

.PHONY: clean run debug help

clean:
	rm -rf $(BUILD_DIR)

run:
	premake5 gmake2;\
	cd build;\
	make config=release;\
	./bin/Release/wave-function-collapse;\
	cd ..

debug:
	premake5 gmake2;\
	cd build;\
	make config=debug;\
	$(DB) ./bin/Release/wave-function-collapse;\
	cd ..

help:
	@echo "Usage: make { clean | run | debug | help }"
	@echo "    clean - Remove build artifacts"
	@echo "    run   - Compile, link and run the program"
	@echo "    debug - Compile, link and run the program in the debugger"
	@echo "            The default debugger is 'lldb', but can be changed"
	@echo "            by setting 'make debug DB=<your preferred debugger>'"
	@echo "    help  - Show this message"
