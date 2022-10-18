#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2e712c61, "module_layout" },
	{ 0x58aa99e6, "param_ops_int" },
	{ 0xa2a8d896, "i2c_del_driver" },
	{ 0xf197b61b, "i2c_register_driver" },
	{ 0x9f20dc20, "v4l2_async_register_subdev" },
	{ 0x6a83e9b9, "media_entity_pads_init" },
	{ 0x98fbb6be, "v4l2_ctrl_handler_setup" },
	{ 0xc871a257, "_dev_err" },
	{ 0xf577f1f3, "v4l2_ctrl_new_int_menu" },
	{ 0xe9ed4584, "v4l2_ctrl_new_std" },
	{ 0x634a8148, "v4l2_ctrl_handler_init_class" },
	{ 0xbfcd8389, "v4l2_i2c_subdev_init" },
	{ 0x5639ed91, "i2c_new_dummy_device" },
	{ 0x2915b3c8, "devm_kmalloc" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0x815588a6, "clk_enable" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0x6d55e5dc, "_dev_info" },
	{ 0x3fa469d9, "_dev_warn" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0x8d7fea70, "i2c_transfer" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x1952ac80, "__v4l2_ctrl_modify_range" },
	{ 0xd3854084, "v4l2_ctrl_handler_free" },
	{ 0x69fb9b69, "v4l2_async_unregister_subdev" },
	{ 0x12d7b385, "i2c_unregister_device" },
	{ 0x73e20c1c, "strlcpy" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "videodev,mc");

MODULE_ALIAS("of:N*T*Comnivision,inno_mipi_ov9281");
MODULE_ALIAS("of:N*T*Comnivision,inno_mipi_ov9281C*");
MODULE_ALIAS("i2c:inno_mipi_ov9281");

MODULE_INFO(srcversion, "C54FDEA26AB5920AE428AD5");
