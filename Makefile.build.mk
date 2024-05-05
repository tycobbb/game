# -- data --
db-src = ./src
db-dst = ./dst
db-lib = ./lib
db-inc = ./include
db-tmp = ./tmp
db-dep = ./dep
db-root = $(db-src)/main.c
db-bin = $(db-dst)/game

# -- lib --
l-raylib = raylib
l-raylib-ver = 5.0
l-raylib-ver-dylib = 500
l-raylib-dl = $(db-tmp)/raylib-$(l-raylib-ver).tar.gz
l-raylib-src = $(db-dep)/raylib-$(l-raylib-ver)/src
l-raylib-lib = $(db-lib)/libraylib.$(l-raylib-ver-dylib).dylib
l-raylib-inc = $(db-inc)/raylib

# -- tools --
tb-clang = clang \
	-I"$(db-inc)" \
	-I"$(db-src)" \
	-L"$(db-lib)" \
	-lraylib.$(l-raylib-ver-dylib) -framework OpenGL -Wl,-rpath,"@executable_path/../lib"

# -- targets --
# -- t/build
$(db-bin): $(db-dst)
	$(tb-clang) -o $(db-bin) $(wildcard $(db-src)/*.c)

$(db-dst):
	mkdir -p $(db-dst)

# -- t/raylib
$(l-raylib): $(l-raylib-lib) $(l-raylib-inc)
.PHONY: $(l-raylib)

$(l-raylib-lib): $(l-raylib-src)
	cd $(l-raylib-src) \
		&& make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

	cd $(db-lib) \
		&& ln -s ../$(l-raylib-src)/libraylib.$(l-raylib-ver-dylib).dylib .

$(l-raylib-inc): $(l-raylib-src)
	mkdir -p $(l-raylib-inc)

	cd $(l-raylib-inc) \
		&& for h in ../../$(l-raylib-src)/*.h; do ln -s "$$h" .; done

$(l-raylib-src): $(l-raylib-dl)
	tar -xvf $(l-raylib-dl) -C $(db-dep)

$(l-raylib-dl):
	curl \
		-L https://github.com/raysan5/raylib/archive/refs/tags/$(l-raylib-ver).tar.gz \
		-o $(l-raylib-dl)

# -- t/dirs
d/init: $(db-lib) $(db-inc) $(db-dep) $(db-tmp)
.PHONY: d/init

d/clean:
	rm -rf $(db-lib)
	rm -rf $(db-inc)
	rm -rf $(db-dep)
	rm -rf $(db-tmp)
.PHONY: d/clean

$(db-lib):
	mkdir -p $(db-lib)

$(db-inc):
	mkdir -p $(db-inc)

$(db-tmp):
	mkdir -p $(db-tmp)

$(db-dep):
	mkdir -p $(db-dep)