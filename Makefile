all:
	node-waf configure build

clean:
	rm -rf build

temp:
	rm -rf tmp/movefile
	mkdir -p tmp/movefile
	cp -r README.md *.{cc,h,js*} wscript Makefile tmp/movefile
	cd tmp/movefile

package: temp
	cd tmp && tar -czvf movefile.tgz movefile
