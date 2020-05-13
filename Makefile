COMP = g++

.PHONY: bin clean

BINARY_NAME = solver
SOURCE_DIR = source
BUILD_DIR = build
HEADER_FILES := $(wildcard $(SOURCE_DIR)/*.h)
OBJECT_FILES := $(BUILD_DIR)/main.o $(BUILD_DIR)/parsing.o

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(HEADER_FILES)
	$(COMP) $(CFLAGS) -c -o $@ $<

bin: $(OBJECT_FILES)
	$(COMP) $(CFLAGS) -o $(BINARY_NAME) $^

clean:
	rm -f $(BUILD_DIR)/* $(BINARY_NAME)
