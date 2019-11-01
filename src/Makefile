SRC_DIR := .
OBJ_DIR := .
OUT_DIR := .
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
OUT_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OUT_DIR)/%.out,$(SRC_FILES))

# Generate dependency (.d) files.
CXXFLAGS += -MMD

.PHONY: default all
default: all
all: $(OUT_FILES)

$(OUT_DIR)/%.out: $(OBJ_DIR)/%.o
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

# Use the dependency files so make knows what to rebuild.
-include $(OBJ_FILES:.o=.d)

.PHONY: clean
clean:
	rm -f $(OBJ_FILES) $(OBJ_FILES:.o=.d) $(OUT_FILES)

.PHONY: list
list:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'
