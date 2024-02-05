CC := gcc
CCPP := g++

INC := .\inc
SRC := .\src
OUT := .\out
OUT_R := $(OUT)\Release
OUT_D := $(OUT)\Debug

# Vulkan
VK_INC := C:\VulkanSDK\1.3.268.0\Include
VK_LIB := C:\VulkanSDK\1.3.268.0\Lib

# Flags
CFAGS := -std=c99 -Wall -Wextra -Wpedantic -I$(INC) -DUNICODE -D_UNICODE
CPPFLAGS := -std=c++11 -Wall -Wextra -Wpedantic -I$(INC) -I$(VK_INC) -DUNICODE -D_UNICODE

# условная сборка
mode := release

ifeq ($(mode), release)
	CFAGS += -DNDEBUG -g0 -o3
	CPPFLAGS += -DNDEBUG -g0 -o3
	OUT := $(OUT_R)
endif

ifeq ($(mode), debug)
	CFAGS += -D_DEBUG -g
	CPPFLAGS += -D_DEBUG -g
	OUT := $(OUT_D)
endif

# файлы приложения
APP_OBJ := $(OUT)/main.o $(OUT)/app_args.o $(OUT)/console.o $(OUT)/dbg_window_proc.o \
$(OUT)/game_loop.o $(OUT)/game_window_proc.o $(OUT)/str_converter.o $(OUT)/vulkan_device.o \
$(OUT)/vulkan_dynamic_ext_loader.o $(OUT)/vulkan_messenger.o $(OUT)/vulkan_renderer.o \
$(OUT)/window.o

# файлы библиотеки линейной алгебры
LA_OBJ := $(OUT)/LA_sup.o $(OUT)/Vector2D.o $(OUT)/Vector3D.o $(OUT)/Vector4D.o \
$(OUT)/Matrix2D.o $(OUT)/Matrix3D.o $(OUT)/Matrix4D.o $(OUT)/Quaternion.o

app.exe : $(APP_OBJ) LA.lib
	$(CCPP) -o $@ $^ -L. -lLA -L$(VK_LIB) -lvulkan-1

$(APP_OBJ) : $(OUT)/%.o : $(SRC)/%.cpp | $(OUT)
	$(CCPP) $(CPPFLAGS) -c $< -o $@

LA.lib : $(LA_OBJ)
	ar rcs $@ $^
	ranlib $@

$(LA_OBJ) : $(OUT)/%.o : $(SRC)/%.c | $(OUT)
	$(CC) $(CFAGS) -c $< -o $@

$(OUT):
	@if not exist "$(OUT)" mkdir "$(OUT)"

.PHONY: clean
clean:
	@if exist "$(OUT)" rd /s /q $(OUT)
	@if exist ".\*.exe" del /q .\*.exe
	@if exist ".\*dll" del /q .\*dll
	@if exist ".\*lib" del /q .\*lib
	