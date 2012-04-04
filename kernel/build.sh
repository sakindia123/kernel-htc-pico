# Make config
make cranium_defconfig
# Make zImage
make -j8

# Copy output files
mv arch/arm/boot/zImage boot.img-zImage
cp boot.img-zImage /home/sarthak/Documents/kernel-htc-pico/boot.img-tools/unpack	
cp drivers/net/wireless/bcm4330b2/bcm4330.ko /home/sarthak/Documents/kernel-htc-pico/boot.img-tools/output
cp drivers/net/kineto_gan.ko /home/sarthak/Documents/kernel-htc-pico/boot.img-tools/output
cd ../
cd boot.img-tools
rm target_img/boot.img

# Make boot.img
tools/mkbootfs boot | gzip > unpack/boot.img-ramdisk-new.gz
mkdir -p target_img
tools/mkbootimg --kernel unpack/boot.img-zImage --ramdisk unpack/boot.img-ramdisk-new.gz -o target_img/boot.img --base `cat unpack/boot.img-base`	
cp target_img/boot.img output
#Strip modules
./strip.sh

#Profit!


