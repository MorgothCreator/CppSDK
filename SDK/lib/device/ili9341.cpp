/*
 * device/ili9341.cpp
 */

#include "ili9341.h"
#include <api/dev_request.h>
#include <interface/spi.h>
#include <api/timer.h>
#include <api/io_handle.h>
#include <interface/screen.h>

/**
 * @brief  Writes register value.
 * @param  None
 * @retval None
 */
void ili9341_WriteData(GI::IO *spi_ctrl, unsigned char RegValue)
{
	GI::IO wrx = GI::IO((char *) "lcd_wrx");
	/* Set WRX to send data */
	wrx.write(true);

	/* Reset LCD control line(/CS) and Send data */
	//ncs.write(false);
	//unsigned long tmp;
	//spi_ctrl->ctl(spi_ctrl->IO_CTL_ASSERT, &tmp);
	spi_ctrl->write(&RegValue, 1);
	//spi_ctrl->ctl(spi_ctrl->IO_CTL_DEASSERT, &tmp);

	/* Deselect: Chip Select high */
	//ncs.write(true);
}

/**
 * @brief  Writes register address.
 * @param  None
 * @retval None
 */
void ili9341_WriteReg(GI::IO *spi_ctrl, unsigned char Reg)
{
	GI::IO wrx = GI::IO((char *) "lcd_wrx");
	/* Reset WRX to send command */
	wrx.write(false);

	/* Reset LCD control line(/CS) and Send command */
	//ncs.write(false);
	//unsigned long tmp;
	//spi_ctrl->ctl(spi_ctrl->IO_CTL_ASSERT, &tmp);
	spi_ctrl->write(&Reg, 1);
	//spi_ctrl->ctl(spi_ctrl->IO_CTL_DEASSERT, &tmp);

	/* Deselect: Chip Select high */
	//ncs.write(true);
}

/**
 * @brief  Reads register value.
 * @param  RegValue Address of the register to read
 * @param  ReadSize Number of bytes to read
 * @retval Content of the register value
 */
unsigned long ili9341_ReadData(GI::IO *spi_ctrl, unsigned char RegValue, unsigned char ReadSize)
{
	GI::IO wrx = GI::IO((char *) "lcd_wrx");
	//GI::IO rdx = GI::IO((char *) "lcd_rdx");
	unsigned char readvalue = 0;

	/* Select: Chip Select low */
	//ncs.write(false);

	/* Reset WRX to send command */
	wrx.write(false);

	ioCtlRwMsgs_t msg;
	ioCtlMsg_t msgs[2];
	msg.nMsgs = 2;
	msg.msgs = msgs;

	msgs[0].buff = &RegValue;
	msgs[0].len = 1;
	msgs[1].buff = &readvalue;
	msgs[1].len = 1;
	spi_ctrl->ctl(GI::IO::IO_CTL_WR, (u32 *)&msg);

	/* Set WRX to send data */
	wrx.write(true);

	/* Deselect: Chip Select high */
	//ncs.write(true);

	return readvalue;
}

/**
 * @brief  Power on the LCD.
 * @param  None
 * @retval None
 */
