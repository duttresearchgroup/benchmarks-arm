sudo losetup -f ./benchmarks.img
mkdir -p benchmarks.imgmount
sudo mount /dev/loop0 benchmarks.imgmount

