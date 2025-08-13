BUILD_DIR := build

# Default target.
# Build project and produce dynamic library
lib: $(BUILD_DIR)
	@cmake -S . -B "$(BUILD_DIR)"
	@cmake --build "$(BUILD_DIR)" --target grain

test: lib
	@cmake --build "$(BUILD_DIR)" --target test

# Create necessary directories
$(BUILD_DIR):
	@mkdir -p "$@"

# Clean rule
clean:
	@rm -rf "$(BUILD_DIR)"

.PHONY: lib test clean