void ili9341_Init(const char *spiPath, void *_pDisplay)
{
	//GI::Dev::Screen *pDisplay = (GI::Dev::Screen *) _pDisplay;
	GI::IO spi_ctrl = GI::IO((char *) "spi-4.0");
	/* Initialize ILI9341 low level bus layer ----------------------------------*/
	//LCD_IO_Init();
	/* Configure LCD */
	ili9341_WriteReg(&spi_ctrl, 0xCA);
	ili9341_WriteData(&spi_ctrl, 0xC3);
	ili9341_WriteData(&spi_ctrl, 0x08);
	ili9341_WriteData(&spi_ctrl, 0x50);
	ili9341_WriteReg(&spi_ctrl, LCD_POWERB);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0xC1);
	ili9341_WriteData(&spi_ctrl, 0x30);
	ili9341_WriteReg(&spi_ctrl, LCD_POWER_SEQ);
	ili9341_WriteData(&spi_ctrl, 0x64);
	ili9341_WriteData(&spi_ctrl, 0x03);
	ili9341_WriteData(&spi_ctrl, 0x12);
	ili9341_WriteData(&spi_ctrl, 0x81);
	ili9341_WriteReg(&spi_ctrl, LCD_DTCA);
	ili9341_WriteData(&spi_ctrl, 0x85);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x78);
	ili9341_WriteReg(&spi_ctrl, LCD_POWERA);
	ili9341_WriteData(&spi_ctrl, 0x39);
	ili9341_WriteData(&spi_ctrl, 0x2C);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x34);
	ili9341_WriteData(&spi_ctrl, 0x02);
	ili9341_WriteReg(&spi_ctrl, LCD_PRC);
	ili9341_WriteData(&spi_ctrl, 0x20);
	ili9341_WriteReg(&spi_ctrl, LCD_DTCB);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteReg(&spi_ctrl, LCD_FRMCTR1);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x1B);
	ili9341_WriteReg(&spi_ctrl, LCD_DFC);
	ili9341_WriteData(&spi_ctrl, 0x0A);
	ili9341_WriteData(&spi_ctrl, 0xA2);
	ili9341_WriteReg(&spi_ctrl, LCD_POWER1);
	ili9341_WriteData(&spi_ctrl, 0x10);
	ili9341_WriteReg(&spi_ctrl, LCD_POWER2);
	ili9341_WriteData(&spi_ctrl, 0x10);
	ili9341_WriteReg(&spi_ctrl, LCD_VCOM1);
	ili9341_WriteData(&spi_ctrl, 0x45);
	ili9341_WriteData(&spi_ctrl, 0x15);
	ili9341_WriteReg(&spi_ctrl, LCD_VCOM2);
	ili9341_WriteData(&spi_ctrl, 0x90);
	ili9341_WriteReg(&spi_ctrl, LCD_MAC);
	ili9341_WriteData(&spi_ctrl, 0xC8);
	ili9341_WriteReg(&spi_ctrl, LCD_3GAMMA_EN);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteReg(&spi_ctrl, LCD_RGB_INTERFACE);
	ili9341_WriteData(&spi_ctrl, 0xC2);
	ili9341_WriteReg(&spi_ctrl, LCD_DFC);
	ili9341_WriteData(&spi_ctrl, 0x0A);
	ili9341_WriteData(&spi_ctrl, 0xA7);
	ili9341_WriteData(&spi_ctrl, 0x27);
	ili9341_WriteData(&spi_ctrl, 0x04);

	/* Colomn address set */
	ili9341_WriteReg(&spi_ctrl, LCD_COLUMN_ADDR);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0xEF);
	/* Page address set */
	ili9341_WriteReg(&spi_ctrl, LCD_PAGE_ADDR);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x01);
	ili9341_WriteData(&spi_ctrl, 0x3F);
	ili9341_WriteReg(&spi_ctrl, LCD_INTERFACE);
	ili9341_WriteData(&spi_ctrl, 0x01);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x06);

	ili9341_WriteReg(&spi_ctrl, LCD_GRAM);
	GI::Sys::Timer::delay(200);

	ili9341_WriteReg(&spi_ctrl, LCD_GAMMA);
	ili9341_WriteData(&spi_ctrl, 0x01);

	ili9341_WriteReg(&spi_ctrl, LCD_PGAMMA);
	ili9341_WriteData(&spi_ctrl, 0x0F);
	ili9341_WriteData(&spi_ctrl, 0x29);
	ili9341_WriteData(&spi_ctrl, 0x24);
	ili9341_WriteData(&spi_ctrl, 0x0C);
	ili9341_WriteData(&spi_ctrl, 0x0E);
	ili9341_WriteData(&spi_ctrl, 0x09);
	ili9341_WriteData(&spi_ctrl, 0x4E);
	ili9341_WriteData(&spi_ctrl, 0x78);
	ili9341_WriteData(&spi_ctrl, 0x3C);
	ili9341_WriteData(&spi_ctrl, 0x09);
	ili9341_WriteData(&spi_ctrl, 0x13);
	ili9341_WriteData(&spi_ctrl, 0x05);
	ili9341_WriteData(&spi_ctrl, 0x17);
	ili9341_WriteData(&spi_ctrl, 0x11);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteReg(&spi_ctrl, LCD_NGAMMA);
	ili9341_WriteData(&spi_ctrl, 0x00);
	ili9341_WriteData(&spi_ctrl, 0x16);
	ili9341_WriteData(&spi_ctrl, 0x1B);
	ili9341_WriteData(&spi_ctrl, 0x04);
	ili9341_WriteData(&spi_ctrl, 0x11);
	ili9341_WriteData(&spi_ctrl, 0x07);
	ili9341_WriteData(&spi_ctrl, 0x31);
	ili9341_WriteData(&spi_ctrl, 0x33);
	ili9341_WriteData(&spi_ctrl, 0x42);
	ili9341_WriteData(&spi_ctrl, 0x05);
	ili9341_WriteData(&spi_ctrl, 0x0C);
	ili9341_WriteData(&spi_ctrl, 0x0A);
	ili9341_WriteData(&spi_ctrl, 0x28);
	ili9341_WriteData(&spi_ctrl, 0x2F);
	ili9341_WriteData(&spi_ctrl, 0x0F);

	ili9341_WriteReg(&spi_ctrl, LCD_SLEEP_OUT);
	GI::Sys::Timer::delay(200);
	ili9341_WriteReg(&spi_ctrl, LCD_DISPLAY_ON);
	/* GRAM start writing */
	ili9341_WriteReg(&spi_ctrl, LCD_GRAM);
}

