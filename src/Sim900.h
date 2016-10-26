#ifndef _SIM900_H_
#define _SIM900_H_

#define buffUARTGSMrx_dimension 512

//--- NRESET ---//
//--- PC0 ---//
#define GSM_NRESET (GPIOC->ODR & 0x0001)
#define GSM_NRESET_ON	GPIOC->BSRR = 0x00000001
#define GSM_NRESET_OFF 	GPIOC->BSRR = 0x00010000

//--- POWER KEY ---//
//--- PA7 ---//
#define GSM_PWRKEY (GPIOA->ODR & 0x0080)
#define GSM_PWRKEY_ON	GPIOA->BSRR = 0x00000080
#define GSM_PWRKEY_OFF 	GPIOA->BSRR = 0x00800000

//--- STATUS ---//
//--- PC4 ---//
#define GSM_STATUS (GPIOC->IDR & 0x0010)

//--- NETLIGHT ---//
//--- PC5 ---//
#define GSM_NETLIGHT (GPIOC->IDR & 0x0020)

void GSMConfig (void);
char GSM_Start(void);
void GSM_Stop(void);
void GSMReceive (unsigned char * pAlertasReportar, char * puserCode, unsigned char * pclaveAct, unsigned char * pActDact);
char GSMSendCommand (char *ptrCommand, unsigned char timeOut, unsigned char rta,char *ptrRta);
char GSM_Config(unsigned char timeOut);
char GSMSendSMS (char *ptrMSG, char *ptrNUM, unsigned char timeOut, char sim);
char GSMConfigGPRS (char sim, char *ptrAPN, char *ptrUSER, char *ptrKEY , char *ptrIPAdd, char *ptrIPremote, char *ptrPORTremote,unsigned char timeOut);
char GSM_SetSIM (unsigned char sim);
char GSMSendIP (char *ptrMSG, unsigned char timeOut);
void UARTGSM_Config(void);
void UARTGSMSend(char * ptrSend);
void GSM_TIM6 (void);
//void GSMPrestador(unsigned char * prestadorSim1, unsigned char * prestadorSim2);
void GSMPrestador(unsigned char * pGSMHWstatus, unsigned char * prestadorSim1, unsigned char * prestadorSim2, char * pCONFIGURACIONgprsAPN1, char * pCONFIGURACIONgprsUsuario1, char * pCONFIGURACIONgprsClave1, char * pCONFIGURACIONgprsProveedor1, char * pCONFIGURACIONgprsAPN2, char * pCONFIGURACIONgprsUsuario2, char * pCONFIGURACIONgprsClave2, char * pCONFIGURACIONgprsProveedor2, char * pCONFIGURACIONIPREMOTE, char * pCONFIGURACIONPORTREMOTE);
void GSMrxSMS(unsigned char * pAlertasReportar, char * puserCode, unsigned char * pclaveAct, unsigned char * pActDact, char * pGSMReadSMStel);
char GSMCloseIP(void);
char GSMConfigPDPGPRS (char sim, char *ptrAPN, char *ptrUSER, char *ptrKEY , char *ptrIPAdd, char *ptrIPremote, char *ptrPORTremote,unsigned char timeOut);

#endif
