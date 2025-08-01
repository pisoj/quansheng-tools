MAKEFILES := $(shell find src -name Makefile)
PHONY := all $(MAKECMDGOALS) lib/*/Makefile

all: $(MAKEFILES)
	$(info $(MAKEFILES))

$(MAKECMDGOALS): $(MAKEFILES)

define process_makefile
deps_$1 := $(shell \
  test -f $(dir $1)/Makefile.deps && \
  cd $(dir $1) && $(MAKE) -s -f Makefile.deps print-deps || echo "")
  PHONY += $1
$1: $$(deps_$1)
	$$(info Dependencies for $1 are $$(deps_$1))
	cd $$(dir $$@) && $$(MAKE) $$(MAKECMDGOALS)
endef

$(foreach mf,$(MAKEFILES),$(eval $(call process_makefile,$(mf))))

lib/*/Makefile:
	cd $(dir $@) && $(MAKE) $(MAKECMDGOALS)

.PHONY: $(PHONY)
