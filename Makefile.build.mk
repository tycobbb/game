# -- data --
db-src = ./src
db-dst = ./dst
db-lib = ./lib
db-inc = ./include
db-tmp = ./tmp
db-dep = ./dep
db-bin = $(db-dst)/game
db-dbg = $(db-dst)/game-debug

# -- lib --
l-glfw = $(db-dep)/glfw-$(l-glfw-v)
l-glfw-v = 3.3.6
l-glfw-arch = arm64

l-ufbx = $(db-dep)/ufbx-$(l-ufbx-v)
l-ufbx-v = 0.17.1

# -- tools --
tb-clang = clang \
	-I"$(db-inc)" \
	-I"$(db-src)" \
	-L"$(db-lib)" \
	-lglfw.3 -framework OpenGL \
	-Wl,-rpath,"@executable_path/../lib"

# -- targets --
# -- t/build
$(db-bin): $(db-dst)
	$(tb-clang) -o $(db-bin) $(wildcard $(db-src)/*.c) $(wildcard $(db-lib)/*.c)

-- todo: remove duplication w/ $(db-bin)
$(db-dbg): $(db-dst)
	$(tb-clang) -g -O0 -o $(db-dbg) $(wildcard $(db-src)/*.c) $(wildcard $(db-lib)/*.c)

$(db-dst):
	mkdir -p $(db-dst)

# -- t/lib
$(l-glfw):
	curl \
		-L https://github.com/glfw/glfw/releases/download/$(l-glfw-v)/glfw-$(l-glfw-v).bin.MACOS.zip \
		-o $(db-tmp)/glfw-$(l-glfw-v).zip

	unzip $(db-tmp)/glfw-$(l-glfw-v).zip \
		-d $(db-tmp)

	rm $(db-tmp)/glfw-$(l-glfw-v).zip
	mv $(db-tmp)/* $(l-glfw)

	mv $(l-glfw)/include/* $(db-inc)
	mv $(l-glfw)/lib-$(l-glfw-arch)/* $(db-lib)

$(l-ufbx):
	curl \
		-L https://github.com/ufbx/ufbx/archive/refs/tags/v$(l-ufbx-v).zip \
		-o $(db-tmp)/ufbx-$(l-ufbx-v).zip

	unzip $(db-tmp)/ufbx-$(l-ufbx-v).zip \
		-d $(db-tmp) \
		ufbx-$(l-ufbx-v)/ufbx.h ufbx-$(l-ufbx-v)/ufbx.c

	rm $(db-tmp)/ufbx-$(l-ufbx-v).zip
	mv $(db-tmp)/* $(l-ufbx)

	mv $(l-ufbx)/ufbx.h $(db-inc)
	mv $(l-ufbx)/ufbx.c $(db-lib)

# -- t/dirs
d/init: $(db-lib) $(db-inc) $(db-dep) $(db-tmp)
.PHONY: i/dirs

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
