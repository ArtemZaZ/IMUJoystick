/* карта регистров ITG3205 */
#ifndef ITG3205RM_H_
#define ITG3205RM_H_

#define ITG3205_ADDR					0x68
/* ITG3205 registers */
#define ITG3205_DEVID					0x00
#define ITG3205_SMPLRT_DIV    0x15
#define ITG3205_DLPF_FS       0x16
#define ITG3205_INT_CFG       0x17
#define ITG3205_INT_STATUS    0x1A
#define ITG3205_TEMP_OUT_H    0x1B
#define ITG3205_TEMP_OUT_L    0x1C
#define ITG3205_GYRO_XOUT_H   0x1D
#define ITG3205_GYRO_XOUT_L   0x1E
#define ITG3205_GYRO_YOUT_H   0x1F
#define ITG3205_GYRO_YOUT_L   0x20
#define ITG3205_GYRO_ZOUT_H   0x21
#define ITG3205_GYRO_ZOUT_L   0x22
#define ITG3205_PWR_MGM       0x3E

#endif /* ITG3205RM_H_ */
