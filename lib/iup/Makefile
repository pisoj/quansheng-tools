IS_TARGETING_WINDOWS := $(shell $(CC) -dumpmachine | grep mingw)
IS_64_BIT := $(shell $(CC) -dumpmachine | grep x86_64)

PROJECT_NAME := iup
BUILD_DIR := ../../build/lib/$(PROJECT_NAME)

IUP_URL_LINUX64 := https://master.dl.sourceforge.net/project/iup/3.32/Linux%20Libraries/iup-3.32_Linux68_64_lib.tar.gz?viasf=1

ifeq ($(IS_64_BIT),)
	IUP_URL_WINDOWS := https://master.dl.sourceforge.net/project/iup/3.32/Windows%20Libraries/Dynamic/iup-3.32_Win32_dllw6_lib.zip?viasf=1
else
	IUP_URL_WINDOWS := https://master.dl.sourceforge.net/project/iup/3.32/Windows%20Libraries/Dynamic/iup-3.32_Win64_dllw6_lib.zip?viasf=1
endif

ifndef PREFIX
	PREFIX := /usr/local
endif
ifndef LIB_DIR
	LIB_DIR := lib64
endif

.PHONY: clean install uninstall

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
ifeq ($(IS_TARGETING_WINDOWS),)
	curl -Lo - "$(IUP_URL_LINUX64)" | tar -xz -C $(BUILD_DIR)
else
	curl -Lo $(BUILD_DIR)/iup_bin.zip "$(IUP_URL_WINDOWS)" && unzip -d $(BUILD_DIR) $(BUILD_DIR)/iup_bin.zip && rm $(BUILD_DIR)/iup_bin.zip
endif

clean:
	rm -r $(BUILD_DIR)

install: $(BUILD_DIR)
	cp $(BUILD_DIR)/libiup*.so $(PREFIX)/$(LIB_DIR)
	mkdir -p $(PREFIX)/include/$(PROJECT_NAME)
	cp $(BUILD_DIR)/include/*.h $(PREFIX)/include/$(PROJECT_NAME)

uninstall:
	rm $(PREFIX)/$(LIB_DIR)/libiup*.so
	rm -r $(PREFIX)/include/$(PROJECT_NAME)
