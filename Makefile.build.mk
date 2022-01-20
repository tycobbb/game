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
l-glfw = $(db-dep)/glfw-$(l-glfw-v)
l-glfw-v = 3.3.6

# -- tools --
tb-clang = clang -I"$(db-inc)" -L"$(db-lib)" -lglfw.3 -framework OpenGL -Wl,-rpath,"@executable_path/../lib"

# -- targets --
# -- t/build
$(db-bin): $(db-dst)
	$(tb-clang) -o $(db-bin) $(db-root)

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
	mv $(l-glfw)/lib-x86_64/* $(db-lib)

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
