ata, dev->cfg.bt_datalen,
			 bcm5974_irq_button, dev, 1);

	usb_fill_int_urb(dev->tp_urb, udev,
			 usb_rcvintpipe(udev, cfg->tp_ep),
			 dev->tp_data, dev->cfg.tp_datalen,
			 bcm5974_irq_trackpad, dev, 1);

	/* create bcm5974 device */
	usb_make_path(udev, dev->phys, sizeof(dev->phys));
	strlcat(dev->phys, "/input0", sizeof(dev->phys));

	input_dev->name = "bcm5974";
	input_dev->phys = dev->phys;
	usb_to_input_id(dev->udev, &input_dev->id);
	/* report driver capabilities via the version field */
	input_dev->id.version = cfg->caps;
	input_dev->dev.parent = &iface->dev;

	input_set_drvdata(input_dev, dev);

	input_dev->open = bcm5974_open;
	input_dev->close = bcm5974_close;

	setup_events_to_report(input_dev, cfg);

	error = input_register_device(dev->input);
	if (error)
		goto err_free_buffer;

	/* save our data pointer in this interface device */
	usb_set_intfdata(iface, dev);

	return 0;

err_free_buffer:
	usb_buffer_free(dev->udev, dev->cfg.tp_datalen,
		dev->tp_data, dev->tp_urb->transfer_dma);
err_free_bt_buffer:
	usb_buffer_free(dev->udev, dev->cfg.bt_datalen,
		dev->bt_data, dev->bt_urb->transfer_dma);
err_free_urb:
	usb_free_urb(dev->tp_urb);
err_free_bt_urb:
	usb_free_urb(dev->bt_urb);
err_free_devs:
	usb_set_intfdata(iface, NULL);
	input_free_device(input_dev);
	kfree(dev);
	return error;
}

static void bcm5974_disconnect(struct usb_interface *iface)
{
	struct bcm5974 *dev = usb_get_intfdata(iface);

	usb_set_intfdata(iface, NULL);

	input_unregister_device(dev->input);
	usb_buffer_free(dev->udev, dev->cfg.tp_datalen,
			dev->tp_data, dev->tp_urb->transfer_dma);
	usb_buffer_free(dev->udev, dev->cfg.bt_datalen,
			dev->bt_data, dev->bt_urb->transfer_dma);
	usb_free_urb(dev->tp_urb);
	usb_free_urb(dev->bt_urb);
	kfree(dev);
}

static struct usb_driver bcm5974_driver = {
	.name			= "bcm5974",
	.probe			= bcm5974_probe,
	.disconnect		= bcm5974_disconnect,
	.suspend		= bcm5974_suspend,
	.resume			= bcm5974_resume,
	.reset_resume		= bcm5974_resume,
	.id_table		= bcm5974_table,
	.supports_autosuspend	= 1,
};

static int __init bcm5974_init(void)
{
	return usb_register(&bcm5974_driver);
}

static void __exit bcm5974_exit(void)
{
	usb_deregister(&bcm5974_driver);
}

module_init(bcm5974_init);
module_exit(bcm5974_exit);

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         /*
 *  Copyright (c) 1999-2001 Vojtech Pavlik
 *  Copyright (c) 1999 Brian Gerst
 */

/*
 * NS558 based standard IBM game port driver for Linux
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */

#include <asm/io.h>

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/gameport.h>
#include <linux/slab.h>
#include <linux/pnp.h>

MODULE_AUTHOR("Vojtech Pavlik <vojtech@ucw.cz>");
MODULE_DESCRIPTION("Classic gameport (ISA/PnP) driver");
MODULE_LICENSE("GPL");

static int ns558_isa_portlist[] = { 0x201, 0x200, 0x202, 0x203, 0x204, 0x205, 0x207, 0x209,
				    0x20b, 0x20c, 0x20e, 0x20f, 0x211, 0x219, 0x101, 0 };

struct ns558 {
	int type;
	int io;
	int size;
	struct pnp_dev *dev;
	struct gameport *gameport;
	struct list_head node;
};

static LIST_HEAD(ns558_list);

