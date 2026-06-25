#ifndef _I2C_MODULE_
#define _I2C_MODULE_
#include <stdint.h>

/*offset de clock do i2c1*/
#define CM_PER_I2C1_CLKCTRL 0x48

/*offsets de multiplexação (CONTROL_MOULE)*/
#define conf_spi0_cs0 0x95C // p9_17 -> I2C1_SCL
#define conf_spi0_d1 0x958 // p9_18  -> I2C1_SDA

/*offsets de configuração do i2c1*/
#define I2C_CON 0xA4 // registrador de configuração i2c1
#define I2C_PSC 0xB0 // necessario para criar um clock interno do 12c, ao dividir o clock funcional da placa
#define I2C_SCLL 0xB4 // Controla por quanto tempo o sinal elétrico vai ficar em baixa
#define I2C_SCLH 0xB8 // Controla por quanto tempo o sinal vai ficar em alta
#define I2C_SA 0xAC // endereço do escravo
#define I2C_DATA 0x9C // endereço de dados
#define I2C_CNT 0x98 // conta bytes de tranmissaão
#define I2C_IRQSTATUS_RAW 0x24 // regitrador de status do i2c1

/*tempo máximo de espera para não travar o processador*/
#define I2C_TIMEOUT 100000UL


/*escopo de funções*/
void i2c1_init(void);
int i2c1_write(uint8_t slave_addr, uint8_t *buf, uint16_t len);
int i2c1_read(uint8_t slave_addr, uint8_t *buf, uint16_t len);



#endif