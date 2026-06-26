#include "soc_AM335x.h"
#include "hw_types.h"
#include "i2c.h"

/*
    Habilita o clock do perifrérico i2c1
    Efetua a multiplexação
    Configura a velocidade de transmissão
*/
void i2c1_init(void){
    /*clock do modulo i2c1 habilitado*/
    HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) = 0x2;

    /*verificano se realmente foi habilitado*/
    while((HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) & 0x3 << 16) != 0x0);

    /*efetuando a multiplexação*/
    HWREG(SOC_CONTROL_REGS + conf_spi0_cs0) = (1 << 5) | (1 << 4) | (0 << 3) | 0x2; // I2C1_SCL
    HWREG(SOC_CONTROL_REGS + conf_spi0_d1) = (1 << 5) | (1 << 4) | (0 << 3) | 0x2;  // I2C1_SDA

    /*configuração do i2c1*/
    HWREG(SOC_I2C_1_REGS + I2C_CON) &= ~( 1 << 15); // garante o deligamento do modulo, para eftuar a configuração

   /* configuração da velocidade do I2C para 100 kbps(velocidade de transmissão de dados) */
    HWREG(SOC_I2C_1_REGS + I2C_PSC) = (48 / 12) - 1; // prescaler: frequência Base (48MHz) / frequência interna (12MHz) - 1
    HWREG(SOC_I2C_1_REGS + I2C_SCLL) = 60 - 7; // tempo em baixa: 60 tiques da onda - 7 de atraso físico do hardware
    HWREG(SOC_I2C_1_REGS + I2C_SCLH) = 60 - 5; // tempo em alta: 60 tiques da onda - 5 de atraso físico do hardware

    // Liga o módulo I2C1  e define a BeagleBone como Master 
    HWREG(SOC_I2C_1_REGS + I2C_CON) |= (1 << 15) | (1 << 10);
}


/*
    Efetua a trnsmissão de N bytes de dados entre o escravo e o mestre
    Retorna 0 (sucesso) ou -1 (erro/timeout).
*/
int i2c1_write(uint8_t slave_addr, uint8_t *buf, uint16_t len){
    uint32_t timeout;

    /*carrega o endereço de escavo no registrador de escravo*/
    HWREG(SOC_I2C_1_REGS + I2C_SA) = slave_addr;

    /*indica que a mensagem tem 'len' bytes*/
    HWREG(SOC_I2C_1_REGS + I2C_CNT) = len;

    /*habilita o modulo, habilita o master mode, habilita a transmissão, efetua a parada do módulo 12c1, efetua a incialização*/
    HWREG(SOC_I2C_1_REGS + I2C_CON) = (1 << 15) | (1 << 10) | (1 << 9) | (1 << 1) | (1 << 0);

    /*envia os bytes um por um */
    for(uint16_t i = 0; i < len;i++){
        timeout = I2C_TIMEOUT;

        /*espera o hardware pedir o próximo byte (XRDY - bit 4)*/
        while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 4)) == 0) && --timeout);
        if (timeout == 0) return -1; // acaba com tudo se a placa travar

        /*carrega os dados no regitrador de dados*/
        HWREG(SOC_I2C_1_REGS + I2C_DATA) = buf[i];

        /* Limpa a flag XRDY escrevendo 1 nela */
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 4);

    }

    /*espera o hardware avisar que terminou de usar os registradores e o barramento (ARDY = 1)*/
    timeout = I2C_TIMEOUT;
    while(((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 2)) == 0 ) && --timeout);
    if (timeout == 0) return -1;

    /*verifica se o escravo não respondeu (NACK)*/
    if (HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 1)) { 
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 1) | (1 << 2);
        return -1;
    }

    /*limpa a flag ARDY escrevendo 1 nela, deixando tudo pronto para o próximo envio*/
    HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 2);

    return 0;
}



/*
    Efetua a recepção de N bytes de dados do escravo para o mestre.
    Retorna 0 (sucesso) ou -1 (erro/timeout).
*/
int i2c1_read(uint8_t slave_addr, uint8_t *buf, uint16_t len){
    uint32_t timeout;

    HWREG(SOC_I2C_1_REGS + I2C_SA)  = slave_addr;
    HWREG(SOC_I2C_1_REGS + I2C_CNT) = len;
    
    /* liga o módulo, liga o mestre, desativa transmissão, atiava o stop, atiava start*/
    HWREG(SOC_I2C_1_REGS + I2C_CON) = (1 << 15) | (1 << 10) | (1 << 1) | (1 << 0);

    for (uint16_t i = 0; i < len; i++) {
        timeout = I2C_TIMEOUT;
        /*espera o hardware avisar que um dado chegou*/
        while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 3)) == 0) && --timeout); 
        if (timeout == 0) return -1;

        /* lê o dado recebido para o nosso vetor */
        buf[i] = (uint8_t)HWREG(SOC_I2C_1_REGS + I2C_DATA);
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 3); /* limpa RRDY */
    }

    /*espera o hardware avisar que terminou de usar os registradores e o barramento (ARDY = 1)*/
    timeout = I2C_TIMEOUT;
    while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 2)) == 0) && --timeout); /* ARDY */
    if (timeout == 0) return -1;

    if (HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 1)) { /* NACK */
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 1) | (1 << 2);
        return -1;
    }

    /*limpa a flag ARDY escrevendo 1 nela, deixando tudo pronto para o próximo envio*/
    HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 2);
    return 0;
}




