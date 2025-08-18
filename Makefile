BUILD_DIR := build

# Default target.
# Build project and produce static library
lib: $(BUILD_DIR)
	@cmake -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE=Release
	@cmake --build "$(BUILD_DIR)" --target grain

test: lib
	@cmake -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE=Release
	@cmake --build "$(BUILD_DIR)" --target test

lib_debug: $(BUILD_DIR)
	@cmake -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE=Debug
	@cmake --build "$(BUILD_DIR)" --target grain

test_debug: lib_debug
	@cmake -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE=Debug
	@cmake --build "$(BUILD_DIR)" --target test

# Create necessary directories
$(BUILD_DIR):
	@mkdir -p "$@"

# Clean rule
clean:
	@rm -rf "$(BUILD_DIR)"

.PHONY: lib test test_debug clean