/*
 * ns558_isa_probe() tries to find an isa gameport at the
 * specified address, and also checks for mirrors.
 * A joystick must be attached for this to work.
 */

static int ns558_isa_probe(int io)
{
	int i, j, b;
	unsigned char c, u, v;
	struct ns558 *ns558;
	struct gameport *port;

/*
 * No one should be using this address.
 */

	if (!request_region(io, 1, "ns558-isa"))
		return -EBUSY;

/*
 * We must not be able to write arbitrary values to the port.
 * The lower two axis bits must be 1 after a write.
 */

	c = inb(io);
	outb(~c & ~3, io);
	if (~(u = v = inb(io)) & 3) {
		outb(c, io);
		release_region(io, 1);
		return -ENODEV;
	}
/*
 * After a trigger, there must be at least some bits changing.
 */

	for (i = 0; i < 1000; i++) v &= inb(io);

	if (u == v) {
		outb(c, io);
		release_region(io, 1);
		return -ENODEV;
	}
	msleep(3);
/*
 * After some time (4ms) the axes shouldn't change anymore.
 */

	u = inb(io);
	for (i = 0; i < 1000; i++)
		if ((u ^ inb(io)) & 0xf) {
			outb(c, io);
			release_region(io, 1);
			return -ENODEV;
		}
/*
 * And now find the number of mirrors of the port.
 */

	for (i = 1; i < 5; i++) {

		release_region(io & (-1 << (i - 1)), (1 << (i - 1)));

		if (!request_region(io & (-1 << i), (1 << i), "ns558-isa"))
			break;				/* Don't disturb anyone */

		outb(0xff, io & (-1 << i));
		for (j = b = 0; j < 1000; j++)
			if (inb(io & (-1 << i)) != inb((io & (-1 << i)) + (1 << i) - 1)) b++;
		msleep(3);

		if (b > 300) {				/* We allow 30% difference */
			release_region(io & (-1 << i), (1 << i));
			break;
		}
	}

	i--;

	if (i != 4) {
		if (!request_region(io & (-1 << i), (1 << i), "ns558-isa"))
			return -EBUSY;
	}

	ns558 = kzalloc(sizeof(struct ns558), GFP_KERNEL);
	port = gameport_allocate_port();
	if (!ns558 || !port) {
		printk(KERN_ERR "ns558: Memory allocation failed.\n");
		release_region(io & (-1 << i), (1 << i));
		kfree(ns558);
		gameport_free_port(port);
		return -ENOMEM;
	}

	ns558->io = io;
	ns558->size = 1 << i;
	ns558->gameport = port;

	port->io = io;
	gameport_set_name(port, "NS558 ISA Gameport");
	gameport_set_phys(port, "isa%04x/gameport0", io & (-1 << i));

	gameport_register_port(port);

	list_add(&ns558->node, &ns558_list);

	return 0;
}

#ifdef CONFIG_PNP

static struct pnp_device_id pnp_devids[] = {
	{ .id = "@P@0001", .driver_data = 0 }, /* ALS 100 */
	{ .id = "@P@0020", .driver_data = 0 }, /* ALS 200 */
	{ .id = "@P@1001", .driver_data = 0 }, /* ALS 100+ */
	{ .id = "@P@2001", .driver_data = 0 }, /* ALS 120 */
	{ .id = "ASB16fd", .driver_data = 0 }, /* AdLib NSC16 */
	{ .id = "AZT3001", .driver_data = 0 }, /* AZT1008 */
	{ .id = "CDC0001", .driver_data = 0 }, /* Opl3-SAx */
	{ .id = "CSC0001", .driver_data = 0 }, /* CS4232 */
	{ .id = "CSC000f", .driver_data = 0 }, /* CS4236 */
	{ .id = "CSC0101", .driver_data = 0 }, /* CS4327 */
	{ .id = "CTL7001", .driver_data = 0 }, /* SB16 */
	{ .id = "CTL7002", .driver_data = 0 }, /* AWE64 */
	{ .id = "CTL7005", .driver_data = 0 }, /* Vibra16 */
	{ .id = "ENS2020", .driver_data = 0 }, /* SoundscapeVIVO */
	{ .id = "ESS0001", .driver_data = 0 }, /* ES1869 */
	{ .id = "ESS0005", .driver_data = 0 }, /* ES1878 */
	{ .id = "ESS6880", .driver_data = 0 }, /* ES688 */
	{ .id = "IBM0012", .driver_data = 0 }, /* CS4232 */
	{ .id = "OPT0001", .driver_data = 0 }, /* OPTi Audio16 */
	{ .id = "YMH0006", .driver_data = 0 }, /* Opl3-SA */
	{ .id = "YMH0022", .driver_data = 0 }, /* Opl3-SAx */
	{ .id = "PNPb02f", .driver_data = 0 }, /* Generic */
	{ .id = "", },
};

