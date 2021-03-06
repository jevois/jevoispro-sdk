
/*
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <environment.h>
#include <fdt_support.h>
#include <libfdt.h>
#ifdef CONFIG_SYS_I2C_AML
#include <aml_i2c.h>
#include <asm/arch/secure_apb.h>
#endif
#include <amlogic/canvas.h>
#ifdef CONFIG_AML_VPU
#include <vpu.h>
#endif
#include <vpp.h>
#ifdef CONFIG_AML_V2_FACTORY_BURN
#include <amlogic/aml_v2_burning.h>
#endif// #ifdef CONFIG_AML_V2_FACTORY_BURN
#ifdef CONFIG_AML_HDMITX20
#include <amlogic/hdmi.h>
#endif
#include <asm/arch/eth_setup.h>
#include <phy.h>
#include <asm/cpu_id.h>
#include <dm.h>
#ifdef CONFIG_AML_SPIFC
#include <amlogic/spifc.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

//new static eth setup
struct eth_board_socket*  eth_board_skt;


int serial_set_pin_port(unsigned long port_base)
{
    //UART in "Always On Module"
    //GPIOAO_0==tx,GPIOAO_1==rx
    //setbits_le32(P_AO_RTI_PIN_MUX_REG,3<<11);
    return 0;
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	return 0;
}

/* secondary_boot_func
 * this function should be write with asm, here, is is only for compiling pass
 * */
void secondary_boot_func(void)
{
}
void internalPhyConfig(struct phy_device *phydev)
{
	/*Enable Analog and DSP register Bank access by*/
	phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x0000);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x0400);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x0000);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x0400);
	/*Write Analog register 23*/
	phy_write(phydev, MDIO_DEVAD_NONE, 0x17, 0x8E0D);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x4417);
	/*Enable fractional PLL*/
	phy_write(phydev, MDIO_DEVAD_NONE, 0x17, 0x0005);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x5C1B);
	//Programme fraction FR_PLL_DIV1
	phy_write(phydev, MDIO_DEVAD_NONE, 0x17, 0x029A);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x5C1D);
	//## programme fraction FR_PLL_DiV1
	phy_write(phydev, MDIO_DEVAD_NONE, 0x17, 0xAAAA);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x14, 0x5C1C);
}

#ifdef CONFIG_INTERNAL_PHY
static void setup_net_chip(void)
{
	eth_aml_reg0_t eth_reg0;

	eth_reg0.d32 = 0;
	eth_reg0.b.phy_intf_sel = 0;
	eth_reg0.b.data_endian = 0;
	eth_reg0.b.desc_endian = 0;
	eth_reg0.b.rx_clk_rmii_invert = 0;
	eth_reg0.b.rgmii_tx_clk_src = 0;
	eth_reg0.b.rgmii_tx_clk_phase = 0;
	eth_reg0.b.rgmii_tx_clk_ratio = 0;
	eth_reg0.b.phy_ref_clk_enable = 0;
	eth_reg0.b.clk_rmii_i_invert = 1;
	eth_reg0.b.clk_en = 1;
	eth_reg0.b.adj_enable = 0;
	eth_reg0.b.adj_setup = 0;
	eth_reg0.b.adj_delay = 0;
	eth_reg0.b.adj_skew = 0;
	eth_reg0.b.cali_start = 0;
	eth_reg0.b.cali_rise = 0;
	eth_reg0.b.cali_sel = 0;
	eth_reg0.b.rgmii_rx_reuse = 0;
	eth_reg0.b.eth_urgent = 0;
	setbits_le32(P_PREG_ETH_REG0, eth_reg0.d32);// rmii mode
	*P_PREG_ETH_REG2 = 0x10110181;
	*P_PREG_ETH_REG3 = 0xe409087f;
	setbits_le32(HHI_GCLK_MPEG1,1<<3);
	/* power on memory */
	clrbits_le32(HHI_MEM_PD_REG0, (1 << 3) | (1<<2));

}
#else
static void setup_net_chip(void)
{
	eth_aml_reg0_t eth_reg0;

	//setup ethernet clk need calibrate to configre
	setbits_le32(P_PERIPHS_PIN_MUX_4, 0xfffc00);

	eth_reg0.d32 = 0;
	eth_reg0.b.phy_intf_sel = 1;
	eth_reg0.b.data_endian = 0;
	eth_reg0.b.desc_endian = 0;
	eth_reg0.b.rx_clk_rmii_invert = 0;
	eth_reg0.b.rgmii_tx_clk_src = 0;
	eth_reg0.b.rgmii_tx_clk_phase = 1;
	eth_reg0.b.rgmii_tx_clk_ratio = 4;
	eth_reg0.b.phy_ref_clk_enable = 1;
	eth_reg0.b.clk_rmii_i_invert = 0;
	eth_reg0.b.clk_en = 1;
	eth_reg0.b.adj_enable = 0;
	eth_reg0.b.adj_setup = 0;
	eth_reg0.b.adj_delay = 0;
	eth_reg0.b.adj_skew = 0;
	eth_reg0.b.cali_start = 0;
	eth_reg0.b.cali_rise = 0;
	eth_reg0.b.cali_sel = 0;
	eth_reg0.b.rgmii_rx_reuse = 0;
	eth_reg0.b.eth_urgent = 0;
	setbits_le32(P_PREG_ETH_REG0, eth_reg0.d32);// rgmii mode

	setbits_le32(HHI_GCLK_MPEG1,1<<3);

	/* power on memory */
	clrbits_le32(HHI_MEM_PD_REG0, (1 << 3) | (1<<2));

	/* hardware reset ethernet phy : gpioz14 connect phyreset pin*/
	clrbits_le32(PREG_PAD_GPIO3_EN_N, 1 << 14);
	clrbits_le32(PREG_PAD_GPIO3_O, 1 << 14);
	udelay(10000);
	setbits_le32(PREG_PAD_GPIO3_O, 1 << 14);
}
#endif


