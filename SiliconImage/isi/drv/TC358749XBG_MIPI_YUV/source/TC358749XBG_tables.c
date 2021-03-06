#include <ebase/types.h>
#include <ebase/trace.h>
#include <ebase/builtins.h>

#include <common/return_codes.h>

#include "isi.h"
#include "isi_iss.h"
#include "isi_priv.h"
#include "TC358749XBG_priv.h"


/*****************************************************************************
 * DEFINES
 *****************************************************************************/


/*****************************************************************************
 * GLOBALS
 *****************************************************************************/

// Image sensor register settings default values taken from data sheet OV8810_DS_1.1_SiliconImage.pdf.
// The settings may be altered by the code in IsiSetupSensor.
const IsiRegDescription_t TC358749XBG_g_aRegDescription[] =
{
    //<!-- Software Reset -->
{0x0004,0x0400,"0x0100",eReadWrite_16},
{0x0002,0x807F,"0x0100",eReadWrite_16},
{0x0002,0x0000,"0x0100",eReadWrite_16},
    //<!-- PLL Setting -->
{0x0020,0x5B30,"0x0100",eReadWrite_16},
{0x0022,0x0302,"0x0100",eReadWrite_16},
    {0x0 ,0x1,"0x0100",eDelay},  
{0x0022,0x1302,"0x0100",eReadWrite_16},
    //<!-- FIFO Delay Setting -->
{0x0006,0xF401,"0x0100",eReadWrite_16},
    //<!-- Misc Setting -->
{0x0060,0x0100,"0x0100",eReadWrite_16},
{0x7080,0x0000,"0x0100",eReadWrite_16},
    //<!-- Interrupt Control -->
{0x0014,0x0000,"0x0100",eReadWrite_16},
{0x0016,0xFF05,"0x0100",eReadWrite_16},
    //<!-- CSI Lane Enable -->
{0x0140,0x00000000,"0x0100",eReadWrite_32},
{0x0144,0x00000000,"0x0100",eReadWrite_32},
{0x0148,0x00000000,"0x0100",eReadWrite_32},
{0x014C,0x00000000,"0x0100",eReadWrite_32},
{0x0150,0x00000000,"0x0100",eReadWrite_32},
    //<!-- CSI Transition Timing -->
{0x0210,0x70170000,"0x0100",eReadWrite_32},
{0x0214,0x05000000,"0x0100",eReadWrite_32},
{0x0218,0x05260000,"0x0100",eReadWrite_32},
{0x021C,0x02000000,"0x0100",eReadWrite_32},
{0x0220,0x05050000,"0x0100",eReadWrite_32},
{0x0224,0x68420000,"0x0100",eReadWrite_32},
{0x0228,0x09000000,"0x0100",eReadWrite_32},
{0x022C,0x03000000,"0x0100",eReadWrite_32},
{0x0230,0x05000000,"0x0100",eReadWrite_32},
{0x0234,0x1F000000,"0x0100",eReadWrite_32},
{0x0238,0x00000000,"0x0100",eReadWrite_32},
{0x0204,0x01000000,"0x0100",eReadWrite_32},
{0x0518,0x01000000,"0x0100",eReadWrite_32},
{0x0500,0x868000A3,"0x0100",eReadWrite_32},
    //<!-- Data ID Setting -->
{0x0012,0x0200,"0x0100",eReadWrite_16},
    //<!-- HDMI Interrupt Mask -->
{0x8502,0x01,"0x0100",eReadWrite},
{0x8512,0xFE,"0x0100",eReadWrite},
{0x8514,0x00,"0x0100",eReadWrite},
{0x8515,0x00,"0x0100",eReadWrite},
{0x8516,0x00,"0x0100",eReadWrite},
    //<!-- HDMI Audio REFCLK -->
{0x8531,0x01,"0x0100",eReadWrite},
{0x8540,0x8C,"0x0100",eReadWrite},
{0x8541,0x0A,"0x0100",eReadWrite},
{0x8630,0xB0,"0x0100",eReadWrite},
{0x8631,0x1E,"0x0100",eReadWrite},
{0x8632,0x04,"0x0100",eReadWrite},
{0x8670,0x01,"0x0100",eReadWrite},
    //<!-- HDMI PHY -->
{0x8532,0x80,"0x0100",eReadWrite},
{0x8536,0x40,"0x0100",eReadWrite},
{0x853F,0x0A,"0x0100",eReadWrite},
    //<!-- HDMI SYSTEM -->
{0x8543,0x32,"0x0100",eReadWrite},
{0x8544,0x10,"0x0100",eReadWrite},
{0x8545,0x31,"0x0100",eReadWrite},
{0x8546,0x2D,"0x0100",eReadWrite},
    //<!-- EDID -->
{0x85C7,0x01,"0x0100",eReadWrite},
{0x85CA,0x00,"0x0100",eReadWrite},
{0x85CB,0x01,"0x0100",eReadWrite},
    //<!-- EDID Data -->
{0x8C00,0x00,"0x0100",eReadWrite},
{0x8C01,0xFF,"0x0100",eReadWrite},
{0x8C02,0xFF,"0x0100",eReadWrite},
{0x8C03,0xFF,"0x0100",eReadWrite},
{0x8C04,0xFF,"0x0100",eReadWrite},
{0x8C05,0xFF,"0x0100",eReadWrite},
{0x8C06,0xFF,"0x0100",eReadWrite},
{0x8C07,0x00,"0x0100",eReadWrite},
{0x8C08,0x52,"0x0100",eReadWrite},
{0x8C09,0x62,"0x0100",eReadWrite},
{0x8C0A,0x88,"0x0100",eReadWrite},
{0x8C0B,0x88,"0x0100",eReadWrite},
{0x8C0C,0x00,"0x0100",eReadWrite},
{0x8C0D,0x88,"0x0100",eReadWrite},
{0x8C0E,0x88,"0x0100",eReadWrite},
{0x8C0F,0x88,"0x0100",eReadWrite},
{0x8C10,0x1C,"0x0100",eReadWrite},
{0x8C11,0x15,"0x0100",eReadWrite},
{0x8C12,0x01,"0x0100",eReadWrite},
{0x8C13,0x03,"0x0100",eReadWrite},
{0x8C14,0x80,"0x0100",eReadWrite},
{0x8C15,0x00,"0x0100",eReadWrite},
{0x8C16,0x00,"0x0100",eReadWrite},
{0x8C17,0x78,"0x0100",eReadWrite},
{0x8C18,0x0A,"0x0100",eReadWrite},
{0x8C19,0x0D,"0x0100",eReadWrite},
{0x8C1A,0xC9,"0x0100",eReadWrite},
{0x8C1B,0xA0,"0x0100",eReadWrite},
{0x8C1C,0x57,"0x0100",eReadWrite},
{0x8C1D,0x47,"0x0100",eReadWrite},
{0x8C1E,0x98,"0x0100",eReadWrite},
{0x8C1F,0x27,"0x0100",eReadWrite},
{0x8C20,0x12,"0x0100",eReadWrite},
{0x8C21,0x48,"0x0100",eReadWrite},
{0x8C22,0x4C,"0x0100",eReadWrite},
{0x8C23,0x00,"0x0100",eReadWrite},
{0x8C24,0x00,"0x0100",eReadWrite},
{0x8C25,0x00,"0x0100",eReadWrite},
{0x8C26,0x01,"0x0100",eReadWrite},
{0x8C27,0x01,"0x0100",eReadWrite},
{0x8C28,0x01,"0x0100",eReadWrite},
{0x8C29,0x01,"0x0100",eReadWrite},
{0x8C2A,0x01,"0x0100",eReadWrite},
{0x8C2B,0x01,"0x0100",eReadWrite},
{0x8C2C,0x01,"0x0100",eReadWrite},
{0x8C2D,0x01,"0x0100",eReadWrite},
{0x8C2E,0x01,"0x0100",eReadWrite},
{0x8C2F,0x01,"0x0100",eReadWrite},
{0x8C30,0x01,"0x0100",eReadWrite},
{0x8C31,0x01,"0x0100",eReadWrite},
{0x8C32,0x01,"0x0100",eReadWrite},
{0x8C33,0x01,"0x0100",eReadWrite},
{0x8C34,0x01,"0x0100",eReadWrite},
{0x8C35,0x01,"0x0100",eReadWrite},
{0x8C36,0x02,"0x0100",eReadWrite},
{0x8C37,0x3A,"0x0100",eReadWrite},
{0x8C38,0x80,"0x0100",eReadWrite},
{0x8C39,0x18,"0x0100",eReadWrite},
{0x8C3A,0x71,"0x0100",eReadWrite},
{0x8C3B,0x38,"0x0100",eReadWrite},
{0x8C3C,0x2D,"0x0100",eReadWrite},
{0x8C3D,0x40,"0x0100",eReadWrite},
{0x8C3E,0x58,"0x0100",eReadWrite},
{0x8C3F,0x2C,"0x0100",eReadWrite},
{0x8C40,0x45,"0x0100",eReadWrite},
{0x8C41,0x00,"0x0100",eReadWrite},
{0x8C42,0xC4,"0x0100",eReadWrite},
{0x8C43,0x8E,"0x0100",eReadWrite},
{0x8C44,0x21,"0x0100",eReadWrite},
{0x8C45,0x00,"0x0100",eReadWrite},
{0x8C46,0x00,"0x0100",eReadWrite},
{0x8C47,0x1E,"0x0100",eReadWrite},
{0x8C48,0x01,"0x0100",eReadWrite},
{0x8C49,0x1D,"0x0100",eReadWrite},
{0x8C4A,0x00,"0x0100",eReadWrite},
{0x8C4B,0x72,"0x0100",eReadWrite},
{0x8C4C,0x51,"0x0100",eReadWrite},
{0x8C4D,0xD0,"0x0100",eReadWrite},
{0x8C4E,0x1E,"0x0100",eReadWrite},
{0x8C4F,0x20,"0x0100",eReadWrite},
{0x8C50,0x6E,"0x0100",eReadWrite},
{0x8C51,0x28,"0x0100",eReadWrite},
{0x8C52,0x55,"0x0100",eReadWrite},
{0x8C53,0x00,"0x0100",eReadWrite},
{0x8C54,0xC4,"0x0100",eReadWrite},
{0x8C55,0x8E,"0x0100",eReadWrite},
{0x8C56,0x21,"0x0100",eReadWrite},
{0x8C57,0x00,"0x0100",eReadWrite},
{0x8C58,0x00,"0x0100",eReadWrite},
{0x8C59,0x1E,"0x0100",eReadWrite},
{0x8C5A,0x00,"0x0100",eReadWrite},
{0x8C5B,0x00,"0x0100",eReadWrite},
{0x8C5C,0x00,"0x0100",eReadWrite},
{0x8C5D,0xFC,"0x0100",eReadWrite},
{0x8C5E,0x00,"0x0100",eReadWrite},
{0x8C5F,0x54,"0x0100",eReadWrite},
{0x8C60,0x6F,"0x0100",eReadWrite},
{0x8C61,0x73,"0x0100",eReadWrite},
{0x8C62,0x68,"0x0100",eReadWrite},
{0x8C63,0x69,"0x0100",eReadWrite},
{0x8C64,0x62,"0x0100",eReadWrite},
{0x8C65,0x61,"0x0100",eReadWrite},
{0x8C66,0x2D,"0x0100",eReadWrite},
{0x8C67,0x48,"0x0100",eReadWrite},
{0x8C68,0x32,"0x0100",eReadWrite},
{0x8C69,0x43,"0x0100",eReadWrite},
{0x8C6A,0x50,"0x0100",eReadWrite},
{0x8C6B,0x0A,"0x0100",eReadWrite},
{0x8C6C,0x00,"0x0100",eReadWrite},
{0x8C6D,0x00,"0x0100",eReadWrite},
{0x8C6E,0x00,"0x0100",eReadWrite},
{0x8C6F,0xFD,"0x0100",eReadWrite},
{0x8C70,0x00,"0x0100",eReadWrite},
{0x8C71,0x17,"0x0100",eReadWrite},
{0x8C72,0x3D,"0x0100",eReadWrite},
{0x8C73,0x0F,"0x0100",eReadWrite},
{0x8C74,0x8C,"0x0100",eReadWrite},
{0x8C75,0x17,"0x0100",eReadWrite},
{0x8C76,0x00,"0x0100",eReadWrite},
{0x8C77,0x0A,"0x0100",eReadWrite},
{0x8C78,0x20,"0x0100",eReadWrite},
{0x8C79,0x20,"0x0100",eReadWrite},
{0x8C7A,0x20,"0x0100",eReadWrite},
{0x8C7B,0x20,"0x0100",eReadWrite},
{0x8C7C,0x20,"0x0100",eReadWrite},
{0x8C7D,0x20,"0x0100",eReadWrite},
{0x8C7E,0x01,"0x0100",eReadWrite},
{0x8C7F,0x63,"0x0100",eReadWrite},
{0x8C80,0x02,"0x0100",eReadWrite},
{0x8C81,0x03,"0x0100",eReadWrite},
{0x8C82,0x17,"0x0100",eReadWrite},
{0x8C83,0x74,"0x0100",eReadWrite},
{0x8C84,0x47,"0x0100",eReadWrite},
{0x8C85,0x10,"0x0100",eReadWrite},
{0x8C86,0x04,"0x0100",eReadWrite},
{0x8C87,0x05,"0x0100",eReadWrite},
{0x8C88,0x05,"0x0100",eReadWrite},
{0x8C89,0x05,"0x0100",eReadWrite},
{0x8C8A,0x05,"0x0100",eReadWrite},
{0x8C8B,0x05,"0x0100",eReadWrite},
{0x8C8C,0x23,"0x0100",eReadWrite},
{0x8C8D,0x09,"0x0100",eReadWrite},
{0x8C8E,0x07,"0x0100",eReadWrite},
{0x8C8F,0x01,"0x0100",eReadWrite},
{0x8C90,0x66,"0x0100",eReadWrite},
{0x8C91,0x03,"0x0100",eReadWrite},
{0x8C92,0x0C,"0x0100",eReadWrite},
{0x8C93,0x00,"0x0100",eReadWrite},
{0x8C94,0x30,"0x0100",eReadWrite},
{0x8C95,0x00,"0x0100",eReadWrite},
{0x8C96,0x80,"0x0100",eReadWrite},
{0x8C97,0x8C,"0x0100",eReadWrite},
{0x8C98,0x0A,"0x0100",eReadWrite},
{0x8C99,0xD0,"0x0100",eReadWrite},
{0x8C9A,0x01,"0x0100",eReadWrite},
{0x8C9B,0x1D,"0x0100",eReadWrite},
{0x8C9C,0x80,"0x0100",eReadWrite},
{0x8C9D,0x18,"0x0100",eReadWrite},
{0x8C9E,0x71,"0x0100",eReadWrite},
{0x8C9F,0x38,"0x0100",eReadWrite},
{0x8CA0,0x16,"0x0100",eReadWrite},
{0x8CA1,0x40,"0x0100",eReadWrite},
{0x8CA2,0x58,"0x0100",eReadWrite},
{0x8CA3,0x2C,"0x0100",eReadWrite},
{0x8CA4,0x25,"0x0100",eReadWrite},
{0x8CA5,0x00,"0x0100",eReadWrite},
{0x8CA6,0x80,"0x0100",eReadWrite},
{0x8CA7,0x38,"0x0100",eReadWrite},
{0x8CA8,0x74,"0x0100",eReadWrite},
{0x8CA9,0x00,"0x0100",eReadWrite},
{0x8CAA,0x00,"0x0100",eReadWrite},
{0x8CAB,0x18,"0x0100",eReadWrite},
{0x8CAC,0x01,"0x0100",eReadWrite},
{0x8CAD,0x1D,"0x0100",eReadWrite},
{0x8CAE,0x80,"0x0100",eReadWrite},
{0x8CAF,0x18,"0x0100",eReadWrite},
{0x8CB0,0x71,"0x0100",eReadWrite},
{0x8CB1,0x38,"0x0100",eReadWrite},
{0x8CB2,0x16,"0x0100",eReadWrite},
{0x8CB3,0x40,"0x0100",eReadWrite},
{0x8CB4,0x58,"0x0100",eReadWrite},
{0x8CB5,0x2C,"0x0100",eReadWrite},
{0x8CB6,0x25,"0x0100",eReadWrite},
{0x8CB7,0x00,"0x0100",eReadWrite},
{0x8CB8,0x80,"0x0100",eReadWrite},
{0x8CB9,0x38,"0x0100",eReadWrite},
{0x8CBA,0x74,"0x0100",eReadWrite},
{0x8CBB,0x00,"0x0100",eReadWrite},
{0x8CBC,0x00,"0x0100",eReadWrite},
{0x8CBD,0x18,"0x0100",eReadWrite},
{0x8CBE,0x01,"0x0100",eReadWrite},
{0x8CBF,0x1D,"0x0100",eReadWrite},
{0x8CC0,0x80,"0x0100",eReadWrite},
{0x8CC1,0x18,"0x0100",eReadWrite},
{0x8CC2,0x71,"0x0100",eReadWrite},
{0x8CC3,0x38,"0x0100",eReadWrite},
{0x8CC4,0x16,"0x0100",eReadWrite},
{0x8CC5,0x40,"0x0100",eReadWrite},
{0x8CC6,0x58,"0x0100",eReadWrite},
{0x8CC7,0x2C,"0x0100",eReadWrite},
{0x8CC8,0x25,"0x0100",eReadWrite},
{0x8CC9,0x00,"0x0100",eReadWrite},
{0x8CCA,0x80,"0x0100",eReadWrite},
{0x8CCB,0x38,"0x0100",eReadWrite},
{0x8CCC,0x74,"0x0100",eReadWrite},
{0x8CCD,0x00,"0x0100",eReadWrite},
{0x8CCE,0x00,"0x0100",eReadWrite},
{0x8CCF,0x18,"0x0100",eReadWrite},
{0x8CD0,0x01,"0x0100",eReadWrite},
{0x8CD1,0x1D,"0x0100",eReadWrite},
{0x8CD2,0x80,"0x0100",eReadWrite},
{0x8CD3,0x18,"0x0100",eReadWrite},
{0x8CD4,0x71,"0x0100",eReadWrite},
{0x8CD5,0x38,"0x0100",eReadWrite},
{0x8CD6,0x16,"0x0100",eReadWrite},
{0x8CD7,0x40,"0x0100",eReadWrite},
{0x8CD8,0x58,"0x0100",eReadWrite},
{0x8CD9,0x2C,"0x0100",eReadWrite},
{0x8CDA,0x25,"0x0100",eReadWrite},
{0x8CDB,0x00,"0x0100",eReadWrite},
{0x8CDC,0x80,"0x0100",eReadWrite},
{0x8CDD,0x38,"0x0100",eReadWrite},
{0x8CDE,0x74,"0x0100",eReadWrite},
{0x8CDF,0x00,"0x0100",eReadWrite},
{0x8CE0,0x00,"0x0100",eReadWrite},
{0x8CE1,0x18,"0x0100",eReadWrite},
{0x8CE2,0x00,"0x0100",eReadWrite},
{0x8CE3,0x00,"0x0100",eReadWrite},
{0x8CE4,0x00,"0x0100",eReadWrite},
{0x8CE5,0x00,"0x0100",eReadWrite},
{0x8CE6,0x00,"0x0100",eReadWrite},
{0x8CE7,0x00,"0x0100",eReadWrite},
{0x8CE8,0x00,"0x0100",eReadWrite},
{0x8CE9,0x00,"0x0100",eReadWrite},
{0x8CEA,0x00,"0x0100",eReadWrite},
{0x8CEB,0x00,"0x0100",eReadWrite},
{0x8CEC,0x00,"0x0100",eReadWrite},
{0x8CED,0x00,"0x0100",eReadWrite},
{0x8CEE,0x00,"0x0100",eReadWrite},
{0x8CEF,0x00,"0x0100",eReadWrite},
{0x8CF0,0x00,"0x0100",eReadWrite},
{0x8CF1,0x00,"0x0100",eReadWrite},
{0x8CF2,0x00,"0x0100",eReadWrite},
{0x8CF3,0x00,"0x0100",eReadWrite},
{0x8CF4,0x00,"0x0100",eReadWrite},
{0x8CF5,0x00,"0x0100",eReadWrite},
{0x8CF6,0x00,"0x0100",eReadWrite},
{0x8CF7,0x00,"0x0100",eReadWrite},
{0x8CF8,0x00,"0x0100",eReadWrite},
{0x8CF9,0x00,"0x0100",eReadWrite},
{0x8CFA,0x00,"0x0100",eReadWrite},
{0x8CFB,0x00,"0x0100",eReadWrite},
{0x8CFC,0x00,"0x0100",eReadWrite},
{0x8CFD,0x00,"0x0100",eReadWrite},
{0x8CFE,0x00,"0x0100",eReadWrite},
{0x8CFF,0xB5,"0x0100",eReadWrite},
    //<!-- HDCP Setting -->
    //<!-- Video Setting -->
{0x8573,0x81,"0x0100",eReadWrite},
    //<!-- HDMI Audio Setting -->
{0x8600,0x00,"0x0100",eReadWrite},
{0x8602,0xF3,"0x0100",eReadWrite},
{0x8603,0x02,"0x0100",eReadWrite},
{0x8604,0x0C,"0x0100",eReadWrite},
{0x8606,0x05,"0x0100",eReadWrite},
{0x8607,0x00,"0x0100",eReadWrite},
{0x8620,0x2A,"0x0100",eReadWrite},
{0x8621,0x02,"0x0100",eReadWrite},
{0x8640,0x01,"0x0100",eReadWrite},
{0x8641,0x65,"0x0100",eReadWrite},
{0x8642,0x07,"0x0100",eReadWrite},
{0x8652,0x02,"0x0100",eReadWrite},
{0x8665,0x10,"0x0100",eReadWrite},
    //<!-- Info Frame Extraction -->
{0x8709,0xFF,"0x0100",eReadWrite},
{0x870B,0x2C,"0x0100",eReadWrite},
{0x870C,0x53,"0x0100",eReadWrite},
{0x870D,0x01,"0x0100",eReadWrite},
{0x870E,0x30,"0x0100",eReadWrite},
{0x9007,0x10,"0x0100",eReadWrite},
{0x854A,0x01,"0x0100",eReadWrite},
{0x0004,0xD70C,"0x0100",eReadWrite_16},	
{0x0000 ,0x00,"eTableEnd",eTableEnd}
};

