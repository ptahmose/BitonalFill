# BitonalFill

This is a finger exercise in SIMD-optimization. The operation implemented is: 

* We have a bitonal bitmap (=one bit per pixel).
* In a second bitmap, we want to set each pixel which is a one (or a zero) in the bitonal bitmap to a specific value.

The bitonal bitmap here is organized as follows: the most significant bit corresponds to the leftmost pixel.
Here is an example for "FillFromBitonalFromOnes" with a "valueForOnes" of 0x23:

![example of operation](https://github.com/ptahmose/BitonalFill/blob/master/Readme/operation-example.png?raw=true)

The repository contains implementations leveraging [AVX-instructions](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions) (Intel/AMD) and [Neon](https://developer.arm.com/documentation/dht0002/a/Introducing-NEON/NEON-architecture-overview/NEON-instructions#:~:text=The%20NEON%20instructions%20provide%20data,manage%20all%20program%20flow%20control.) (ARM).

## building

```
cd build
mkdir build
cmake .. -DCMAKE_BUILD_TYPE
cmake  --build . --config Release
```

## performance

Performance for the operation "FillFromOnes", 1MB=1,000,000 bytes.

i9-12900K, VisualStudio2022, x64

| version                | performance in MB/s |
| ---------------------- | ------------------- |
| gray8_C                | 320.7               |
| gray8_AVX              | 8297.7              |
| gray16_C               | 648.7               |
| gray16_AVX             | 8407.2              |
| BGR24_C                | 1014.9              |
| BGR24_AVX              | 8089.6              |
| BGR48_C                | 1887.1              |
| BGR48_AVX              | 7780.4              |
| float32_C              | 1336.1              |
| float32_AVX            | 7576.2              |

Raspberry Pi 4, arm64, gcc

| version                | performance in MB/s |
| ---------------------- | ------------------- |
| gray8_C                | 160.6               |
| gray8_Neon             | 1592.7              |
| gray16_C               | 320.0               |
| gray16_Neon            | 1561.4              |
| BGR24_C                | 461.5               |
| BGR24_Neon             | 1533.6              |
| BGR48_C                | 921.2               |
| BGR48_Neon             | 1449.1              |
| float32_C              | 636.6               |
| float32_Neon           | 1514.6              |

Galaxy Book Go (Snapdragon 7c Gen2), arm64, VisualStudio2022

| version                | performance in MB/s |
| ---------------------- | ------------------- |
| gray8_C                | 256.3               |
| gray8_Neon             | 4932.1              |
| gray16_C               | 531.0               |
| gray16_Neon            | 6325.8              |
| BGR24_C                | 754.6               |
| BGR24_Neon             | 5315.1              |
| BGR48_C                | 1446.7              |
| BGR48_Neon             | 6339.4              |
| float32_C              | 1009.3              |
| float32_Neon           | 6302.6              |

AMD Ryzen 5 1600X (Linux), x64, gcc9.3

| version                | performance in MB/s |
| ---------------------- | ------------------- |
| gray8_C                | 334.6               |
| gray8_Neon             | 1271.8              |
| gray16_C               | 678.2               |
| gray16_Neon            | 2435.5              |
| BGR24_C                | 986.3               |
| BGR24_Neon             | 2818.2              |
| BGR48_C                | 1974.8              |
| BGR48_Neon             | 3026.3              |
| float32_C              | 1358.1              |
| float32_Neon           | 3026.3              |

AMD Ryzen 7 4700U (Windows), x64, VisualStudio2022

| version                | performance in MB/s |
| ---------------------- | ------------------- |
| gray8_C                | 284.7               |
| gray8_Neon             | 2595.5              |
| gray16_C               | 549.8               |
| gray16_Neon            | 2745.7              |
| BGR24_C                | 847.0               |
| BGR24_Neon             | 2793.8              |
| BGR48_C                | 1674.5              |
| BGR48_Neon             | 2818.1              |
| float32_C              | 1136.1              |
| float32_Neon           | 2770.2              |

MediaTek Helio P60 (MT6771V/C), aarch64, gcc8.3

| version                | performance in MB/s |
| ---------------------- | ------------------- |
| gray8_C                | 262.0               |
| gray8_Neon             | 3198.5              |
| gray16_C               | 522.0               |
| gray16_Neon            | 3107.4              |
| BGR24_C                | 743.0               |
| BGR24_Neon             | 2671.3              |
| BGR48_C                | 1485.6              |
| BGR48_Neon             | 3088.7              |
| float32_C              | 1044.1              |
| float32_Neon           | 2892.6              |

i7-8700K (Windows), x64, VisualStudio2022

| version                | performance in MB/s |
| ---------------------- | ------------------- |
| gray8_C                | 306.0               |
| gray8_Neon             | 4850.9              |
| gray16_C               | 625.4               |
| gray16_Neon            | 5022.7              |
| BGR24_C                | 968.2               |
| BGR24_Neon             | 5006.9              |
| BGR48_C                | 1816.9              |
| BGR48_Neon             | 4877.4              |
| float32_C              | 1232.1              |
| float32_Neon           | 4944.1              |

## license

The code is licensed under the [unlicense](http://unlicense.org/) license. Please see the file license.txt in this repository.