MODULE_DEVICE_TABLE(pnp, pnp_devids);

static int ns558_pnp_probe(struct pnp_dev *dev, const struct pnp_device_id *did)
{
	int ioport, iolen;
	struct ns558 *ns558;
	struct gameport *port;

	if (!pnp_port_valid(dev, 0)) {
		printk(KERN_WARNING "ns558: No i/o ports on a gameport? Weird\n");
		return -ENODEV;
	}

	ioport = pnp_port_start(dev, 0);
	iolen = pnp_port_len(dev, 0);

	if (!request_region(ioport, iolen, "ns558-pnp"))
		return -EBUSY;

	ns558 = kzalloc(sizeof(struct ns558), GFP_KERNEL);
	port = gameport_allocate_port();
	if (!ns558 || !port) {
		printk(KERN_ERR "ns558: Memory allocation failed\n");
		kfree(ns558);
		gameport_free_port(port);
		return -ENOMEM;
	}

	ns558->io = ioport;
	ns558->size = iolen;
	ns558->dev = dev;
	ns558->gameport = port;

	gameport_set_name(port, "NS558 PnP Gameport");
	gameport_set_phys(port, "pnp%s/gameport0", dev_name(&dev->dev));
	port->dev.parent = &dev->dev;
	port->io = ioport;

	gameport_register_port(port);

	list_add_tail(&ns558->node, &ns558_list);
	return 0;
}

static struct pnp_driver ns558_pnp_driver = {
	.name		= "ns558",
	.id_table	= pnp_devids,
	.probe		= ns558_pnp_probe,
};

#else

static struct pnp_driver ns558_pnp_driver;

#endif

static int __init ns558_init(void)
{
	int i = 0;
	int error;

	error = pnp_register_driver(&ns558_pnp_driver);
	if (error && error != -ENODEV)	/* should be ENOSYS really */
		return error;

/*
 * Probe ISA ports after PnP, so that PnP ports that are already
 * enabled get detected as PnP. This may be suboptimal in multi-device
 * configurations, but saves hassle with simple setups.
 */

	while (ns558_isa_portlist[i])
		ns558_isa_probe(ns558_isa_portlist[i++]);

	return list_empty(&ns558_list) && error ? -ENODEV : 0;
}

static void __exit ns558_exit(void)
{
	struct ns558 *ns558, *safe;

	list_for_each_entry_safe(ns558, safe, &ns558_list, node) {
		gameport_unregister_port(ns558->gameport);
		release_region(ns558->io & ~(ns558->size - 1), ns558->size);
		kfree(ns558);
	}

	pnp_unregister_driver(&ns558_pnp_driver);
}

module_init(ns558_init);
module_exit(ns558_exit);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 P3
# 224-color SuperH Linux logo
80 80
255
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  6   6   6   6   6   6  10  10  10  10  10  10
 10  10  10   6   6   6   6   6   6   6   6   6
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   6   6   6  10  10  10  14  14  13
 22  22  22  26  26  26  30  30  30  34  34  34
 30  30  30  30  30  30  26  26  26  18  18  18
 14  14  13  10  10  10   6   6   6   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  6   6   6  14  14  13  26  26  26  42  42  43
 54  54  54  66  66  66  78  78  78  78  78  78
 78  78  78  74  74  74  66  66  66  54  54  54
 42  42  43  26  26  26  18  18  18  10  10  10
  6   6   6   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2
  2   2   2   2   2   2   2   2   2   2   2   2