# Embedded device drivers library

All documentation of the driver can be fount at https://gonzafernan.github.io/embedded-drivers/

## Sensors
### ICM-20948
- Description: 9-Axis MEMS Motion Tracking Device
- Datasheet link: https://invensense.tdk.com/wp-content/uploads/2024/03/DS-000189-ICM-20948-v1.6.pdf

### AK09916
- Description: 3-axis Electronic Compass (integrated in ICM20948).
- Datasheet link: https://www.y-ic.es/datasheet/78/SMDSW.020-2OZ.pdf

## How to build the library
From the 'build/' folder:

```bash
cmake ..
make
ctest
```