extern struct eth_board_socket* eth_board_setup(char *name);
extern int designware_initialize(ulong base_addr, u32 interface);
int board_eth_init(bd_t *bis)
{
	setup_net_chip();
	udelay(1000);
#ifdef CONFIG_INTERNAL_PHY
	designware_initialize(ETH_BASE, PHY_INTERFACE_MODE_RMII);
#else
	designware_initialize(ETH_BASE, PHY_INTERFACE_MODE_RGMII);
#endif
	return 0;
}

#if CONFIG_AML_SD_EMMC
#include <mmc.h>
#include <asm/arch/sd_emmc.h>
static int  sd_emmc_init(unsigned port)
{
    switch (port)
	{
		case SDIO_PORT_A:
			break;
		case SDIO_PORT_B:
			//todo add card detect
			//setbits_le32(P_PREG_PAD_GPIO5_EN_N,1<<29);//CARD_6
			break;
		case SDIO_PORT_C:
			//enable pull up
			//clrbits_le32(P_PAD_PULL_UP_REG3, 0xff<<0);
			break;
		default:
			break;
	}

	return cpu_sd_emmc_init(port);
}

extern unsigned sd_debug_board_1bit_flag;
static int  sd_emmc_detect(unsigned port)
{
	int ret;
    switch (port) {

	case SDIO_PORT_A:
			break;
	case SDIO_PORT_B:
		setbits_le32(P_PREG_PAD_GPIO2_EN_N, 1 << 26);//CARD_6
		ret = readl(P_PREG_PAD_GPIO2_I) & (1 << 26) ? 0 : 1;
		printf("%s\n", ret ? "card in" : "card out");
		if ((readl(P_PERIPHS_PIN_MUX_6) & (3 << 8))) { //if uart pinmux set, debug board in
			if (!(readl(P_PREG_PAD_GPIO2_I) & (1 << 24))) {
				printf("sdio debug board detected, sd card with 1bit mode\n");
				sd_debug_board_1bit_flag = 1;
			} else{
				printf("sdio debug board detected, no sd card in\n");
				sd_debug_board_1bit_flag = 0;
				return 1;
			}
		}
		break;
	default:
		break;
	}
	return 0;
}


static void sd_emmc_pwr_prepare(unsigned port)
{
	cpu_sd_emmc_pwr_prepare(port);
}

