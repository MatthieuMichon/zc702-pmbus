#include "xiicps.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xstatus.h"
#include <xil_assert.h>

const u8 I2C_FIRST_ADDR = 0x08;
const u8 I2C_LAST_ADDR = 0x77;
const u8 ZC702_I2C_MUX = 0x74;
const float UCD92XX_VOLTS_PER_LSB = 0.000244; // see SLUU337 section 2.1 

XIicPs XIicPsDev;

s32 check_i2c_addr(u8 i2c_addr);
s32 scan_i2c();

/**
 * @brief Check i2c address
 *
 * Check if a device acknowledges an i2c read command.
 *
 * @param InstancePtr Pointer to the XIicPs instance
 *
 * @param i2c_addr 7-bit i2c address
 * 
 * @return TRUE if i2c address is used, FALSE otherwise
 */
s32
check_i2c_addr(u8 i2c_addr)
{
    u8 dummy = 0xCD;
    u32 status;

    status = XIicPs_MasterSendPolled(&XIicPsDev, &dummy, 0, i2c_addr);
    if (XST_SUCCESS != status) {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Scan i2c bus
 *
 * Scan all 128 addresses on the i2c bus.
 *
 * @return Always TRUE
 */
s32
scan_i2c()
{
    xil_printf("0x-- ");
    for (int i = 0x0; i < 0x10; i++) {
        xil_printf("%x ", i);
    }
    print("\r\n");

    for (u8 i = 0x0; i < 0x80; i++) {
        if (!(i % 0x10))
            xil_printf("0x%x- ", i >> 4);
        if ((i < I2C_FIRST_ADDR) || (i > I2C_LAST_ADDR))
            xil_printf("r "); // reserved address
        else
            check_i2c_addr(i) ? xil_printf("Y ") : xil_printf("? ");
        if (!((i + 1) % 0x10))
            print("\r\n");
    }
    return XST_SUCCESS;
}

/**
 * @brief Read voltage value
 *
 * Read voltage using PMBus commands from the given device / page.
 *
 * @param i2c_addr 7-bit i2c address
 * 
 * @param page PMBus page corresponding to the targeted power rail
 *
 * @return rail voltage in Volts
 */
float
PMBus_readVoltage(u8 i2c_addr, u8 page)
{
    usleep(10000);
    while (XIicPs_BusIsBusy(&XIicPsDev)) {};

    /*
     * Select PMBus Page
     */

        u8 buffer[2];
        buffer[0] = 0x00; // CMD_PAGE
        buffer[1] = page; // data

        XStatus status = XIicPs_MasterSendPolled(&XIicPsDev, buffer, sizeof(buffer), i2c_addr);
        Xil_AssertNonvoid(XST_SUCCESS == status);
        while (XIicPs_BusIsBusy(&XIicPsDev));
        usleep(250000);

    /*
     * Read voltage raw data
     */

        status = XIicPs_SetOptions(&XIicPsDev, XIICPS_REP_START_OPTION);
        Xil_AssertNonvoid(XST_SUCCESS == status);

        buffer[0] = 0x8B; // CMD_READ_VOUT
        status = XIicPs_MasterSendPolled(&XIicPsDev, buffer, 1, i2c_addr);
        usleep(10000);

        status = XIicPs_MasterRecvPolled(&XIicPsDev, buffer, 2, i2c_addr);
        if (XST_SUCCESS != status) {
            status = XIicPs_ReadReg(XIicPsDev.Config.BaseAddress, XIICPS_ISR_OFFSET);
            xil_printf("ERROR: RX error: 0x%08X\r\n", status);
        }

        status = XIicPs_ClearOptions(&XIicPsDev, XIICPS_REP_START_OPTION);
        Xil_AssertNonvoid(XST_SUCCESS == status);

    /*
     * Convert voltage data
     */

        float voltage = UCD92XX_VOLTS_PER_LSB * (float) ((u16)(buffer[1] << 8 | buffer[0]));

    return voltage;

}

/**
 * @brief Scan i2c bus
 *
 * Read voltage using PMBus commands from the given device / page.
 *
 * @param i2c_addr 7-bit i2c address
 * 
 * @param page PMBus page corresponding to the targeted power rail
 *
 * @return Always TRUE
 */
XStatus
PMBus_Report()
{
    float v = PMBus_readVoltage(0x34, 0);
    printf("V: %f\r\n", v);
    return XST_SUCCESS;
}

/**
 * @brief Main entrypoint
 *
 * @return Always zero
 */
int
main ()
{
    print("\r\n");
    print("hello\r\n");

	XStatus status;

	/*
	 * Initialize the i2c driver so that it's ready to use
	 * Look up the configuration in the config table, then initialize it.
	 */

    	XIicPs_Config *XIicPsCfgPtr;

        XIicPsCfgPtr = XIicPs_LookupConfig(XPAR_XIICPS_0_BASEADDR);
        if (NULL == XIicPsCfgPtr) {
            return XST_FAILURE;
        }

        status = XIicPs_CfgInitialize(&XIicPsDev, XIicPsCfgPtr, XIicPsCfgPtr->BaseAddress);
        if (XST_SUCCESS != status) {
            return XST_FAILURE;
        }

        status = XIicPs_SelfTest(&XIicPsDev);
        if (XST_SUCCESS != status) {
            return XST_FAILURE;
        }

        XIicPs_Reset(&XIicPsDev);

        /* Increase the default timeout value to handle clock stretching from the PMBus controllers */
        XIicPs_WriteReg(XIicPsCfgPtr->BaseAddress, XIICPS_TIME_OUT_OFFSET, 0x0000007f);

        XIicPs_SetSClk(&XIicPsDev, 100000);
        if (XST_SUCCESS != status) {
            return XST_FAILURE;
        }

    /*
     * Scan i2c bus 
     */
#if 0
        for (u8 ch = 0; ch < 8; ch++) {
            xil_printf("# Scanning i2c channel %d\r\n", ch);
            const u8 mux_ch = 1 << ch;
            status = XIicPs_MasterSendPolled(&XIicPsDev, &mux_ch, 1, ZC702_I2C_MUX);
            if (XST_SUCCESS != status) {
                return XST_FAILURE;
            }
        scan_i2c();
        }
#endif
    /*
     * Query PM-BUS data
     */

        const u8 mux_ch = 1 << 7;
        status = XIicPs_MasterSendPolled(&XIicPsDev, &mux_ch, 1, ZC702_I2C_MUX);
        if (XST_SUCCESS != status) {
            return XST_FAILURE;
        }

        PMBus_Report();

    print("\r\n");

    return 0;
}
