# BitonalFill

This is a finger exercise in SIMD-optimization. The operation implemented is: 

* We have a bitonal bitmap (=one bit per pixel).
* In a second bitmap, we want to set each pixel which is a one (or a zero) in the bitonal bitmap to a specific value.

The bitonal bitmap here is organized as follows: the most significant bit corresponds to the leftmost pixel.
Here is an example for "FillFromBitonalFromOnes" with a "valueForOnes" of 0x23:

![example of operation](https://github.com/ptahmose/BitonalFill/blob/master/Readme/operation-example.png?raw=true)

The repository contains implementations leveraging [AVX-instructions](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions) (Intel/AMD) and [Neon](https://developer.arm.com/documentation/dht0002/a/Introducing-NEON/NEON-architecture-overview/NEON-instructions#:~:text=The%20NEON%20instructions%20provide%20data,manage%20all%20program%20flow%20control.) (ARM).

## license

The code is licensed under the [unlicense](http://unlicense.org/) license. Please see the file license.txt in this repository.