static void sd_emmc_pwr_on(unsigned port)
{
    switch (port)
	{
		case SDIO_PORT_A:
			break;
		case SDIO_PORT_B:
//            clrbits_le32(P_PREG_PAD_GPIO5_O,(1<<31)); //CARD_8
//            clrbits_le32(P_PREG_PAD_GPIO5_EN_N,(1<<31));
			/// @todo NOT FINISH
			break;
		case SDIO_PORT_C:
			break;
		default:
			break;
	}
	return;
}
static void sd_emmc_pwr_off(unsigned port)
{
	/// @todo NOT FINISH
    switch (port)
	{
		case SDIO_PORT_A:
			break;
		case SDIO_PORT_B:
//            setbits_le32(P_PREG_PAD_GPIO5_O,(1<<31)); //CARD_8
//            clrbits_le32(P_PREG_PAD_GPIO5_EN_N,(1<<31));
			break;
		case SDIO_PORT_C:
			break;
				default:
			break;
	}
	return;
}

// #define CONFIG_TSD      1
static void board_mmc_register(unsigned port)
{
	struct aml_card_sd_info *aml_priv=cpu_sd_emmc_get(port);
    if (aml_priv == NULL)
		return;

	aml_priv->sd_emmc_init=sd_emmc_init;
	aml_priv->sd_emmc_detect=sd_emmc_detect;
	aml_priv->sd_emmc_pwr_off=sd_emmc_pwr_off;
	aml_priv->sd_emmc_pwr_on=sd_emmc_pwr_on;
	aml_priv->sd_emmc_pwr_prepare=sd_emmc_pwr_prepare;
	aml_priv->desc_buf = malloc(NEWSD_MAX_DESC_MUN*(sizeof(struct sd_emmc_desc_info)));

	if (NULL == aml_priv->desc_buf)
		printf(" desc_buf Dma alloc Fail!\n");
	else
		printf("aml_priv->desc_buf = 0x%p\n",aml_priv->desc_buf);

	sd_emmc_register(aml_priv);
}
int board_mmc_init(bd_t	*bis)
{
#ifdef CONFIG_VLSI_EMULATOR
	//board_mmc_register(SDIO_PORT_A);
#else
	//board_mmc_register(SDIO_PORT_B);
#endif
	board_mmc_register(SDIO_PORT_B);
	board_mmc_register(SDIO_PORT_C);
//	board_mmc_register(SDIO_PORT_B1);
	return 0;
}

#ifdef CONFIG_AML_SPIFC
/*
 * BOOT_11: NOR_D:reg7[13], clr reg7[4] + reg7[15]
 * BOOT_12: NOR_Q:reg7[12], clr reg7[3]
 * BOOT_13: NOR_C:reg7[11], clr reg7[2]
 * BOOT_15: NOR_CS:reg7[10],clr reg7[0] + reg7[28]
 */
#define SPIFC_NUM_CS 1
static int spifc_cs_gpios[SPIFC_NUM_CS] = {97/*PIN_BOOT_15*/};

static int spifc_pinctrl_enable(void *pinctrl, bool enable)
{
    unsigned int val;

    val = readl(P_PERIPHS_PIN_MUX_7);
    val &= ~((1<<13)|(1<<12)|(1<<11)|(1<<10));
    if (enable) {
        val |= (1<<13)|(1<<12)|(1<<11);
        val &= ~((1<<28)|(1<<15)|(1<<4)|(1<<3)|(1<<2)|(1<<0));
    }
    writel(val, P_PERIPHS_PIN_MUX_7);

    return 0;
}

static const struct spifc_platdata spifc_platdata = {
    .reg = 0xc1108c80,
    .mem_map = 0xcc000000,
    .pinctrl_enable = spifc_pinctrl_enable,
    .num_chipselect = SPIFC_NUM_CS,
    .cs_gpios = spifc_cs_gpios,
};

U_BOOT_DEVICE(spifc) = {
    .name = "spifc",
    .platdata = &spifc_platdata,
};
#endif /* CONFIG_AML_SPIFC */


