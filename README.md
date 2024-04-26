# ML-KEM (FIPS-203)
### Abstract
- Implementation of the [FIPS-203](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.203.ipd.pdf) standard in C, in where specifies the ML-KEM algorithm.
- Module Lattice Key Encryption Mechanism (ML-KEM) is supposed to be able to deal with the computing power of quantum computers.
- Tanking into account a trade of between security and computational resources, the user must select one of three parameter sets.


### Parameter Sets

In the **functs.h** file the values of whatever parameter set has been chosen must be written.
By default, the **ML-KEM-1024** paremeter is setted.

### Run the code
For the moment, use the **runTestML_KEM()** to run the whole algorithm an get the keys. 
Only the **gcc** compiler is needed to generate the file to be executed.

