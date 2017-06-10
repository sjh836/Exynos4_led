#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#define DRIVER_AUTHOR      "hanback"
#define DRIVER_DESC      "Led program"

#define LED_NAME          "m2_led"
#define LED_MODULE_VERSION    "LED V1.0"
	
static int m2_led_open(struct inode * inode, struct file * file) {
	int err,i;

	for (i=0; i<7; i++) {
		err = gpio_request(EXYNOS4_GPK3(i), "Led");

		if (err)
			printk("led.c failed to request GPK3(%d) \n",i);

		s3c_gpio_setpull(EXYNOS4_GPK3(i), S3C_GPIO_PULL_NONE);
		gpio_direction_output(EXYNOS4_GPK3(i), 0);
	}
	
	return 0;
}

static int m2_led_release(struct inode * inode, struct file * file) {
	int i;
	for (i=0; i<7; i++)
		gpio_free(EXYNOS4_GPK3(i));
	
	return 0;
}

static ssize_t m2_led_read(struct file * file, char * buf, size_t length, loff_t * ofs) {
	printk("m2_led_read, \n");
	
	return 0;
}
static ssize_t m2_led_write(struct file * file, const char * buf, size_t length, loff_t * ofs) {
	int ret, i;
	unsigned char cbuf[8];
	ret = copy_from_user(cbuf,buf,length);

	for (i=0; i<7; i++) {
		gpio_direction_output(EXYNOS4_GPK3(i), (unsigned int) cbuf[i]);
	}
	
	return length;
}

static DEFINE_MUTEX(m2_led_mutex);
static long m2_led_ioctl(struct inode *inode, unsigned char cmd[], size_t length, loff_t *off_what){
	int i;

	for (i=0; i<7; i++) {
		gpio_direction_output(EXYNOS4_GPK3(i), (unsigned int) cmd[i]);
	}
	
	return 0;
}

static struct file_operations m2_led_fops = {
	.owner    = THIS_MODULE,
	.open    = m2_led_open,
	.unlocked_ioctl  = m2_led_ioctl,
	.write    = m2_led_write,
	.release  = m2_led_release,
};

static struct miscdevice m2_led_driver = {
	.fops  = &m2_led_fops,
	.name = LED_NAME,
	.minor = MISC_DYNAMIC_MINOR,
};

static int m2_led_init(void){ //모듈이 커널에 적재될 떄 호출되는 함수
	int i;
	printk("Embedded classA: 2012335054 LJH, 2012335073 CJS\n");

	for (i=0; i<7; i+=2) {
		gpio_direction_output(EXYNOS4_GPK3(i), (unsigned int) 1); //GPIO포트에 있는 EXYNOS4_GPK3 LED를 ON(1)로 설정
	}

	printk("driver: %s DRIVER INIT\n",LED_NAME);
	return misc_register(&m2_led_driver);
}

static void m2_led_exit(void){ //모듈이 커널에서 제거될 떄 호출되는 함수
	misc_deregister(&m2_led_driver);
	printk("driver: %s DRIVER EXIT\n",LED_NAME);
}

module_init(m2_led_init);
module_exit(m2_led_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");