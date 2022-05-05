# BitonalFill

This is a finger exercise in SIMD-optimization. The operation implemented is: 

* We have a bitonal bitmap (=one bit per pixel).
* In a second bitmap, we want to set each pixel which is a one (or a zero) in the bitonal bitmap to a specific value.

The bitonal bitmap here is organized as follows: the most significant bit corresponds to the leftmost pixel.
Here is an example for "FillFromBitonalFromOnes" with a "valueForOnes" of 0x23:

![example of operation](https://github.com/ptahmose/BitonalFill/blob/master/Readme/operation-example.png?raw=true)

## license

The code is licensed under the [unlicense](http://unlicense.org/) license. Please see the file license.txt in this repository.