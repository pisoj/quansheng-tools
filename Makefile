MAKEFILES := $(shell find src -name Makefile)

.PHONY: all $(MAKECMDGOALS) $(MAKEFILES)
all: $(MAKEFILES)
$(MAKECMDGOALS): $(MAKEFILES)

$(MAKEFILES):
	cd $$(dirname $@) && $(MAKE) $(MAKECMDGOALS)

