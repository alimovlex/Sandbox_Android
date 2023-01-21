# Android apk templates (examples) without using gradle.


Prerequisites
==
- [JDK-17](https://jdk.java.net/java-se-ri/17)
- zip
- [Command line tools](https://developer.android.com/studio#downloads)
- [Setting Up Enviroment Variables](https://developer.android.com/studio/command-line/variables)
    - Example of my .bashrc
        ```bash
        ## Java part
        export JAVA_HOME="$HOME/.local/java/jdk-17"
        if [[ ! $PATH == *"$JAVA_HOME/bin"* ]]; then
            export PATH="$PATH:$JAVA_HOME/bin"
        fi

        ## Android part
        export ANDROID_USER_HOME="$HOME/.local/android/SDK"
        export ANDROID_HOME="$ANDROID_USER_HOME"
        export ANDROID_AVD_HOME="$ANDROID_USER_HOME/avd"
        if [[ ! $PATH == *"$HOME/.local/android/SDK/cmdline-tools/tools/bin"* ]]; then
            export PATH="$PATH:$HOME/.local/android/SDK/cmdline-tools/tools/bin"
        fi
        ```
- Using [`sdkmanager`](https://developer.android.com/studio/command-line/sdkmanager) install.
    ``` shell
    $ sdkmanager "build-tools;29.0.3" "platform-tools" "platforms;android-29"
    ```

Debug Key
==
if not sure how to get debug.keystore.
```shell
$ keytool -keyalg RSA -genkeypair -alias debug -keypass android -keystore debug.keystore -storepass android -dname "CN=Android Debug,O=Android,C=US" -validity 9999 -deststoretype pkcs12
```
> keytool comes with jdk

How to build
==
run
```shell
$ ./build
```
> if `./build` dose not have execution rights just type `chmod +x ./build` before running.