#ifdef CONFIG_SYS_I2C_AML
static void board_i2c_set_pinmux(void){
	
	// Disable TCON_CPH / TCON_OEH / TCON_VCOM
	clrbits_le32(P_PERIPHS_PIN_MUX_1, ((1<<20)|(1<<18)|(1<<19)));
	// Disable UART_CTS_B / UART_RTS_B
	clrbits_le32(P_PERIPHS_PIN_MUX_2, ((1<<14)|(1<<13)));
	// Enable I2C_SCK_B / I2C_SDA_B
	setbits_le32(P_PERIPHS_PIN_MUX_1,
	(MESON_I2C_MASTER_B_GPIODV_27_BIT | MESON_I2C_MASTER_B_GPIODV_26_BIT));

	udelay(10);
};
struct aml_i2c_platform g_aml_i2c_plat = {
	.wait_count         = 1000000,
	.wait_ack_interval  = 5,
	.wait_read_interval = 5,
	.wait_xfer_interval = 5,
	.master_no          = AML_I2C_MASTER_B,
	.use_pio            = 0,
	.master_i2c_speed   = AML_I2C_SPPED_100K,
	.master_b_pinmux = {
		.scl_reg    = (unsigned long)MESON_I2C_MASTER_B_GPIODV_27_REG,
		.scl_bit    = MESON_I2C_MASTER_B_GPIODV_27_BIT,
		.sda_reg    = (unsigned long)MESON_I2C_MASTER_B_GPIODV_26_REG,
		.sda_bit    = MESON_I2C_MASTER_B_GPIODV_26_BIT,
	}
};
static void board_i2c_init(void)
{
	//set I2C pinmux with PCB board layout
	board_i2c_set_pinmux();

	//Amlogic I2C controller initialized
	//note: it must be call before any I2C operation
//	i2c_init(g_aml_i2c_plat.master_i2c_speed, 0x0);
	i2c_plat_init();
	aml_i2c_init();

	udelay(10);
}
#endif
#endif

#if defined(CONFIG_BOARD_EARLY_INIT_F)
int board_early_init_f(void){
	/*add board early init function here*/
	return 0;
}
#endif

#ifdef CONFIG_USB_XHCI_AMLOGIC_GXL
#include <asm/arch/usb-new.h>
#include <asm/arch/gpio.h>
#define CONFIG_GXL_USB_U2_PORT_NUM	3
#define CONFIG_GXL_USB_U3_PORT_NUM	0

struct amlogic_usb_config g_usb_config_GXL_skt={
	CONFIG_GXL_XHCI_BASE,
	USB_ID_MODE_HARDWARE,
	NULL,//gpio_set_vbus_power, //set_vbus_power
	CONFIG_GXL_USB_PHY2_BASE,
	CONFIG_GXL_USB_PHY3_BASE,
	CONFIG_GXL_USB_U2_PORT_NUM,
	CONFIG_GXL_USB_U3_PORT_NUM,
};
#endif /*CONFIG_USB_XHCI_AMLOGIC*/

#ifdef CONFIG_AML_HDMITX20
static void hdmi_tx_set_hdmi_5v(void)
{
	/*Power on VCC_5V for HDMI_5V*/
	clrbits_le32(P_PREG_PAD_GPIO1_EN_N, 1 << 23);
	clrbits_le32(P_PREG_PAD_GPIO1_O, 1 << 23);
}
#endif

int board_init(void)
{
    //Please keep CONFIG_AML_V2_FACTORY_BURN at first place of board_init
#ifdef CONFIG_AML_V2_FACTORY_BURN
	if ((0x1b8ec003 != readl(P_PREG_STICKY_REG2)) && (0x1b8ec004 != readl(P_PREG_STICKY_REG2))) {
		aml_try_factory_usb_burning(0, gd->bd);
	}
#endif// #ifdef CONFIG_AML_V2_FACTORY_BURN
	
	clrbits_le32(PREG_PAD_GPIO0_EN_N, (1 << 2));
	clrbits_le32(PREG_PAD_GPIO0_O, 1 << 2);

	/*for LED*/
	//clear pinmux
	clrbits_le32(AO_RTI_PIN_MUX_REG, ((1<<3)|(1<<4)));
	clrbits_le32(AO_RTI_PIN_MUX_REG2, ((1<<1)|(1<<31)));
	//set output mode
	clrbits_le32(PREG_PAD_GPIO0_EN_N, (1 << 24));
	//set output 1
	setbits_le32(PREG_PAD_GPIO0_O, (1 << 24));

	/*Power on GPIOAO_2 for VCC_5V*/
	clrbits_le32(P_AO_GPIO_O_EN_N, ((1<<2)|(1<<18)));
#ifdef CONFIG_USB_XHCI_AMLOGIC_GXL
	board_usb_init(&g_usb_config_GXL_skt,BOARD_USB_MODE_HOST);
#endif /*CONFIG_USB_XHCI_AMLOGIC*/
	canvas_init();

#ifndef CONFIG_AML_IRDETECT_EARLY
#ifdef CONFIG_AML_HDMITX20
	hdmi_tx_set_hdmi_5v();
	hdmi_tx_init();
#endif
#endif
#ifdef CONFIG_AML_NAND
	extern int amlnf_init(unsigned char flag);
	amlnf_init(0);
#endif
#ifdef CONFIG_SYS_I2C_AML
	board_i2c_init();
#endif

	return 0;
}
#ifdef CONFIG_AML_IRDETECT_EARLY
#ifdef CONFIG_AML_HDMITX20
static int do_hdmi_init(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	hdmi_tx_set_hdmi_5v();
	hdmi_tx_init();
return 0;
}

