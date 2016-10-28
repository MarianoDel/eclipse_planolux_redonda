/*
 * gps_vktel.c
 *
 *  Created on: 27/10/2016
 *      Author: Mariano
 */

#include "gps_vktel.h"
#include "main_menu.h"
#include "uart.h"


//--- Externals variables ---//
extern volatile unsigned char gps_mini_timeout;
extern volatile unsigned char gps_pckt_ready;
extern volatile unsigned char gps_have_data;
extern unsigned char gps_pckt_bytes;
extern unsigned char gps_buff [];


//--- Private variables ---//
GPSState gps_state = GPS_INIT;

volatile unsigned short gps_timeout;
unsigned char gps_buff [SIZEOF_GPSBUFF];

// ------- String de configuracion del GPS -------
unsigned char str_conf1 [] = One_Output_Ten_Secs_String_Cmd;
//unsigned char str_conf1 [] = One_Output_One_Secs_String_Cmd;
unsigned char str_conf2 [] = Disable_GPRMC_String_Cmd;
unsigned char str_conf3 [] = Disable_GPVTG_String_Cmd;
unsigned char str_conf4 [] = Disable_GPGGA_String_Cmd;
//unsigned char str_conf5 [] = Disable_GPGSA_String_Cmd;
unsigned char str_conf5 [] = Enable_GPGSA_String_Cmd;
unsigned char str_conf6 [] = Disable_GPGSV_String_Cmd;

//unsigned char str_conf5 [] = Disable_GPDTM_String_Cmd;
//unsigned char str_conf6 [] = Disable_GPGBS_String_Cmd;
//unsigned char str_conf7 [] = Disable_GPGGA_String_Cmd;
unsigned char str_factory [] = Set_Factory_String_Cmd;
unsigned char str_save [] = Save_Changes_String_Cmd;


//Inicio del GPS
//contesta RESP_CONTNUE o RESP_OK
unsigned char GPSStart(void)
{
	unsigned char resp = RESP_CONTINUE;

	switch (gps_state)
	{
		case GPS_INIT:
			GPS_PIN_DISA;
			gps_timeout = 100;
			gps_state++;
			break;

		case GPS_INIT2:
			if (!gps_timeout)
			{
				GPS_PIN_ENA;
				gps_timeout = 300;	//300ms startup
				gps_state++;
			}
			break;

		case GPS_INIT3:
			if (!gps_timeout)
			{
				Usart1SendSingle(0xFF);
				gps_timeout = 2;
				gps_state++;
			}
			break;

		case GPS_INIT4:
			if (!gps_timeout)
			{
				resp = RESP_OK;
				gps_state = GPS_INIT;
			}
			break;

		default:
			gps_state = GPS_INIT;
			break;
	}
	return resp;
}

//Timers internos para el GPS
void GPSTimeoutCounters (void)
{
	if (gps_timeout)
		gps_timeout--;

	if (gps_mini_timeout)
		gps_mini_timeout--;

}

void GPSProcess (void)
{
	if ((gps_have_data) && (!gps_mini_timeout))
	{
		gps_have_data = 0;
		gps_pckt_ready = 1;
		gps_pckt_bytes = ReadUsart1Buffer(gps_buff, SIZEOF_GPSBUFF);

	}
}

void GPSConfigResetSM(void)
{
	gps_state = GPS_INIT;
}

//Configuracion inicial del GPS
//contesta RESP_CONTNUE o RESP_OK
unsigned char GPSConfig(void)
{
	unsigned char resp = RESP_CONTINUE;

	switch (gps_state)
	{
		case GPS_INIT:
			Usart1SendUnsigned(str_conf1, sizeof(str_conf1));
			gps_timeout = 100;
			gps_state++;
			break;

		case GPS_INIT2:
			if (!gps_timeout)
			{
				Usart1SendUnsigned(str_conf2, sizeof(str_conf2));
				gps_timeout = 100;
				gps_state++;
			}
			break;

		case GPS_INIT3:
			if (!gps_timeout)
			{
				Usart1SendUnsigned(str_conf3, sizeof(str_conf3));
				gps_timeout = 100;
				gps_state++;
			}
			break;

		case GPS_INIT4:
			if (!gps_timeout)
			{
				Usart1SendUnsigned(str_conf4, sizeof(str_conf4));
				gps_timeout = 100;
				gps_state++;
			}
			break;

		case GPS_INIT5:
			if (!gps_timeout)
			{
				Usart1SendUnsigned(str_conf5, sizeof(str_conf5));
				gps_timeout = 100;
				gps_state++;
			}
			break;

		case GPS_INIT6:
			if (!gps_timeout)
			{
				Usart1SendUnsigned(str_conf6, sizeof(str_conf6));
				gps_timeout = 100;
				gps_state = GPS_SAVE;
			}
			break;

		case GPS_SAVE:
			if (!gps_timeout)
			{
				Usart1SendUnsigned(str_save, sizeof(str_save));
//				gps_timeout = 100;
//				gps_state++;
				resp = RESP_OK;
				gps_state = GPS_INIT;
			}
			break;

		default:
			gps_state = GPS_INIT;
			break;
	}

	return resp;
}

//Configuracion de Factoty Default GPS
//contesta RESP_CONTNUE o RESP_OK
unsigned char GPSResetFactory(void)
{
	unsigned char resp = RESP_CONTINUE;

	switch (gps_state)
	{
		case GPS_INIT:
			Usart1SendUnsigned(str_factory, sizeof(str_factory));
			gps_timeout = 100;
			gps_state = GPS_SAVE;
			break;

		case GPS_SAVE:
			if (!gps_timeout)
			{
				Usart1SendUnsigned(str_save, sizeof(str_save));
//				gps_timeout = 100;
//				gps_state++;
				resp = RESP_OK;
				gps_state = GPS_INIT;
			}
			break;

		default:
			gps_state = GPS_INIT;
			break;
	}

	return resp;
}

