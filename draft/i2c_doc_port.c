/**
 * @file i2c.c
 * @brief Implementação do driver do barramento I2C1 para o AM335x (BeagleBone Black).
 *
 * @details Fornece funções para inicialização do hardware, transmissão de múltiplos bytes (escrita)
 * e recepção de múltiplos bytes (leitura) utilizando um mecanismo de polling no modo mestre.
 */

#include "soc_AM335x.h"
#include "hw_types.h"
#include "i2c.h"

/**
 * @brief Inicializa o módulo periférico I2C1.
 *
 * @details Esta função realiza a sequência completa de inicialização do hardware:
 * - Habilita o clock operacional para o módulo I2C1 e aguarda até que esteja ativo.
 * - Configura a multiplexação de pinos (Pinmux) para as linhas SCL e SDA (Rx ativo, pull-up interno habilitado e Modo 2).
 * - Desabilita temporariamente o módulo I2C para modificar os registradores de configuração com segurança.
 * - Configura a velocidade para 100 kbps usando um prescaler (clock do sistema de 48MHz reduzido para 12MHz internos) 
 * e ajusta os tempos de SCL em nível baixo/alto subtraindo os atrasos físicos do hardware.
 * - Reabilita o módulo I2C1 e define a BeagleBone como Mestre do Barramento.
 */
void i2c1_init(void){
    HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) = 0x2;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) & (0x3 << 16)) != 0x0);

    HWREG(SOC_CONTROL_REGS + conf_spi0_cs0) = (1 << 5) | (1 << 4) | (0 << 3) | 0x2; 
    HWREG(SOC_CONTROL_REGS + conf_spi0_d1) = (1 << 5) | (1 << 4) | (0 << 3) | 0x2;  

    HWREG(SOC_I2C_1_REGS + I2C_CON) &= ~(1 << 15);

    HWREG(SOC_I2C_1_REGS + I2C_PSC) = (48 / 12) - 1; 
    HWREG(SOC_I2C_1_REGS + I2C_SCLL) = 60 - 7;      
    HWREG(SOC_I2C_1_REGS + I2C_SCLH) = 60 - 5;      

    HWREG(SOC_I2C_1_REGS + I2C_CON) |= (1 << 15) | (1 << 10);
}

/**
 * @brief Transmite bytes de dados para um dispositivo escravo I2C específico.
 *
 * @details Esta função gerencia o protocolo de transmissão de múltiplos bytes:
 * - Carrega o endereço do periférico alvo no registrador I2C_SA.
 * - Configura o tamanho exato da carga útil (payload) no registrador I2C_CNT.
 * - Emite uma condição de Start com as configurações de Mestre Transmissor e condição de Stop habilitadas.
 * - Itera sobre o buffer, realizando polling na flag XRDY com uma proteção de timeout por software.
 * - Carrega cada byte no registrador de dados de hardware e limpa a flag XRDY.
 * - Aguarda a finalização da transação através da flag ARDY.
 * - Verifica se houve uma condição de Não Reconhecimento (NACK) por parte do escravo, limpa as flags de erro 
 * e retorna o status correspondente.
 *
 * @param slave_addr Endereço I2C de 7 bits do periférico escravo alvo.
 * @param buf        Ponteiro para o buffer de dados que contém os bytes a enviar.
 * @param len        Número total de bytes a transmitir.
 *
 * @return int 0 em caso de sucesso na transmissão; -1 em caso de timeout ou se um NACK for detetado.
 */
int i2c1_write(uint8_t slave_addr, uint8_t *buf, uint16_t len){
    uint32_t timeout;

    HWREG(SOC_I2C_1_REGS + I2C_SA) = slave_addr;
    HWREG(SOC_I2C_1_REGS + I2C_CNT) = len;

    HWREG(SOC_I2C_1_REGS + I2C_CON) = (1 << 15) | (1 << 10) | (1 << 9) | (1 << 1) | (1 << 0);

    for(uint16_t i = 0; i < len; i++){
        timeout = I2C_TIMEOUT;

        while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 4)) == 0) && --timeout);
        if (timeout == 0) return -1; 

        HWREG(SOC_I2C_1_REGS + I2C_DATA) = buf[i];
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 4);
    }

    timeout = I2C_TIMEOUT;
    while(((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 2)) == 0 ) && --timeout);
    if (timeout == 0) return -1;

    if (HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 1)) { 
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 1) | (1 << 2);
        return -1;
    }

    HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 2);

    return 0;
}

/**
 * @brief Recebe bytes de dados de um dispositivo escravo I2C específico.
 *
 * @details Esta função gerencia o protocolo de recepção de múltiplos bytes:
 * - Carrega o endereço do periférico alvo no registrador I2C_SA.
 * - Configura o tamanho esperado da carga útil no registrador I2C_CNT.
 * - Emite uma condição de Start com as configurações de Mestre Receptor e condição de Stop habilitadas.
 * - Itera com base no tamanho esperado, realizando polling na flag RRDY com proteção de timeout por software.
 * - Extrai do registrador de hardware cada byte recebido, armazena-o no buffer local e limpa a flag RRDY.
 * - Aguarda a finalização da transação através da flag ARDY.
 * - Verifica se houve uma condição de Não Reconhecimento (NACK) por parte do escravo, limpa as flags de erro 
 * e retorna o status correspondente.
 *
 * @param slave_addr Endereço I2C de 7 bits do periférico escravo alvo.
 * @param buf        Ponteiro para o buffer local onde os bytes recebidos serão armazenados.
 * @param len        Número total de bytes a ler.
 *
 * @return int 0 em caso de sucesso na recepção; -1 em caso de timeout ou se um NACK for detetado.
 */
int i2c1_read(uint8_t slave_addr, uint8_t *buf, uint16_t len){
    uint32_t timeout;

    HWREG(SOC_I2C_1_REGS + I2C_SA)  = slave_addr;
    HWREG(SOC_I2C_1_REGS + I2C_CNT) = len;
    
    HWREG(SOC_I2C_1_REGS + I2C_CON) = (1 << 15) | (1 << 10) | (1 << 1) | (1 << 0);

    for (uint16_t i = 0; i < len; i++) {
        timeout = I2C_TIMEOUT;
        
        while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 3)) == 0) && --timeout); 
        if (timeout == 0) return -1;

        buf[i] = (uint8_t)HWREG(SOC_I2C_1_REGS + I2C_DATA);
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 3); 
    }

    timeout = I2C_TIMEOUT;
    while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 2)) == 0) && --timeout);
    if (timeout == 0) return -1;

    if (HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 1)) { 
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 1) | (1 << 2);
        return -1;
    }

    HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 2);
    
    return 0;
}