TOOLCHAIN = $(HOME)/android
GCC = $(TOOLCHAIN)/ndk-bundle/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android29-clang
CXX_FLAGS = -march=x86_64 --sysroot=$(TOOLCHAIN)/sysroot

BUILD_TOOLS=$(HOME)/android/build-tools/29.0.3
PLATFORM=$(HOME)/android/platforms/android-29/android.jar

AAPT2=$(BUILD_TOOLS)/aapt2
D8=$(BUILD_TOOLS)/d8
ZIPALIGN=$(BUILD_TOOLS)/zipalign
APKSIGNER=$(BUILD_TOOLS)/apksigner

all: clean build deploy 

.PHONY : build
.PHONY : deploy
.PHONY : clean

deploy :
	adb shell am force-stop io.github.synix.helloworld
	adb uninstall io.github.synix.helloworld
	adb install -r build/app.apk
	adb shell am start -n io.github.synix.helloworld/.MainActivity 
build : 
	mkdir -p compiled build
	$(AAPT2) compile res/values/strings.xml res/layout/activity_main.xml -o compiled/
	$(AAPT2) link -o build/unsigned_app.apk -I $(PLATFORM) -R compiled/*.flat --manifest AndroidManifest.xml --java src/ -v
	javac -d classes -classpath src -bootclasspath $(PLATFORM) -source 1.8 -target 1.8 src/io/github/synix/helloworld/MainActivity.java
	javac -d classes -classpath src -bootclasspath $(PLATFORM) -source 1.8 -target 1.8 src/io/github/synix/helloworld/R.java
	$(D8) classes/io/github/synix/helloworld/*.class --lib $(PLATFORM) --output build/
	zip -uj build/unsigned_app.apk build/classes.dex
	$(ZIPALIGN) -f 4 build/unsigned_app.apk build/app.apk
	keytool -genkey -v -keystore my-release-key.keystore -alias standkey -keyalg RSA -keysize 2048 -validity 10000 -storepass password -keypass password -dname "CN=example.com, OU=ID, O=Example, L=Doe, S=John, C=GB"
	$(APKSIGNER) sign --key-pass pass:password --ks-pass pass:password --ks my-release-key.keystore build/app.apk 
		
clean:
	rm -rf classes/
	rm -rf compiled/
	rm -rf build/
	rm -rf src/io/github/synix/helloworld/R.java
	rm my-release-key.keystore 