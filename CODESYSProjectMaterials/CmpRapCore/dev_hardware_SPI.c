/*****************************************************************************
* | File        :   dev_hardware_SPI.c
* | Author      :   Waveshare team
* | Function    :   Read and write /dev/SPI,  hardware SPI
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-26
* | Info        :   Basic version
*
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "dev_hardware_SPI.h"


#include <stdlib.h>
#include <stdio.h>

#include <stdint.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <getopt.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <linux/types.h> 
#include <linux/spi/spidev.h> 

HARDWARE_SPI hardware_SPI;

static uint8_t bits = 8; 

#define SPI_CS_HIGH     0x04                //Chip select high  
#define SPI_LSB_FIRST   0x08                //LSB  
#define SPI_3WIRE       0x10                //3-wire mode SI and SO same line
#define SPI_LOOP        0x20                //Loopback mode  
#define SPI_NO_CS       0x40                //A single device occupies one SPI bus, so there is no chip select 
#define SPI_READY       0x80                //Slave pull low to stop data transmission  

/******************************************************************************
function:   Serial port initialization
parameter:
    SPI_device : Device name
Info:
    /dev/ttyTHS*
    The default baud rate is 9600, 8-bit data, 1 stop bit, no parity
******************************************************************************/
void DEV_HARDWARE_SPI_begin(char *SPI_device)
{
    //device
    uint8_t mode; 
    int ret = 0; 
    if((hardware_SPI.fd = open(SPI_device, O_RDWR )) < 0)  {
        perror("Failed to open SPI device.\n");  
        printf("Failed to open SPI device\r\n");
        exit(1); 
    } else {
        DEV_HARDWARE_SPI_Debug("open : %s\r\n", SPI_device);
    }
    hardware_SPI.mode = 0;
    
    ret = ioctl(hardware_SPI.fd, SPI_IOC_WR_BITS_PER_WORD, &bits);   //写 每个字含多少位 
    if (ret == -1) {
        printf("can't set bits per word\r\n"); 
    }
 
    ret = ioctl(hardware_SPI.fd, SPI_IOC_RD_BITS_PER_WORD, &bits);   //读 每个字含多少位 
    if (ret == -1) {
        printf("can't get bits per word\r\n"); 
    }
    
    DEV_HARDWARE_SPI_Mode(SPI_MODE_0);
    DEV_HARDWARE_SPI_ChipSelect(SPI_CS_Mode_LOW);
    DEV_HARDWARE_SPI_SetBitOrder(SPI_BIT_ORDER_MSBFIRST);
    DEV_HARDWARE_SPI_setSpeed(500000);
    DEV_HARDWARE_SPI_TransferByte(0xaf);
}

void DEV_HARDWARE_SPI_beginSet(char *SPI_device, SPIMode mode, uint32_t speed)
{
    //device
    int ret = 0; 
    hardware_SPI.mode = 0;
    if((hardware_SPI.fd = open(SPI_device, O_RDWR )) < 0)  {
        perror("Failed to open SPI device.\n");  
        exit(1); 
    } else {
        DEV_HARDWARE_SPI_Debug("open : %s\r\n", SPI_device);
    }
    
    ret = ioctl(hardware_SPI.fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1) 
        printf("can't set bits per word\r\n"); 
 
    ret = ioctl(hardware_SPI.fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1) 
        printf("can't get bits per word\r\n"); 

    DEV_HARDWARE_SPI_Mode(mode);
    DEV_HARDWARE_SPI_ChipSelect(SPI_CS_Mode_LOW);
    DEV_HARDWARE_SPI_setSpeed(speed);
}


/******************************************************************************
function:   Serial device End
parameter:
Info:
******************************************************************************/
void DEV_HARDWARE_SPI_end(void)
{
    hardware_SPI.mode = 0;
    if (close(hardware_SPI.fd) != 0){
        DEV_HARDWARE_SPI_Debug("Failed to close SPI device\r\n");
        perror("Failed to close SPI device.\n");  
    }
}