const IsiRegDescription_t TC358749XBG_g_aRegVedioON[] = 
{
	//------------------------------------
	//Let HDMI Source start access
	{0x854A ,0x00,"0x0100",eReadWrite},//01
	//Let HDMI Source start access
	{0x854A ,0x00,"0x0100",eReadWrite},//01

	{0x0 ,0x1,"0x0100",eDelay},	//eDelay 10us  shiji:1ms

	//Wait until HDMI sync is established 
	{0x8520 ,0x00,"0x0100",eReadOnly}, //CLK_Status

	{0x0004 ,0x0400,"0x0100",eReadWrite_16},  //ConfCtl 0CD7
	//---------------------------
	{0x0000 ,0x00,"eTableEnd",eTableEnd}
};

const IsiRegDescription_t TC358749XBG_g_hdmi_input_check[] =
{
    //------HDMI input video timing check
	//PCLK
    {0x852E ,0x00,"0x0100",eReadOnly}, //PX_FREQ0
    {0x852F ,0x00,"0x0100",eReadOnly}, //PX_FREQ1
    //Horizontal Related
	{0x858A ,0x00,"0x0100",eReadOnly}, //H_SIZE[7:0]
	{0x858B ,0x00,"0x0100",eReadOnly}, //H_SIZE[12:8]
	{0x8580 ,0x00,"0x0100",eReadOnly}, //DE_HPOS[7:0]
	{0x8581 ,0x00,"0x0100",eReadOnly}, //DE_HPOS[12:8]
	{0x8582 ,0x00,"0x0100",eReadOnly}, //DE_HWID[7:0]
	{0x8583 ,0x00,"0x0100",eReadOnly}, //DE_HWID[12:8]
	//Vertical Related
	{0x858C ,0x00,"0x0100",eReadOnly}, //V_SIZE[7:0]
	{0x858D ,0x00,"0x0100",eReadOnly}, //V_SIZE[12:8]
	{0x8584 ,0x00,"0x0100",eReadOnly}, //DE_VPOS_A[7:0]
	{0x8585 ,0x00,"0x0100",eReadOnly}, //DE_VPOS_A[12:8]
	{0x8586 ,0x00,"0x0100",eReadOnly}, //DE_VPOS_B[7:0]
	{0x8587 ,0x00,"0x0100",eReadOnly}, //DE_VPOS_B[12:8]
	{0x8588 ,0x00,"0x0100",eReadOnly}, //DE_VWID[7:0]
	{0x8589 ,0x00,"0x0100",eReadOnly}, //DE_VWID[12:8]
	//VSYNC,HSYNC Porarity
	{0x8586 ,0x00,"0x0100",eReadOnly}, //CLK_Status
	
    {0x0000 ,0x00,"eTableEnd",eTableEnd}
};

