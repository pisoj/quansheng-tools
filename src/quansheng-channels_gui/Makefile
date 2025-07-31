EXECUTABLE_TYPE := application
CFLAGS += -I../../build -I../../build/lib/iup/include
LDFLAGS += -L../../build/libquansheng-channels -lquansheng-channels -L../../build/lib/iup -liup -liupimglib

include ../build-common/Makefile.common


PACKAGING_DIR_WIN := $(BUILD_DIR)/packaging/windows

prepare_windows_packaging: $(BUILD_DIR)/$(EXECUTABLE)
ifeq ($(IS_MINGW_WINDOWS),)
	@echo "You must build with MinGW to prepare for Windows packaging."
else
	mkdir -p $(PACKAGING_DIR_WIN)
	cp *.ico $(PACKAGING_DIR_WIN)
	cp *.nsi $(PACKAGING_DIR_WIN)
	cp $(BUILD_DIR)/../lib/iup/{iup,iupimglib}.dll $(PACKAGING_DIR_WIN)
	cp $(BUILD_DIR)/../libquansheng-channels/*.dll $(PACKAGING_DIR_WIN)
	cp $(BUILD_DIR)/$(EXECUTABLE) $(PACKAGING_DIR_WIN)
endif
