SRC_DIRS := ./src ./lib
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

BUILD_DIR := ./build
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

LIB_DIRS := ./lib
LIBS := $(shell find $(LIB_DIRS) -name '*.a')
LIB_FLAGS := -L$(LIB_DIRS) $(LIBS)

INC_DIRS := $(shell find $(SRC_DIRS) -type d) 
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC := emcc
CFLAGS := $(INC_FLAGS) -MMD -MP -Wall -Wextra -Werror
TARGET_EXEC := particles.html
LDFLAGS := $(LIB_FLAGS) -Os -sUSE_GLFW=3 -DPLATFORM_WEB --shell-file ./src/shell.html

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean run

run: 
	emmake $(MAKE) 
	emrun ./build/$(TARGET_EXEC)

clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
