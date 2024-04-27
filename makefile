SRC_DIRS := ./src ./lib
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

BUILD_DIR := ./build
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

LIB_DIRS := ./lib
LIBS := $(shell find $(LIB_DIRS) -name '*.a')
LIB_FLAGS := -L$(LIB_DIRS) $(LIBS:./lib/lib%.a=-l%)

INC_DIRS := $(shell find $(SRC_DIRS) -type d) 
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC := emcc
CFLAGS := $(INC_FLAGS) -MMD -MP -Wall
TARGET_EXEC := particles.html
LDFLAGS := -Os -sUSE_GLFW=3 -DPLATFORM_WEB

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LIB_FLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(TARGET_EXEC)
	emrun ./build/$(TARGET_EXEC)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
