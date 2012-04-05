# Cranium Kernel Compiler.Set correct environment variables before executing!

# Make config
make cranium_defconfig

# Control kernel version for tracking purposes
# current_version is our kernel version,it is copied to .version since the compiler reads from .version
cp current_version .version

# Make zImage
make -j32

# Copy output files
mv arch/arm/boot/zImage boot.img-zImage
cp boot.img-zImage /home/sarthak/Documents/kernel-htc-pico/boot.img-tools/unpack	
cp drivers/net/wireless/bcm4330b2/bcm4330.ko /home/sarthak/Documents/kernel-htc-pico/boot.img-tools/output/system/lib/modules
cp drivers/net/kineto_gan.ko /home/sarthak/Documents/kernel-htc-pico/boot.img-tools/output/system/lib/modules
cp fs/cifs/cifs.ko /home/sarthak/Documents/kernel-htc-pico/boot.img-tools/output/system/lib/modules
rm boot.img-zImage
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
cd output 

#Make CWM-flashable zip 
zip cranium-kernel.zip boot.img META-INF/com/google/android/updater-script META-INF/com/google/android/update-binary system/lib/modules/bcm4330.ko  system/lib/modules/kineto_gan.ko system/lib/modules/cifs.ko
cp cranium-kernel.zip /home/sarthak/Documents/kernel-htc-pico/auto-sign
cd ../../

#Sign zip
cd auto-sign
java -jar signapk.jar testkey.x509.pem testkey.pk8 cranium-kernel.zip signed_cranium-kernel.zip

# Output is in auto-sign/cranium-kernel.zip.Boot into recovery mode and flash!
# Fastboot-compatible output is in boot.img-tools/target_img
