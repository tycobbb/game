include ./Makefile.base.mk
include ./Makefile.build.mk

# -- cosmetics --
help-colw = 7

## -- init (i) --
$(eval $(call alias, init, i/0))
$(eval $(call alias, i, i/0))

## init dev env
i/0: d/init $(l-glfw) $(l-raylib)
.PHONY: i/0

## -- build (b) --
$(eval $(call alias, build, b/0))
$(eval $(call alias, b, b/0))

## build the game
b/0: $(db-bin)
.PHONY: b/0

## force a build
b/force: b/clean b/0
.PHONY: b/force

## clean the build
b/clean:
	rm -rf $(db-dst)
.PHONY: b/clean

## clean build & lib
b/reset: b/clean d/clean
.PHONY: b/reset

## -- run (r) --
$(eval $(call alias, run, r/0))
$(eval $(call alias, r, r/0))

## run the game
r/0:
	$(db-bin)
.PHONY: r/0