const IsiRegDescription_t TC358749XBG_g_edio[] =
{
   //EDIO
	{0x8C09 ,0x62,"0x0100",eReadWrite},
	{0x8C0A ,0x88,"0x0100",eReadWrite},
	{0x8C0B ,0x88,"0x0100",eReadWrite},
	{0x8C0C ,0x00,"0x0100",eReadWrite},
	{0x8C0D ,0x88,"0x0100",eReadWrite},
	{0x8C0E ,0x88,"0x0100",eReadWrite},
	{0x8C0F ,0x88,"0x0100",eReadWrite},

	{0x8C10 ,0x1C,"0x0100",eReadWrite},
	{0x8C11 ,0x15,"0x0100",eReadWrite},

	{0x8C36 ,0x02,"0x0100",eReadWrite},
	{0x8C37 ,0x3A,"0x0100",eReadWrite},
	{0x8C38 ,0x80,"0x0100",eReadWrite},
	{0x8C39 ,0x18,"0x0100",eReadWrite},
	{0x8C3A ,0x71,"0x0100",eReadWrite},
	{0x8C3B ,0x38,"0x0100",eReadWrite},
	{0x8C3C ,0x2D,"0x0100",eReadWrite},
	{0x8C3D ,0x40,"0x0100",eReadWrite},
	{0x8C3E ,0x58,"0x0100",eReadWrite},
	{0x8C3F ,0x2C,"0x0100",eReadWrite},

	{0x8C40 ,0x45,"0x0100",eReadWrite},
	{0x8C41 ,0x00,"0x0100",eReadWrite},
	{0x8C42 ,0x80,"0x0100",eReadWrite},
	{0x8C43 ,0x38,"0x0100",eReadWrite},
	{0x8C44 ,0x74,"0x0100",eReadWrite},
	{0x8C48 ,0x02,"0x0100",eReadWrite},
	{0x8C49 ,0x34,"0x0100",eReadWrite},
	{0x8C4A ,0x80,"0x0100",eReadWrite},
	{0x8C4B ,0x18,"0x0100",eReadWrite},
	{0x8C4C ,0x71,"0x0100",eReadWrite},
	{0x8C4D ,0x38,"0x0100",eReadWrite},
	{0x8C4E ,0x2D,"0x0100",eReadWrite},
	{0x8C4F ,0x40,"0x0100",eReadWrite},

	{0x8C50 ,0x58,"0x0100",eReadWrite},
	{0x8C51 ,0x2C,"0x0100",eReadWrite},
	{0x8C52 ,0x45,"0x0100",eReadWrite},
	{0x8C53 ,0x00,"0x0100",eReadWrite},
	{0x8C54 ,0x80,"0x0100",eReadWrite},
	{0x8C55 ,0x38,"0x0100",eReadWrite},
	{0x8C56 ,0x74,"0x0100",eReadWrite},
	{0x8C5E ,0x00,"0x0100",eReadWrite},
	{0x8C5F ,0x54,"0x0100",eReadWrite},

	{0x8C60 ,0x6F,"0x0100",eReadWrite},
	{0x8C61 ,0x73,"0x0100",eReadWrite},
	{0x8C62 ,0x68,"0x0100",eReadWrite},
	{0x8C63 ,0x69,"0x0100",eReadWrite},
	{0x8C64 ,0x62,"0x0100",eReadWrite},
	{0x8C65 ,0x61,"0x0100",eReadWrite},
	{0x8C66 ,0x2D,"0x0100",eReadWrite},
	{0x8C67 ,0x48,"0x0100",eReadWrite},
	{0x8C68 ,0x32,"0x0100",eReadWrite},
	{0x8C69 ,0x44,"0x0100",eReadWrite},
	{0x8C6A ,0x0A,"0x0100",eReadWrite},
	{0x8C6B ,0x20,"0x0100",eReadWrite},

	{0x8C84 ,0x47,"0x0100",eReadWrite},
	{0x8C85 ,0x0A,"0x0100",eReadWrite},
	{0x8C86 ,0x0A,"0x0100",eReadWrite},
	{0x8C87 ,0x0A,"0x0100",eReadWrite},
	{0x8C88 ,0x0A,"0x0100",eReadWrite},
	{0x8C89 ,0x0A,"0x0100",eReadWrite},
	{0x8C8A ,0x0A,"0x0100",eReadWrite},
	{0x8C8B ,0x0A,"0x0100",eReadWrite},

	{0x8C9A ,0x02,"0x0100",eReadWrite},
	{0x8C9B ,0x3A,"0x0100",eReadWrite},
	{0x8C9C ,0x80,"0x0100",eReadWrite},
	{0x8C9D ,0x18,"0x0100",eReadWrite},
	{0x8C9E ,0x71,"0x0100",eReadWrite},
	{0x8C9F ,0x38,"0x0100",eReadWrite},

	{0x8CA0 ,0x2D,"0x0100",eReadWrite},
	{0x8CA1 ,0x40,"0x0100",eReadWrite},
	{0x8CA2 ,0x58,"0x0100",eReadWrite},
	{0x8CA3 ,0x2C,"0x0100",eReadWrite},
	{0x8CA4 ,0x45,"0x0100",eReadWrite},
	{0x8CA5 ,0x00,"0x0100",eReadWrite},
	{0x8CA6 ,0x80,"0x0100",eReadWrite},
	{0x8CA7 ,0x38,"0x0100",eReadWrite},
	{0x8CA8 ,0x74,"0x0100",eReadWrite},
	{0x8CAC ,0x02,"0x0100",eReadWrite},
	{0x8CAD ,0x3A,"0x0100",eReadWrite},
	{0x8CAE ,0x80,"0x0100",eReadWrite},
	{0x8CAF ,0x18,"0x0100",eReadWrite},

	{0x8CB0 ,0x71,"0x0100",eReadWrite},
	{0x8CB1 ,0x38,"0x0100",eReadWrite},
	{0x8CB2 ,0x2D,"0x0100",eReadWrite},
	{0x8CB3 ,0x40,"0x0100",eReadWrite},
	{0x8CB4 ,0x58,"0x0100",eReadWrite},
	{0x8CB5 ,0x2C,"0x0100",eReadWrite},
	{0x8CB6 ,0x45,"0x0100",eReadWrite},
	{0x8CB7 ,0x00,"0x0100",eReadWrite},
	{0x8CB8 ,0x80,"0x0100",eReadWrite},
	{0x8CB9 ,0x38,"0x0100",eReadWrite},
	{0x8CBA ,0x74,"0x0100",eReadWrite},
	{0x8CBE ,0x02,"0x0100",eReadWrite},
	{0x8CBF ,0x3A,"0x0100",eReadWrite},

	{0x8CC0 ,0x80,"0x0100",eReadWrite},
	{0x8CC1 ,0x18,"0x0100",eReadWrite},
	{0x8CC2 ,0x71,"0x0100",eReadWrite},
	{0x8CC3 ,0x38,"0x0100",eReadWrite},
	{0x8CC4 ,0x2D,"0x0100",eReadWrite},
	{0x8CC5 ,0x40,"0x0100",eReadWrite},
	{0x8CC6 ,0x58,"0x0100",eReadWrite},
	{0x8CC7 ,0x2C,"0x0100",eReadWrite},
	{0x8CC8 ,0x45,"0x0100",eReadWrite},
	{0x8CC9 ,0x00,"0x0100",eReadWrite},
	{0x8CCA ,0x80,"0x0100",eReadWrite},
	{0x8CCB ,0x38,"0x0100",eReadWrite},
	{0x8CCC ,0x74,"0x0100",eReadWrite},

	{0x8CD0 ,0x02,"0x0100",eReadWrite},
	{0x8CD1 ,0x3A,"0x0100",eReadWrite},
	{0x8CD2 ,0x80,"0x0100",eReadWrite},
	{0x8CD3 ,0x18,"0x0100",eReadWrite},
	{0x8CD4 ,0x71,"0x0100",eReadWrite},
	{0x8CD5 ,0x38,"0x0100",eReadWrite},
	{0x8CD6 ,0x2D,"0x0100",eReadWrite},
	{0x8CD7 ,0x40,"0x0100",eReadWrite},
	{0x8CD8 ,0x58,"0x0100",eReadWrite},
	{0x8CD9 ,0x2C,"0x0100",eReadWrite},
	{0x8CDA ,0x45,"0x0100",eReadWrite},
	{0x8CDB ,0x00,"0x0100",eReadWrite},
	{0x8CDC ,0x80,"0x0100",eReadWrite},
	{0x8CDD ,0x38,"0x0100",eReadWrite},
	{0x8CDE ,0x74,"0x0100",eReadWrite},

   {0x0000 ,0x00,"eTableEnd",eTableEnd}
};


const IsiRegDescription_t TC358749XBG_g_svga[] =
{

    {0x0000 ,0x00,"eTableEnd",eTableEnd}
};

const IsiRegDescription_t TC358749XBG_g_1600x1200[] =
{
    {0x0000 ,0x00,"eTableEnd",eTableEnd}

};


