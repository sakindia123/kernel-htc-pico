for i in $(find . | grep .ko | grep './')
do
        echo $i
/home/sarthak/Documents/android_prebuilt_toolchains/arm-eabi-4.4.3/bin/arm-eabi-strip --strip-unneeded $i

done
