B=build
all:
	cmake -H. -B$(B)
	$(MAKE) -C$(B)
.PHONY: Debug Release RelWithDebInfo MinSizeRel
Debug Release RelWithDebInfo MinSizeRel:
	cmake -H. -B$(B)/$@ -DCMAKE_BUILD_TYPE=$@
	$(MAKE) -C$(B)/$@
.PHONY: clean
clean:
	rm -rf $(B)