/******************************************************************************
function:   Set SPI speed
parameter:
Info:   Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_HARDWARE_SPI_setSpeed(uint32_t speed)
{
    int status = 1;
    uint32_t speed1 = hardware_SPI.speed;
    
    hardware_SPI.speed = speed;

    //Write speed
    if (ioctl(hardware_SPI.fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        printf("can't set max speed hz\r\n"); 
        hardware_SPI.speed = speed1;//Setting failure rate unchanged
        return -1;
    }
    
    //Read the speed of just writing
    if (ioctl(hardware_SPI.fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1) {
        printf("can't get max speed hz\r\n"); 
        hardware_SPI.speed = speed1;//Setting failure rate unchanged
        return -1;
    }
    hardware_SPI.speed = speed;
    return 1;
}

/******************************************************************************
function:   Set SPI Mode
parameter:
Info:  
    SPIMode:
        SPI_MODE0 
        SPI_MODE1 
        SPI_MODE2 
        SPI_MODE3
    Return :
        Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_HARDWARE_SPI_Mode(SPIMode mode)
{
    uint16_t temp = hardware_SPI.mode;
    hardware_SPI.mode &= 0xfC;//Clear low 2 digits
    hardware_SPI.mode |= mode;//Setting mode
    
    //Write device
    if (ioctl(hardware_SPI.fd, SPI_IOC_WR_MODE, &hardware_SPI.mode) == -1) {
        printf("can't set spi mode\r\n"); 
        return -1;
    }
    return 1;
}

/******************************************************************************
function:   Set SPI CS Enable
parameter:
Info:  
    EN:
        DISABLE 
        ENABLE 
    Return :
        Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_HARDWARE_SPI_CSEN(SPICSEN EN)
{
    if(EN == ENABLE){
        hardware_SPI.mode |= SPI_NO_CS;
    }else {
        hardware_SPI.mode &= ~SPI_NO_CS;
    }
    //Write device
    if (ioctl(hardware_SPI.fd, SPI_IOC_WR_MODE, &hardware_SPI.mode) == -1) {
        printf("can't set spi CS EN\r\n"); 
        return -1;
    }
    return 1;
}

/******************************************************************************
function:   Chip Select
parameter:
Info:  
    CS_Mode:
        SPI_CS_Mode_LOW
        SPI_CS_Mode_HIGH
        SPI_CS_Mode_NONE
    Return :
        Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_HARDWARE_SPI_ChipSelect(SPIChipSelect CS_Mode)
{
    if(CS_Mode == SPI_CS_Mode_HIGH){
        hardware_SPI.mode |= SPI_CS_HIGH;
        hardware_SPI.mode &= ~SPI_NO_CS;
        printf("CS HIGH \r\n");
    }else if(CS_Mode == SPI_CS_Mode_LOW){
        hardware_SPI.mode &= ~SPI_CS_HIGH;
        hardware_SPI.mode &= ~SPI_NO_CS;
    }else if(CS_Mode == SPI_CS_Mode_NONE){
        hardware_SPI.mode |= SPI_NO_CS;
    }
    
    if (ioctl(hardware_SPI.fd, SPI_IOC_WR_MODE, &hardware_SPI.mode) == -1) {
        printf("can't set spi mode\r\n"); 
        return -1;
    }
    return 1;
}

/******************************************************************************
function:   Sets the SPI bit order
parameter:
Info:  
    Order:
        SPI_BIT_ORDER_LSBFIRST
        SPI_BIT_ORDER_MSBFIRST
    Return :
        Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_HARDWARE_SPI_SetBitOrder(SPIBitOrder Order)
{
    if(Order == SPI_BIT_ORDER_LSBFIRST){
        hardware_SPI.mode |= SPI_LSB_FIRST;
        printf("SPI_LSB_FIRST\r\n");
    }else if(Order == SPI_BIT_ORDER_MSBFIRST){
        hardware_SPI.mode &= ~SPI_LSB_FIRST;
        printf("SPI_MSB_FIRST\r\n");
    }
    unsigned char temp = 0x08;
    // printf("hardware_SPI.mode = 0x%02x\r\n", hardware_SPI.mode);
    int fd = ioctl(hardware_SPI.fd, SPI_IOC_WR_MODE, &hardware_SPI.mode);
    printf("fd = %d\r\n",fd);
    if (fd == -1) {
        printf("can't set spi SPI_LSB_FIRST\r\n"); 
        return -1;
    }
    return 1;
}

/******************************************************************************
function:   Sets the SPI Bus Mode
parameter:
Info:  
    Order:
        SPI_3WIRE_Mode
        SPI_4WIRE_Mode
    Return :
        Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_HARDWARE_SPI_SetBusMode(BusMode mode)
{
    if(mode == SPI_3WIRE_Mode){
        hardware_SPI.mode |= SPI_3WIRE;
    }else if(mode == SPI_4WIRE_Mode){
        hardware_SPI.mode &= ~SPI_3WIRE;
    }
    if (ioctl(hardware_SPI.fd, SPI_IOC_WR_MODE, &hardware_SPI.mode) == -1) {
        printf("can't set spi mode\r\n"); 
        return -1;
    }
    return 1;
}

/******************************************************************************
function: 
    Time interval after transmission of one byte during continuous transmission
parameter:
    us :   Interval time (us)
Info:
******************************************************************************/
void DEV_HARDWARE_SPI_SetDataInterval(uint16_t us)
{
    hardware_SPI.delay = us;
}

/******************************************************************************
function: Serial port sends one byte of data
parameter:
    buf :   Sent data
Info:
******************************************************************************/
uint8_t DEV_HARDWARE_SPI_TransferByte(uint8_t buf)
{
    uint8_t sbuf[1] = {buf};
    uint8_t rbuf[1] = {0};
    uint8_t i;
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)sbuf, 
        .rx_buf = (unsigned long)rbuf, 
        .len = 1, 
        .delay_usecs = hardware_SPI.delay, 
        .speed_hz = hardware_SPI.speed, 
        .bits_per_word = bits, 
    }; 
    
    //ioctl Operation, transmission of data
    i = ioctl(hardware_SPI.fd, SPI_IOC_MESSAGE(1), &tr);
    if ( i < 1 )  
        printf("can't send spi message\r\n"); 
    return rbuf[0];
}

/******************************************************************************
function: The serial port reads a byte
parameter:
Info: Return read data
******************************************************************************/
int DEV_HARDWARE_SPI_Transfer(uint8_t *buf, uint32_t len)
{
    uint8_t rbuf[len];
    uint8_t i;
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)buf, 
        .rx_buf = (unsigned long)rbuf, 
        .len = len, 
        .delay_usecs = hardware_SPI.delay, 
        .speed_hz = hardware_SPI.speed, 
        .bits_per_word = bits, 
    }; 
    
    //ioctl Operation, transmission of data
    if ( ioctl(hardware_SPI.fd, SPI_IOC_MESSAGE(len), &tr)  < 1 ){  
        printf("can't send spi message\r\n"); 
        return -1;
    }
    return 1;
}

