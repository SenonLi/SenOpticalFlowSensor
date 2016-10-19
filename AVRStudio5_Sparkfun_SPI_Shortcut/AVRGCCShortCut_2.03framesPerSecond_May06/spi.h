#define REG_PRODUCT_ID 0x00
#define REG_REVISION_ID 0x01
#define MOTION 0x02
#define DELTA_X 0x03
#define DELTA_Y 0x04
#define SQUAL 0x05
#define PIXEL_SUM 0x06
#define MAXIMUM_PIXEL 0x07
#define CONFIGURATION_BITS 0x0a
#define EXTENDED_CONFIG 0x0b
#define DATA_OUT_LOWER 0x0c
#define DATA_OUT_UPPER 0x0d
#define SHUTTER_LOWER 0x0e
#define SHUTTER_UPPER 0x0f
#define FRAME_PERIOD_LOWER 0x10
#define FRAME_PERIOD_UPPER 0x11
#define MOTION_CLEAR 0x12
#define FRAME_CAPTURE 0x13
#define SROM_ENABLE 0x14
#define FRAME_PERIOD_MAX_BOUND_LOWER 0x19
#define FRAME_PERIOD_MAX_BOUND_UPPER 0x1a
#define FRAME_PERIOD_MIN_BOUND_LOWER 0x1b
#define FRAME_PERIOD_MIN_BOUND_UPPER 0x1c
#define SHUTTER_MAX_BOUND_LOWER 0x1d
#define SHUTTER_MAX_BOUND_UPPER 0x1e
#define SROM_ID 0x1f
#define OBSERVATION 0x3d
#define INVERSE_PRODUCT_ID 0x3f
#define PIXEL_BURST 0x40
#define MOTION_BURST 0x50
#define SROM_LOAD 0x60

#define SEN_IMAGE_DISPLAY_INTERVAL 1

void spi_init(void);
void select(void);
void reselect(void);
void deselect(void);
void send_spi_byte(char c);
char read_spi_byte(void);


char SenReadSensorRegister(char fixedAddress);
void SenSPI_Init(void);
char SenSPI_Transfer(char fixedAddress);
void SenWriteReg(char fixedAddress, char dataValue);
void SenReset(void);