U_BOOT_CMD(hdmi_init, CONFIG_SYS_MAXARGS, 0, do_hdmi_init,
	   "HDMI_INIT sub-system",
	"hdmit init\n")
#endif
#endif
#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void){
	int ret;

	//update env before anyone using it
	run_command("get_rebootmode; echo reboot_mode=${reboot_mode}; "\
			"if test ${reboot_mode} = factory_reset; then "\
			"defenv_reserv;save; fi;", 0);
	run_command("if itest ${upgrade_step} == 1; then "\
				"defenv_reserv; setenv upgrade_step 2; saveenv; fi;", 0);

	/*add board late init function here*/
	ret = run_command("store dtb read $dtb_mem_addr", 1);
	if (ret) {
		printf("%s(): [store dtb read $dtb_mem_addr] fail\n", __func__);
		#ifdef CONFIG_DTB_MEM_ADDR
		char cmd[64];
		printf("load dtb to %x\n", CONFIG_DTB_MEM_ADDR);
		sprintf(cmd, "store dtb read %x", CONFIG_DTB_MEM_ADDR);
		ret = run_command(cmd, 1);
		if (ret) {
			printf("%s(): %s fail\n", __func__, cmd);
		}
		#endif
	}

#ifdef CONFIG_AML_VPU
	vpu_probe();
#endif
	vpp_init();
#ifndef CONFIG_AML_IRDETECT_EARLY
	/* after  */
	run_command("cvbs init;hdmitx hpd", 0);
	run_command("vout output $outputmode", 0);
#endif

#ifdef CONFIG_AML_V2_FACTORY_BURN
	if (0x1b8ec003 == readl(P_PREG_STICKY_REG2))
		aml_try_factory_usb_burning(1, gd->bd);
	aml_try_factory_sdcard_burning(0, gd->bd);
#endif// #ifdef CONFIG_AML_V2_FACTORY_BURN
	if (get_cpu_id().family_id == MESON_CPU_MAJOR_ID_GXL) {
		setenv("maxcpus","4");
		setenv("device_product","p230");
	}
	if (get_cpu_id().family_id == MESON_CPU_MAJOR_ID_GXM) {
		setenv("maxcpus","8");
		setenv("device_product","q200");
	}

	/* load uboot pq value */
	vpp_pq_load();

	return 0;
}
#endif

phys_size_t get_effective_memsize(void)
{
	// >>16 -> MB, <<20 -> real size, so >>16<<20 = <<4
#if defined(CONFIG_SYS_MEM_TOP_HIDE)
	return (((readl(AO_SEC_GP_CFG0)) & 0xFFFF0000) << 4) - CONFIG_SYS_MEM_TOP_HIDE;
#else
	return (((readl(AO_SEC_GP_CFG0)) & 0xFFFF0000) << 4);
#endif
}

#ifdef CONFIG_DDR_AUTO_DTB
int check_ddrsize(void)
{
	unsigned int ddr_size=0;
	int i;
	for (i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
		ddr_size += gd->bd->bi_dram[i].size;
	}
#if defined(CONFIG_SYS_MEM_TOP_HIDE)
		ddr_size += CONFIG_SYS_MEM_TOP_HIDE;
#endif
	switch (ddr_size) {
	case 0x80000000:
		setenv("ddr_size", "2"); //2G DDR
		break;
	case 0xc0000000:
		setenv("ddr_size", "3"); //3G DDR
		break;
	default:
		setenv("ddr_size", "0");
		break;
	}
	return 0;
}
#endif

const char * const _env_args_reserve_[] =
{
	"aml_dt",
	"firstboot",
	"ddr_size",
	"lock",
	"upgrade_step",

	NULL//Keep NULL be last to tell END
};

