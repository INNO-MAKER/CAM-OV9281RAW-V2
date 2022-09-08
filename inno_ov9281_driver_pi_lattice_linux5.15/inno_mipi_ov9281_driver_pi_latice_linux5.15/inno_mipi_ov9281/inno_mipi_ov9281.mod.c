#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

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
	{ 0x7d0b00ef, "module_layout" },
	{ 0xe40769c1, "param_ops_int" },
	{ 0x18321fdf, "i2c_del_driver" },
	{ 0x2b8189c2, "i2c_register_driver" },
	{ 0x9220962b, "v4l2_async_register_subdev" },
	{ 0x3d4f5f40, "media_entity_pads_init" },
	{ 0x11f03218, "v4l2_ctrl_handler_setup" },
	{ 0xdc88f81d, "_dev_err" },
	{ 0x33086fb2, "v4l2_ctrl_new_int_menu" },
	{ 0x72db2f3c, "v4l2_ctrl_new_std" },
	{ 0x10899116, "v4l2_ctrl_handler_init_class" },
	{ 0x4d58e216, "v4l2_i2c_subdev_init" },
	{ 0xcc656c0a, "_dev_info" },
	{ 0x4da8b19c, "i2c_new_dummy_device" },
	{ 0x4cd3ff7e, "devm_kmalloc" },
	{ 0x815588a6, "clk_enable" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0xfe6bc7c7, "_dev_warn" },
	{ 0xd0b3ddca, "i2c_transfer" },
	{ 0xf0165808, "__v4l2_ctrl_modify_range" },
	{ 0xf0de61ec, "v4l2_ctrl_handler_free" },
	{ 0xd0d86f2c, "v4l2_async_unregister_subdev" },
	{ 0xe5079f75, "i2c_unregister_device" },
	{ 0x5792f848, "strlcpy" },
};

MODULE_INFO(depends, "v4l2-async,mc,videodev");

MODULE_ALIAS("of:N*T*Comnivision,inno_mipi_ov9281");
MODULE_ALIAS("of:N*T*Comnivision,inno_mipi_ov9281C*");
MODULE_ALIAS("i2c:inno_mipi_ov9281");

MODULE_INFO(srcversion, "985139CD214760A26CBE1E6");
