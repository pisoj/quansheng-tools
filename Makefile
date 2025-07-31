MAKEFILES := $(shell find src -name Makefile)
MAKEFILES_LIB := $(shell find lib -name Makefile)

.PHONY: all $(MAKECMDGOALS) $(MAKEFILES) $(MAKEFILES_LIB)
all: $(MAKEFILES)
$(MAKECMDGOALS): $(MAKEFILES)

$(MAKEFILES): $(MAKEFILES_LIB)
	cd $$(dirname $@) && $(MAKE) $(MAKECMDGOALS)

$(MAKEFILES_LIB):
	cd $$(dirname $@) && $(MAKE) $(MAKECMDGOALS)

