OUT_DIR			= output
SRC 			= ./src
LINK_TARGET 	= csv-to-sql
SRC_FILES 		= $(shell find $(SRC) -name '*.cpp')
OBJS 			= $(patsubst $(SRC)/%.cpp, $(OUT_DIR)/%.o, $(SRC_FILES))
REBUILDABLES	= $(OBJS) $(LINK_TARGET)
CC_FLAGS		= -std=c++17 -Wall -g -Ofast
CC				= clang++
OUTPUT_FOLDERS	= $(addprefix $(OUT_DIR)/, $(notdir $(patsubst $(SRC), , $(shell find $(SRC) -maxdepth 5 -type d))))

all: $(LINK_TARGET)
	@echo "compilation success ✅"

$(LINK_TARGET): $(OBJS)
	$(CC) $(CC_FLAGS) -o $@ $^

$(OUT_DIR)/%.o: $(SRC)/%.cpp
	$(CC) $(CC_FLAGS) -o $@ -c $<

clean:
	rm -rf $(OUT_DIR)/*
	@echo "clean done ✨"

init:
	mkdir -p $(OUT_DIR) $(OUTPUT_FOLDERS)
	@$(MAKE)

run:
	./output/main.out
