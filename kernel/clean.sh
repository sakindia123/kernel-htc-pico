echo  "========================================================= "
echo  "         CRANIUM KERNEL CLEANING SCRIPT                   "
echo " ========================================================= "


rm .config
make clean -j32
cd ../
cd auto-sign && rm signed_cranium-kernel.zip
cd ../
cd boot.img-tools/output
rm cranium-kernel.zip boot.img
cd system/lib/modules
rm *.ko
cd ../../../../
cd target_img && rm boot.img


echo  "========================================================= "
echo  "         CRANIUM KERNEL CLEANED!!                         "
echo " ========================================================= "
