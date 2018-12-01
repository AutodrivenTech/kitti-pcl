基于原版的 kitti-pcl 工具改的 kitti 数据转换到 pcd 的工具。


## Dependence

```
PCL 1.5+
Boost 1.48.0+
```

## Compile

```sh
mkdir build
cd build
cmake ..
make
```

生成的 `kitti2pcd` 在 `bin` 目录下。使用 `sudo make install` 可以安装到系统之中。

## Usage

```
./kitti2pcd --infile <path to kitti binary file> --outfile <path to pcd file>
OR
./kitti2pcd --indir <path to kitti binary dir> --outdir <path to pcd dir>

```

## Warning

<del>For PCL 1.8.1, the build will failed!</del>

Only Mac OS and Linux have been tested!
