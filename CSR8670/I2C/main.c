#include <stdio.h>
#include <i2c.h>

#define I2C_BASE_ADDR 0xA0

static int8 AT24C32_I2C_write(uint8 addr, uint16 reg, uint8 value);
static int8 AT24C32_I2C_read(uint8 addr, uint16 reg, uint8 *p_value);

int main(void)
{
    uint8 temp;
    int32 a = 0;
    AT24C32_I2C_write(I2C_BASE_ADDR, 0x01, 12);
    while (a < 10000000)
    {
        a = a + 1;
    }
    AT24C32_I2C_read(I2C_BASE_ADDR, 0x01, &temp);
    printf("read:%d\n", temp);
    return 0;
}

static int8 AT24C32_I2C_write(uint8 addr, uint16 reg, uint8 value)
{
    int8 nResult = 0;
    uint8 i2c_reg[3];
    i2c_reg[0] = reg >> 8;
    i2c_reg[1] = reg & 0x00FF;
    i2c_reg[2] = value;
#ifdef I2C_DEBUG
    printf("%s R[%x] D[%x]\n", __func__, reg, value);
#endif

    if (I2cTransfer(addr, i2c_reg, sizeof(i2c_reg), 0, 0))
    {
        nResult = 0;
        printf("write successed\n");
    }
    else
    {
        printf("%s[0x%x] Error %d\n", __func__, addr, nResult);
        nResult = -1;
    }

    return nResult;
}

/** 
 * AT24C32 use 2 bytes to indicate address*/
static int8 AT24C32_I2C_read(uint8 addr, uint16 reg, uint8 *p_value)
{
    int8 nResult = 0;
    uint8 rev_data = 0;
    uint8 i2c_reg[2];
    i2c_reg[0] = reg >> 8;
    i2c_reg[1] = reg & 0x00FF;
    /* write address*/
    nResult = I2cTransfer(addr, i2c_reg, sizeof(i2c_reg), 0, 0);

#ifdef I2C_DEBUG
    printf("%s R[%x] D[%x]\n", __func__, i2c_reg, rev_data);
#endif

    if (nResult)
    {
        /* read data*/
        if (I2cTransfer(addr+1, 0, 0, &rev_data, 1))
        {
            *p_value = rev_data;
            printf("read successed\n");
            return 0;
        }
        else
        {
            printf("Read One Data Fail...\n");
            return -1;
        }
    }
    else
    {
        printf("Write Address Fail...\n");
        return -1;
    }
}
